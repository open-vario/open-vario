
#ifndef OV_STM32HAL_USB_CDC_H
#define OV_STM32HAL_USB_CDC_H

#include "i_usb_cdc.h"
#include "ring_buffer.h"
#include "semaphore.h"

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_pcd.h"
#include "usbd_cdc.h"

namespace ov
{

/** @brief USB CDC driver implementation using STM32HAL */
class stm32hal_usb_cdc : public i_usb_cdc
{
  public:
    /** @brief Constructor */
    stm32hal_usb_cdc();

    /** @brief Initialize the driver */
    bool init();

    /** @brief Register a listener to USB CDC events */
    void register_listener(i_listener& listener) override { m_listener = &listener; }

    /** @brief Indicate if the USB link is up */
    bool is_link_up() override { return m_is_link_up; }

    /** @brief Read data from the serial port */
    bool read(void* buffer, size_t size, uint32_t ms_timeout) override;

    /** @brief Write data to the serial port */
    bool write(const void* buffer, size_t size) override;

    /** @brief Write a null-terminated string to the serial port */
    bool write(const char* str) override;

  private:
    /** @brief HAL USB handle */
    USBD_HandleTypeDef m_usb;
    /** @brief Indicate if the USB CDC link is up */
    bool m_is_link_up;
    /** @brief Rx semaphore */
    semaphore m_rx_sem;
    /** @brief Tx semaphore */
    semaphore m_tx_sem;
    /** @brief Listener to USB CDC events */
    i_listener* m_listener;
    /** @brief  Low level USB receive buffer */
    uint8_t m_ll_rx_buffer[CDC_DATA_FS_IN_PACKET_SIZE];
    /** @brief  Low level USB transmit buffer */
    uint8_t m_ll_tx_buffer[CDC_DATA_FS_OUT_PACKET_SIZE];
    /** @brief Receive buffer */
    ring_buffer<uint8_t, 1024u> m_rx_buffer;

    /** @brief Initializes the CDC media low layer */
    static int8_t iface_init();
    /** @brief DeInitializes the CDC media low layer */
    static int8_t iface_deinit();
    /** @brief Manage the CDC class requests */
    static int8_t iface_control(uint8_t cmd, uint8_t* buff, uint16_t length);
    /** @brief Data received over USB OUT endpoint are sent over CDC interface through this function */
    static int8_t iface_receive(uint8_t* buff, uint32_t* length);
    /** @brief Data has been transmitted over USB IN endpoint */
    static int8_t iface_transmit_done(uint8_t* buff, uint32_t* length, uint8_t epnum);
};

} // namespace ov

#endif // OV_STM32HAL_USB_CDC_H
