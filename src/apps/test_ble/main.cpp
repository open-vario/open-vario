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

#define APP

#ifdef APP

#include "Os.h"
#include "NucleoBoard.h"
#include "TaskHelper.h"
#include "NanoOsConsole.h"
#include "EepromTest.h"
#include "BleTest.h"

using namespace open_vario;


/** \brief Test application */
class TestApp : public ITaskStart
{
	public:

		/** \brief Constructor */
		TestApp()
		: m_os()
		, m_board()
		, m_nano_os_console(m_board.debugUart())
		, m_led_task("Led task", 4u)
		, m_eeprom_test(m_board.eeprom())
		, m_ble_test(m_board.bleModule())
		{}

		/** \brief Start test application */
		void start()
		{
			// Initialize operating system
			if (m_os.init())
			{
				// Start main task
				m_nano_os_console.init();
				m_led_task.start(*this, nullptr);					

				// Start operating system
				m_os.start();
			}
		}

		/** \brief Method which will be called at the task's startup */
        virtual void taskStart(void* const param)
		{
			(void) param;

			// Initialize board
			uint32_t led_period = 250u;
			if (m_board.configure())
			{
				// Start tasks
				if (m_board.bleModule().probe())
				{
					m_ble_test.start();
				}
			}
			else
			{
				led_period = 1000u;
			}
			//m_eeprom_test.start();

			// Task loop
			bool state = true;
			ILed& led = m_board.activityLed();
			while (true)
			{
				led.setState(state);
				m_os.waitMs(led_period);
				state = !state;
			}
		}

	private:

		/** \brief Operating system */
        Os m_os;

		/** \brief Board */
		NucleoBoard m_board;

		/** \brief Nano OS console handler */
		NanoOsConsole m_nano_os_console;

		/** \brief Led task */
		TaskHelper<1024u> m_led_task;

		/** \brief Eeprom test task */
		EepromTest m_eeprom_test;

		/** \brief BLE test task */
		BleTest m_ble_test;
};


/** \brief Test application */
static TestApp test_app;

/** \brief Application's entry point */
int main()
{
	int ret = 0;

	// Start test application
	test_app.start();
	
	return ret;
}

#else

#include "stm32l476xx.h"

#include "Stm32l476Gpio.h"

using namespace open_vario;

Stm32l476Gpio spi_1_cs0_pin(Stm32l476Gpio::PORT_A, 1u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH);
Stm32l476Gpio ble_reset_pin(Stm32l476Gpio::PORT_A, 8u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_MEDIUM);
Stm32l476Gpio ble_irq_pin(Stm32l476Gpio::PORT_A, 0u, Stm32l476Gpio::MODE_INPUT, 0u, Stm32l476Gpio::IT_RAISING, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_MEDIUM);

int main()
{
	spi_1_cs0_pin.setLow();
	ble_reset_pin.setLow();

	ble_irq_pin.configure();
	spi_1_cs0_pin.configure();
	ble_reset_pin.configure();

	spi_1_cs0_pin.setHigh();
	ble_reset_pin.setHigh();

	return 0;
}


#endif // APP