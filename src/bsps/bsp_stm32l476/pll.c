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

#include "stm32l476xx.h"

/** \brief Initialize CPU speed using PLL */
void pll_init()
{
    uint32_t reg;

    /* Reset the RCC clock configuration to the default reset state ------------*/
    /* Set MSION bit */
    RCC->CR |= RCC_CR_MSION;

    /* Reset CFGR register */
    RCC->CFGR = 0x00000000U;

    /* Reset HSEON, CSSON , HSION, and PLLON bits */
    RCC->CR &= 0xEAF6FFFFU;

    /* Reset PLLCFGR register */
    RCC->PLLCFGR = 0x00001000U;

    /* Reset HSEBYP bit */
    RCC->CR &= 0xFFFBFFFFU;

    /* Disable all interrupts */
    RCC->CIER = 0x00000000U;

    /* Configure the Vector Table location add offset address ------------------*/
    SCB->VTOR = FLASH_BASE; /* Vector Table Relocation in Internal FLASH */


    /* Configure flash latency */
    FLASH->ACR = (3u << 8u) | (2u << 0u);

    /* Start external oscillator (LSE) */
    RCC->APB1ENR1 |= (1u << 28u);
    PWR->CR1 |= (1u << 8u);
    RCC->BDCR |= (1u << 0u);
    while ((RCC->BDCR & (1u << 1u)) == 0u)
    {}

    /* Configure MSI to run at 48MHz with hardware autocalibration using LSE (PLL mode) */
    while ((RCC->CR & (1u << 1u)) == 0u)
    {}
    reg = RCC->CR;
    reg &= ~(0x0F << 4u);
    RCC->CR = reg | (11u << 4u) | (1u << 3u);
    while ((RCC->CR & (1u << 1u)) == 0u)
    {}
    RCC->CR |= (1u << 2u);
    while ((RCC->CR & (1u << 1u)) == 0u)
    {}

    /* Select MSI as SYSCLK */
    RCC->CFGR &= ~(3u << 0u);
    while ((RCC->CFGR & (3u << 2u)) != 0u)
    {}

    /* Configure peripheral clocks

         AHBPresc = 1, APB2Presc = 4, APB1Presc = 4
         => CPU clock = 48MHz, APB2 periph = 12MHz, APB1 periph = 12MHz
     */
    RCC->CFGR = (5u << 11u) | (5u << 8u) | (0u << 4u) | (0u << 0u);

    /* Select MSI as source clock for USB */
    RCC->CCIPR |= (3u << 26u);
}
