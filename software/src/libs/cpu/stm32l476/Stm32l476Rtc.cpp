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

#include "Stm32l476Rtc.h"

#include "stm32l476xx.h"

namespace open_vario
{


/** \brief Constructor */
Stm32l476Rtc::Stm32l476Rtc()
{}

/** \brief Configure the RTC */
bool Stm32l476Rtc::configure()
{

    /* Enable RTC clock in normal and low power modes, clock source = LSE */
    RCC->APB1ENR1 |= (1u << 10u);
    RCC->APB1SMENR1 |= (1u << 10u);
    uint32_t reg = RCC->BDCR;
    reg &= ~(3u << 8u); 
    RCC->BDCR = reg | ((1u << 15u) | (1u << 8u));

    /* Allow write access to RTC peripheral */
    PWR->CR1 |= (1u << 8u);

    /* Check if RTC is already initialized */
    if ((RTC->ISR & (1u << 4u)) == 0u)
    {
        /* Allow write access to RTC registers */
        RTC->WPR = 0xCAu;
        RTC->WPR = 0x53u;

        /* Enter init mode */
        RTC->ISR |= (1u << 7u);
        while ((RTC->ISR & (1u << 6u)) == 0u)
        {}

        /* Configure prescaler to obtain a 1Hz clock from the LSE clock(32.768kHz) 
            
        RTC_clock = LSE / ((PREDIV_S + 1) * (PREDIV_A + 1))

        With PREDIV_A = 127 and PREDIV_S = 255
        => RTC_clock = 1Hz

        */
        RTC->PRER = (127u << 16u) | (255u << 0u);

        /* 24h mode, no alarms nor interrupts */
        RTC->CR = 0u;

        /* Exit init mode */
        RTC->ISR &= ~(1u << 7u);

        /* Synchronize shadow registers */
        syncShadowRegs();

        /* Disable write access to RTC registers */
        RTC->WPR = 0x00u;
    }

    return true;
}

/** \brief Set the date and time in the RTC */
bool Stm32l476Rtc::setDateTime(const DateTime& date_time)
{
    /* Allow write access to RTC registers */
    RTC->WPR = 0xCAu;
    RTC->WPR = 0x53u;

    /* Enter init mode */
    RTC->ISR |= (1u << 7u);
    while ((RTC->ISR & (1u << 6u)) == 0u)
    {}

    /* Configure date */
    RTC->DR = (toBcd(date_time.year) << 16u) |
              /*(1u << 16u) |*/ /* Day of the week => not used */
              (toBcd(date_time.month) << 8u) |
              (toBcd(date_time.day) << 0u);

    /* Configure time (24h format) */
    RTC->TR = (toBcd(date_time.hour) << 16u) |
              (toBcd(date_time.minute) << 8u) |
              (toBcd(date_time.second) << 0u);

    /* Exit init mode */
    RTC->ISR &= ~(1u << 7u);

    /* Synchronize shadow registers */
    syncShadowRegs();

    /* Disable write access to RTC registers */
    RTC->WPR = 0x00u;

    return true;
}

/** \brief Get the date and time in the RTC */
bool Stm32l476Rtc::getDateTime(DateTime& date_time)
{
    /* Read date and time registers */
    const uint32_t time_reg = RTC->TR;
    const uint32_t date_reg = RTC->DR;

    /* Extract date and time */
    date_time.year = fromBcd((date_reg >> 16u) & 0xFFu);
    date_time.month = fromBcd((date_reg >> 8u) & 0xFFu);
    date_time.day = fromBcd((date_reg >> 0u) & 0xFFu);

    date_time.hour = fromBcd((time_reg >> 16u) & 0xFFu);
    date_time.minute = fromBcd((time_reg >> 8u) & 0xFFu);
    date_time.second = fromBcd((time_reg >> 0u) & 0xFFu);

    return true;
}


/** \brief Synchronize shadow registers */
void Stm32l476Rtc::syncShadowRegs()
{
    /* Clear RSF bit */
    RTC->ISR &= ~(1u << 5u);

    /* Wait RSF bit to be set again by hardware */
    while ((RTC->ISR & (1u << 5u)) == 0)
    {}
}

/** \brief Convert a value to BCD format */
uint8_t Stm32l476Rtc::toBcd(const uint8_t value)
{
    const uint8_t bcd_value = ((value / 10u) << 4u) + (value % 10u);
    return bcd_value;
}

/** \brief Convert a value from BCD format */
uint8_t Stm32l476Rtc::fromBcd(const uint8_t bcd_value)
{
    const uint8_t value = (bcd_value & 0x0Fu) + ((bcd_value >> 4u) * 10u);
    return value;
}


}

