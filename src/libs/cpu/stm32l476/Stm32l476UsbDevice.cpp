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

#include "Stm32l476UsbDevice.h"
#include "Stm32l476Cpu.h"
#include "IOs.h"

#ifdef OS_NANO_OS
#include "nano_os_api.h"
#endif // OS_NANO_OS

#include "stm32l476xx.h"
#include "usbd_core.h"

/** \brief Low level USB layer data */
extern PCD_HandleTypeDef hpcd;

namespace open_vario
{


/** \brief USB device instance */
static Stm32l476UsbDevice* usbd = nullptr;


/** \brief Constructor */
Stm32l476UsbDevice::Stm32l476UsbDevice(const Stm32l476Cpu& cpu)
: m_cpu(cpu)
, m_usbd(nullptr)
{
    static USBD_HandleTypeDef usbd_handle;

    // Register instance
    usbd = this;
    m_usbd = &usbd_handle;
}

/** \brief Configure the USB */
bool Stm32l476UsbDevice::configureUSB(void* descriptors, void* device_class)
{
    bool ret;

    // Enable PWR module
    RCC->APB1ENR1 |= (1u << 28u);
    
    // Enable VddUSB
    PWR->CR2 |= (1u << 10u);

    // Enable USB clock
    RCC->AHB2ENR |= (1u << 12u);

    // Enable interrupts
    enableInterrupts();

    // Initialize USB
    USBD_HandleTypeDef* usbd_handle = reinterpret_cast<USBD_HandleTypeDef*>(m_usbd);
    USBD_StatusTypeDef usb_status = USBD_Init(usbd_handle,
                                              reinterpret_cast<USBD_DescriptorsTypeDef*>(descriptors), 
                                              0);
    ret = (usb_status == USBD_OK);
    if (ret)
    {
        usb_status = USBD_RegisterClass(usbd_handle,
                                        reinterpret_cast<USBD_ClassTypeDef*>(device_class));
        ret = (usb_status == USBD_OK);
    }

    return ret;
}

/** \brief Start USB */
bool Stm32l476UsbDevice::startUSB()
{
    const USBD_StatusTypeDef usb_status = USBD_Start(reinterpret_cast<USBD_HandleTypeDef*>(m_usbd));
    return (usb_status == USBD_OK);
}

/** \brief Stop USB */
bool Stm32l476UsbDevice::stopUSB()
{
    const USBD_StatusTypeDef usb_status = USBD_Stop(reinterpret_cast<USBD_HandleTypeDef*>(m_usbd));
    return (usb_status == USBD_OK);
}

/** \brief Disable USB device interrupts */
void Stm32l476UsbDevice::disableInterrupts()
{
    NVIC_DisableIRQ(OTG_FS_IRQn);
}

/** \brief Enable USB device interrupts */
void Stm32l476UsbDevice::enableInterrupts()
{
    NVIC_EnableIRQ(OTG_FS_IRQn);
}

/** \brief Retrieve the unique instance */
Stm32l476UsbDevice& Stm32l476UsbDevice::getInstance()
{
    return (*usbd);
}

/** \brief IRQ handler */
void Stm32l476UsbDevice::irqHandler()
{
    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Enter();
    #endif // OS_NANO_OS

    // Call STM32 USB device library handler
    HAL_PCD_IRQHandler(&hpcd);

    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Exit();
    #endif // OS_NANO_OS
}


/** \brief IRQ handler for USB */
extern "C" void OTG_FS_IRQHandler(void)
{
    usbd->irqHandler();
}

/** \brief Provide a tick value for the USB stack */
extern "C" uint32_t HAL_GetTick(void)
{
    uint32_t tick_count = 0;

    #ifdef OS_NANO_OS
    NANO_OS_GetTickCount(&tick_count);
    #endif // OS_NANO_OS
    
    return tick_count;
}

}
