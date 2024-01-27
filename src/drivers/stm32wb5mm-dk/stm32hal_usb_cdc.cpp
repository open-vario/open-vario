

#include "stm32hal_usb_cdc.h"
#include "os.h"
#include "usbd_cdc.h"
#include "usbd_conf.h"
#include "usbd_core.h"
#include "usbd_desc.h"

#include <cstring>

namespace ov
{

/** @brief USB CDC driver instance */
static stm32hal_usb_cdc* s_instance;

/** @brief Constructor */
stm32hal_usb_cdc::stm32hal_usb_cdc()
    : m_usb{},
      m_is_link_up(false),
      m_rx_sem(0u, 1u),
      m_tx_sem(1u, 1u),
      m_listener(nullptr),
      m_ll_rx_buffer(),
      m_ll_tx_buffer(),
      m_rx_buffer()
{
    // Save instance
    s_instance = this;
}

/** @brief Initialize the driver */
bool stm32hal_usb_cdc::init()
{
    bool ret = false;

    /** @brief USB CDC interface callbacks */
    static USBD_CDC_ItfTypeDef usbd_cdc_iface = {&stm32hal_usb_cdc::iface_init,
                                                 &stm32hal_usb_cdc::iface_deinit,
                                                 &stm32hal_usb_cdc::iface_control,
                                                 &stm32hal_usb_cdc::iface_receive,
                                                 &stm32hal_usb_cdc::iface_transmit_done};

    // Initialize USB library (this will initialize all clocks and pinout)
    USBD_StatusTypeDef usb_status = USBD_Init(&m_usb, &VCP_Desc, 0u);
    if (usb_status == USBD_OK)
    {
        // Register USB CDC class
        usb_status = USBD_RegisterClass(&m_usb, USBD_CDC_CLASS);
        if (usb_status == USBD_OK)
        {
            // Register USB CDC interface callbacks
            usb_status = static_cast<USBD_StatusTypeDef>(USBD_CDC_RegisterInterface(&m_usb, &usbd_cdc_iface));
            if (usb_status == USBD_OK)
            {
                // Start USB
                usb_status = USBD_Start(&m_usb);

                ret = (usb_status == USBD_OK);
            }
        }
    }

    return ret;
}

/** @brief Read data from the serial port */
bool stm32hal_usb_cdc::read(void* buffer, size_t size, uint32_t ms_timeout)
{
    bool ret = true;

    // Read loop
    size_t   left    = size;
    uint8_t* rx_data = reinterpret_cast<uint8_t*>(buffer);
    while (ret && (left != 0u))
    {
        // Disable interrupts
        NVIC_DisableIRQ(USB_LP_IRQn);

        // Read available data
        while ((left != 0u) && m_rx_buffer.read(*rx_data))
        {
            // Next data
            rx_data++;
            left--;
        }

        // Enable interrupts
        NVIC_EnableIRQ(USB_LP_IRQn);

        // Wait for more data
        if (left != 0u)
        {
            ret = m_rx_sem.take(ms_timeout);
        }
    }

    return ret;
}

/** @brief Write data to the serial port */
bool stm32hal_usb_cdc::write(const void* buffer, size_t size)
{
    bool ret = true;

    // Write loop
    size_t         left    = size;
    const uint8_t* tx_data = reinterpret_cast<const uint8_t*>(buffer);
    while (ret && (left != 0u))
    {
        // Wait end of previous transmission
        ret = m_tx_sem.take(100u);
        if (ret)
        {
            // Copy data into transmission buffer
            size_t tx_count = left;
            if (tx_count > sizeof(m_ll_tx_buffer))
            {
                tx_count = sizeof(m_ll_tx_buffer);
            }
            memcpy(m_ll_tx_buffer, tx_data, tx_count);

            // Start transmission
            USBD_CDC_SetTxBuffer(&m_usb, m_ll_tx_buffer, tx_count);
            if (USBD_CDC_TransmitPacket(&m_usb) != USBD_OK)
            {
                ret = false;
            }

            // Next data
            left -= tx_count;
            tx_data += tx_count;
        }
    }

    return ret;
}

/** @brief Write a null-terminated string to the serial port */
bool stm32hal_usb_cdc::write(const char* str)
{
    return write(str, strlen(str));
}

/** @brief Initializes the CDC media low layer */
int8_t stm32hal_usb_cdc::iface_init()
{
    // Get instance
    USBD_HandleTypeDef* usbd_handle = &s_instance->m_usb;

    // Set Application Buffers
    USBD_CDC_SetTxBuffer(usbd_handle, s_instance->m_ll_tx_buffer, 0);
    USBD_CDC_SetRxBuffer(usbd_handle, s_instance->m_ll_rx_buffer);

    // Clear receive buffer
    s_instance->m_rx_buffer.clear();

    // Release TX semaphore
    bool higher_priority_task_woken = false;
    s_instance->m_tx_sem.release_from_isr(higher_priority_task_woken);
    os::yield_from_isr(higher_priority_task_woken);

    // Update link status
    s_instance->m_is_link_up = true;

    // Notify listener
    if (s_instance->m_listener != nullptr)
    {
        s_instance->m_listener->on_cdc_link_up();
    }

    return USBD_OK;
}
/** @brief DeInitializes the CDC media low layer */
int8_t stm32hal_usb_cdc::iface_deinit()
{
    // Release TX semaphore
    bool higher_priority_task_woken = false;
    s_instance->m_tx_sem.release_from_isr(higher_priority_task_woken);
    os::yield_from_isr(higher_priority_task_woken);

    // Update link status
    s_instance->m_is_link_up = false;

    // Notify listener
    if (s_instance->m_listener != nullptr)
    {
        s_instance->m_listener->on_cdc_link_down();
    }

    return USBD_OK;
}

/** @brief Manage the CDC class requests */
int8_t stm32hal_usb_cdc::iface_control(uint8_t, uint8_t*, uint16_t)
{
    // Ignore and accept all requests
    return USBD_OK;
}
/** @brief Data received over USB OUT endpoint are sent over CDC interface through this function */
int8_t stm32hal_usb_cdc::iface_receive(uint8_t* buff, uint32_t* length)
{
    // Get instance
    USBD_HandleTypeDef* usbd_handle = &s_instance->m_usb;

    // Check if the receive buffer was empty to notify waiting task
    bool was_empty = s_instance->m_rx_buffer.is_empty();

    // Write data into the receive buffer
    uint32_t left = (*length);
    while ((left != 0u) && (s_instance->m_rx_buffer.write(*buff)))
    {
        buff++;
        left--;
    }

    // Initiate next USB packet transfer
    USBD_CDC_ReceivePacket(usbd_handle);

    // Signal waiting tasks that data is available
    if (was_empty && !s_instance->m_rx_buffer.is_empty())
    {
        bool higher_priority_task_woken = false;
        s_instance->m_rx_sem.release_from_isr(higher_priority_task_woken);
        os::yield_from_isr(higher_priority_task_woken);
    }

    return USBD_OK;
}

/** @brief Data has been transmitted over USB IN endpoint */
int8_t stm32hal_usb_cdc::iface_transmit_done(uint8_t*, uint32_t*, uint8_t)
{
    // Notify end of transmission
    bool higher_priority_task_woken = false;
    s_instance->m_tx_sem.release_from_isr(higher_priority_task_woken);
    os::yield_from_isr(higher_priority_task_woken);

    return USBD_OK;
}

} // namespace ov

/** @brief This function handles USB low priority interrupt */
extern "C" void USB_LP_IRQHandler(void)
{
    extern PCD_HandleTypeDef s_hpcd;
    HAL_PCD_IRQHandler(&s_hpcd);
}
