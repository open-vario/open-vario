/*
Copyright(c) 2017 Cedric Jimenez

This file is part of Open-Vario.

Open-Vario is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Open-Vario is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Open-Vario.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Stm32l476UsbDeviceCdc.h"
#include "Stm32l476Cpu.h"
#include "IOs.h"

#include "stm32l476xx.h"
#include "usbd_core.h"
#include "usbd_cdc.h"

/** \brief Descriptors for the CDC interface */
extern USBD_DescriptorsTypeDef VCP_Desc;

namespace open_vario
{

/** \brief Rx ready flag mask */
static const uint32_t USB_CDC_RX_READY_FLAG_MASK = 1u;

/** \brief Tx ready flag mask */
static const uint32_t USB_CDC_TX_READY_FLAG_MASK = 2u;


/** \brief Constructor */
Stm32l476UsbDeviceCdc::Stm32l476UsbDeviceCdc(const Stm32l476Cpu& cpu)
: Stm32l476UsbDevice(cpu)
, m_status_flags(0u)
, m_rx_queue()
, m_ll_rx_buffer()
, m_ll_tx_buffer()
, m_listener(nullptr)
{}

/** \brief Configure the USB CDC device */
bool Stm32l476UsbDeviceCdc::configure()
{
    bool ret;
    static USBD_CDC_ItfTypeDef usbd_cdc_ops = {
                                                    &Stm32l476UsbDeviceCdc::ifaceInit,
                                                    &Stm32l476UsbDeviceCdc::ifaceDeInit,
                                                    &Stm32l476UsbDeviceCdc::ifaceControl,
                                                    &Stm32l476UsbDeviceCdc::ifaceReceive,
                                                    &Stm32l476UsbDeviceCdc::ifaceTransmitDone,
                                              };

    // Generic USB configuration
    ret = configureUSB(&VCP_Desc, USBD_CDC_CLASS);
    if (ret)
    {
        USBD_HandleTypeDef* usbd_handle = reinterpret_cast<USBD_HandleTypeDef*>(getDevice());
        USBD_StatusTypeDef usb_status = static_cast<USBD_StatusTypeDef>(USBD_CDC_RegisterInterface(usbd_handle, &usbd_cdc_ops));
        ret = (usb_status == USBD_OK);
    }

    return ret;
}

/** \brief Start the USB CDC device */
bool Stm32l476UsbDeviceCdc::start()
{
    // Generic start
    return startUSB();
}

/** \brief Stop the USB CDC device */
bool Stm32l476UsbDeviceCdc::stop()
{
    // Generic stop
    return stopUSB();
}

/** \brief Write data through the USB CDC device */
bool Stm32l476UsbDeviceCdc::write(const uint8_t* const data, const uint32_t size)
{
    bool ret = true;
    USBD_HandleTypeDef* usbd_handle = reinterpret_cast<USBD_HandleTypeDef*>(getDevice());
    uint32_t left = size;
    const uint8_t* buff = data;

    while (ret && (left != 0))
    {
        // Wait transmit ready
        uint32_t wake_up_flags = USB_CDC_TX_READY_FLAG_MASK;
        ret = m_status_flags.wait(wake_up_flags, true, IOs::getInstance().getInfiniteTimeoutValue());
        if (ret)
        {
            // Copy data to transmit
            uint32_t copy_size = left;
            if (copy_size > sizeof(m_ll_tx_buffer))
            {
                copy_size = sizeof(m_ll_tx_buffer);
            }
            NANO_STL_MEMCPY(m_ll_tx_buffer, buff, copy_size);

            // Transmit data
            USBD_CDC_SetTxBuffer(usbd_handle, const_cast<uint8_t*>(buff), copy_size);
            USBD_StatusTypeDef usb_status = static_cast<USBD_StatusTypeDef>(USBD_CDC_TransmitPacket(usbd_handle));
            ret = (usb_status == USBD_OK);

            left -= copy_size;
            buff += copy_size;
        }
    }
    
    return ret;
}

/** \brief Read data from the USB CDC device */
bool Stm32l476UsbDeviceCdc::read(uint8_t* const data, const uint32_t size, uint32_t& read_size, const uint32_t timeout)
{
    bool ret = true;
    // Disable interrupts
    disableInterrupts();

    // Check for data
    if (m_rx_queue.getCount() == 0u)
    {
        // Enable interrupts
        enableInterrupts();

        // Wait for Rx ready
        uint32_t wake_up_flags = USB_CDC_RX_READY_FLAG_MASK;
        ret = m_status_flags.wait(wake_up_flags, false, timeout);
        if (ret)
        {
            // Disable interrupts
            disableInterrupts();
        }
    }
    if (ret)
    {
        // Get data
        uint8_t* buff = data;
        uint32_t left = size;
        read_size = 0;
        while ((left != 0) && (m_rx_queue.pop(*buff)))
        {
            left--;
            buff++;
            read_size++;
        }
        if (m_rx_queue.getCount() == 0u)
        {
            m_status_flags.reset(USB_CDC_RX_READY_FLAG_MASK);
        }
    }

    // Enable interrupts
    enableInterrupts();

    return ret;
}


/** \brief Retrieve the unique instance */
Stm32l476UsbDeviceCdc& Stm32l476UsbDeviceCdc::getInstance()
{
    return static_cast<Stm32l476UsbDeviceCdc&>(Stm32l476UsbDevice::getInstance());
}

/** \brief Initializes the CDC media low layer */
int8_t Stm32l476UsbDeviceCdc::ifaceInit(void)
{
    Stm32l476UsbDeviceCdc& usbd_cdc = Stm32l476UsbDeviceCdc::getInstance();
    USBD_HandleTypeDef* usbd_handle = reinterpret_cast<USBD_HandleTypeDef*>(usbd_cdc.getDevice());

    // Set Application Buffers
    USBD_CDC_SetTxBuffer(usbd_handle, usbd_cdc.m_ll_tx_buffer, 0);
    USBD_CDC_SetRxBuffer(usbd_handle, usbd_cdc.m_ll_rx_buffer);

    // Clear receive queue
    usbd_cdc.m_rx_queue.clear();

    // Reset Tx andRx flags
    usbd_cdc.m_status_flags.reset(0xFFFFFFFFu);
    usbd_cdc.m_status_flags.set(USB_CDC_TX_READY_FLAG_MASK, true);

    // Notify listener
    if (usbd_cdc.m_listener != nullptr)
    {
        usbd_cdc.m_listener->onUsbPlugged();
    }

    return (USBD_OK);
}

/** \brief DeInitializes the CDC media low layer */
int8_t Stm32l476UsbDeviceCdc::ifaceDeInit(void)
{
    Stm32l476UsbDeviceCdc& usbd_cdc = Stm32l476UsbDeviceCdc::getInstance();

    // Notify listener
    if (usbd_cdc.m_listener != nullptr)
    {
        usbd_cdc.m_listener->onUsbUnplugged();
    }

    return (USBD_OK);
}

