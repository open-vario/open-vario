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


#include "OpenVario10Board.h"

using namespace open_vario;



/** \brief Initialize and start the system tick */
static void SYSTICK_Init(void);
/** \brief Retrieve the current systick counter value */
static uint32_t SYSTICK_GetCounter(void);


static OpenVario10Board open_vario_board;


int main(void)
{
	ICpu& cpu = open_vario_board.cpu();
	ILed& activity_led = open_vario_board.activityLed();
	IUart& debug_uart = open_vario_board.debugUart();

	open_vario_board.configure();
	
	SYSTICK_Init();

	bool led_state = false;
	uint32_t counter_value = 0u;
	activity_led.setOn();
	while (true)
	{
		if (SYSTICK_GetCounter() != counter_value)
		{
			counter_value = SYSTICK_GetCounter();
			if ((counter_value % 500) == 0u)
			{
				led_state = !led_state;
				activity_led.setState(led_state);
				if (led_state)
				{
					debug_uart.write(reinterpret_cast<const uint8_t*>("ON!\r\n"), 5u);
				}
				else
				{
					debug_uart.write(reinterpret_cast<const uint8_t*>("OFF!\r\n"), 6u);
				}
			}
		}
	}

	cpu.reset();

	return 0;
}




#include "stm32l476xx.h"

/** \brief Systick counter */
static volatile uint32_t s_sytick_counter;


/** \brief Initialize and start the system tick */
static void SYSTICK_Init(void)
{
    /* Configure and start systick */
    static const uint32_t systick_input_freq_hz = 48000000u; /* 48 MHz */
    static const uint32_t systick_rate_hz = 1000u; /* 1 kHz = each millisecond */
    SysTick->LOAD = (systick_input_freq_hz / systick_rate_hz) - 1u;
    SysTick->CTRL = 0x07u;
    s_sytick_counter = 0u;
}

/** \brief Retrieve the current systick counter value */
static uint32_t SYSTICK_GetCounter(void)
{
    return s_sytick_counter;
}

/** \brief Systick interrupt handler */
extern "C" void SysTick_Handler(void)
{
    s_sytick_counter++;
}
