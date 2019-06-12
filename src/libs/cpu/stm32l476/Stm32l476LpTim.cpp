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

#include "Stm32l476LpTim.h"
#include "Stm32l476Cpu.h"
#include "IOs.h"

#ifdef OS_NANO_OS
#include "nano_os_api.h"
#endif // OS_NANO_OS

#include "stm32l476xx.h"


namespace open_vario
{




/** \brief Register definitions for the LPTIMs */
static LPTIM_TypeDef* const lptim_regs[Stm32l476LpTim::LPTIM_MAX] = {LPTIM1, LPTIM2};

/** \brief LPTIM irq numbers */
static const IRQn_Type lptim_irqs[Stm32l476LpTim::LPTIM_MAX] = {LPTIM1_IRQn, LPTIM2_IRQn};

/** \brief LPTIM instances */
static Stm32l476LpTim* lptims[Stm32l476LpTim::LPTIM_MAX] = {nullptr, nullptr};





/** \brief Constructor */
Stm32l476LpTim::Stm32l476LpTim(const Stm32l476Cpu& cpu, const LpTim lptim)
: m_cpu(cpu)
, m_lptim(lptim)
, m_frequency(0u)
, m_started(false)
{
    // Register instance
    lptims[lptim] = this;
}

/** \brief Configure the timer */
bool Stm32l476LpTim::configureTimer(const uint32_t frequency)
{
    bool ret = true;

    // Enable clock and configure to use PCLK as input clock for frequency > LSE and LSE for slower frequencies */
    uint32_t input_clock_freq = m_cpu.getLseFrequency();
    if ((frequency * 1000u) > input_clock_freq)
    {
        input_clock_freq = m_cpu.getPclk1Frequency();
    }
    switch (m_lptim)
    {
        case LPTIM_1:
        {
            RCC->APB1ENR1 |= (1u << 31u);
            break;
        }

        case LPTIM_2:
        {
            RCC->APB1ENR2 |= (1u << 5u);
            break;
        }

        default:
        {
            // Invalid LPTIM
            ret = false;
            break;
        }
    }
    if (ret)
    {
        RCC->CCIPR &= ~(3u << (18u + 2u * static_cast<uint8_t>(m_lptim)));
        if (input_clock_freq == m_cpu.getLseFrequency())
        {
            RCC->CCIPR |= (3u << (18u + 2u * static_cast<uint8_t>(m_lptim)));
        }
    }
    if (ret)
    {
        // Get LPTIM registers
        LPTIM_TypeDef* lptim = lptim_regs[m_lptim];

        // Disable timer
        lptim->CR = 0;

        // Compute prescaler value and reload value
        uint8_t prescaler_value = 0u;
        uint32_t post_prescaler_freq = input_clock_freq;
        while (post_prescaler_freq >= (frequency * 65536u))
        {
            prescaler_value++;
            post_prescaler_freq /= 2u;
        }
        uint32_t reload_value = post_prescaler_freq / frequency;

        // Configure timer : preload enabled, software trigger, internal clock source
        lptim->CFGR = (1u << 22u) | (prescaler_value << 9u);

        // Disable all interrupts
        lptim->IER = 0;

        // Acknowledge interrupts
        lptim->ICR = 0xFFu;

        // Enable NVIC interrupt
        NVIC_EnableIRQ(lptim_irqs[m_lptim]);

        // Enable timer
        lptim->CR = (1u << 0);

        // Configure reload and comp values
        lptim->ARR = reload_value;
        lptim->CMP = 0;
    }

    return ret;
}

/** \brief Start the timer */
bool Stm32l476LpTim::startTimer()
{
    bool ret = false;
    LPTIM_TypeDef* lptim = lptim_regs[m_lptim];

    // Check if timer is already started
    if (!m_started)
    {
        // Start timer
        lptim->CR |= (1u << 2u);

        m_started = true;
        ret = true;
    }

    return ret;
}

/** \brief Stop the timer */
bool Stm32l476LpTim::stopTimer()
{
    bool ret = false;
    LPTIM_TypeDef* lptim = lptim_regs[m_lptim];

    // Check if timer is already started
    if (m_started)
    {
        // Stop timer
        lptim->CR = 0;
        lptim->CR = (1u << 0);

        m_started = false;
        ret = true;
    }

    return ret;
}

/** \brief Get the pointer to LPTIM registers */
void* Stm32l476LpTim::getLpTimRegisters()
{
    return lptim_regs[m_lptim];
}


/** \brief IRQ handler */
void Stm32l476LpTim::irqHandler()
{
    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Enter();
    #endif // OS_NANO_OS

    // Call child specific interrupt handler
    onInterrupt();

    // Acknowledge interrupt
    LPTIM_TypeDef* lptim = lptim_regs[m_lptim];
    lptim->ICR = 0xFFu;

    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Exit();
    #endif // OS_NANO_OS
}


/** \brief IRQ handler for LPTIM 1 */
extern "C" void LPTIM1_IRQHandler(void)
{
    lptims[0]->irqHandler();
}

/** \brief IRQ handler for LPTIM 2 */
extern "C" void LPTIM2_IRQHandler(void)
{
    lptims[1]->irqHandler();
}


}