 /** \brief Manage the CDC class requests */
int8_t Stm32l476UsbDeviceCdc::ifaceControl(uint8_t cmd, uint8_t* buff, uint16_t length)
{ 
    (void)cmd;
    (void)buff;
    (void)length;
    return (USBD_OK);
}

 /** \brief Data received over USB OUT endpoint are sent over CDC interface through this function */
int8_t Stm32l476UsbDeviceCdc::ifaceReceive(uint8_t* buff, uint32_t* length)
{
    Stm32l476UsbDeviceCdc& usbd_cdc = Stm32l476UsbDeviceCdc::getInstance();
    
    // Push data into receive queue
    uint32_t left = (*length);
    while ((left != 0u) && (usbd_cdc.m_rx_queue.push(*buff)))
    {
        buff++;
        left--;
    }

    // Initiate next USB packet transfer
    USBD_CDC_ReceivePacket(reinterpret_cast<USBD_HandleTypeDef*>(usbd_cdc.getDevice()));

    // Signal waiting tasks that data is available
    if (usbd_cdc.m_rx_queue.getCount() != 0u)
    {
        usbd_cdc.m_status_flags.set(USB_CDC_RX_READY_FLAG_MASK, true);
    }

    return (USBD_OK);
}

/** \brief Data has been transmitted over USB IN endpoint */
int8_t Stm32l476UsbDeviceCdc::ifaceTransmitDone()
{
    Stm32l476UsbDeviceCdc& usbd_cdc = Stm32l476UsbDeviceCdc::getInstance();

    // Signal waiting tasks that transmit is ready
    usbd_cdc.m_status_flags.set(USB_CDC_TX_READY_FLAG_MASK, true);

    return (USBD_OK);
}


}
