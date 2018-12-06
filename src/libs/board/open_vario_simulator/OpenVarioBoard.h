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

#ifndef OPENVARIOBOARD_H
#define OPENVARIOBOARD_H

#include "IOpenVarioBoard.h"

#ifdef OS_WINDOWS
#include "WindowsCpu.h"
#include "WindowsRtc.h"
#endif // OS_WINDOWS
#ifdef OS_LINUX
#include "LinuxCpu.h"
#include "LinuxRtc.h"
#endif // OS_LINUX

#include "McuRtc.h"
#include "SimuLed.h"
#include "SimuUart.h"

namespace open_vario
{



/** \brief Open vario simulated board */
class OpenVarioBoard : public IOpenVarioBoard
{
    public:

        /** \brief Constructor */
        OpenVarioBoard();


        /** \brief Configure the board peripherals */
        virtual bool configure();

        /** \brief Get the board's name */
        virtual const char* name() const { return "Open Vario 1.0"; }

        /** \brief Get the board's CPU */
        virtual ICpu& cpu() { return m_cpu; }

        /** \brief Get the board's RTC */
        virtual IRtc& rtc() { return m_rtc; }

        /** \brief Get the board's activity LED */
        virtual ILed& activityLed() { return m_activity_led; }

        /** \brief Get the board's debug UART */
        virtual IUart& debugUart() { return m_debug_uart; }


    private:

        #ifdef OS_WINDOWS
        /** \brief CPU */
        WindowsCpu m_cpu;

        /** \brief RTC */
        WindowsRtc m_rtc_driver;
        #endif // OS_WINDOWS
        #ifdef OS_LINUX
        /** \brief CPU */
        LinuxCpu m_cpu;

        /** \brief RTC */
        LinuxRtc m_rtc_driver;
        #endif // OS_LINUX

        /** \brief RTC peripheral */
        McuRtc m_rtc;

        /** \brief Activity LED */
        SimuLed m_activity_led;

        /** \brief Debug UART */
        SimuUart m_debug_uart;

};

}

#endif // OPENVARIOBOARD_H
