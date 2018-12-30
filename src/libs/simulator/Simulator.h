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

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "ISimulator.h"
#include "ISimuDevice.h"
#include "IAltimeter.h"
#include "IBarometer.h"
#include "IThermometer.h"
#include "IVariometer.h"
#include "ConfigManager.h"
#include "ConfigValueGroup.h"
#include "ConfigValue.h"
#include "TaskHelper.h"
#include "UdpSocket.h"
#include "StaticVector.h"

namespace open_vario
{


/** \brief Simulator */
class Simulator : public ISimulator, public ITaskStart, public IAltimeterListener, public IBarometerListener, public IThermometerListener, public IVariometerListener
{
    public:

        /** \brief Constructor */
        Simulator(ConfigManager& config_manager);


        /** \brief Configure the simulator */
        bool configure();

        /** \brief Start the simulator */
        bool start();

        /** \brief Register a simulated device */
        virtual bool registerSimuDevice(ISimuDevice& simu_device);


        /** \brief Method which will be called at the task's startup */
        virtual void taskStart(void* const param);


        /** \brief Called when new altimeter values have been computed */
        virtual void onAltimeterValues(const AltimeterValues& alti_values);

        /** \brief Called when new barometer values have been computed */
        virtual void onBarometerValues(const BarometerValues& baro_values);

        /** \brief Called when new thermometer values have been computed */
        virtual void onThermometerValues(const ThermometerValues& temp_values);

        /** \brief Called when new variometer values have been computed */
        virtual void onVariometerValues(const VariometerValues& vario_values);


    private:


        /** \brief Configuration manager */
        ConfigManager& m_config_manager;


        /** \brief Configuration values */
        ConfigValueGroup<3u> m_config_values;
        /** \brief Configuration value : simulator enable flag */
        ConfigValue<bool> m_config_simu_enabled;
        /** \brief Configuration value : simulator ip address */
        StringConfigValue<IpEndPoint::IP_ADDRESS_MAX_SIZE> m_config_simu_ip_address;
        /** \brief Configuration value : simulator port */
        ConfigValue<uint16_t> m_config_simu_port;


        /** \brief Udp socket */
        UdpSocket m_socket;

        /** \brief Simulator task */
        TaskHelper<1u> m_task;

        /** \brief Notification end point */
        IpEndPoint m_notification_endpoint;

        /** \brief Indicate if the simulator is connected */
        bool m_connected;

        /** \brief List of simulated devices */
        nano_stl::StaticVector<ISimuDevice*, 5u> m_simulated_devices;
      


        /** \brief Response frame*/
        static const uint8_t RESPONSE_FRAME = 0x52u;

        /** \brief Notification frame*/
        static const uint8_t NOTIFICATION_FRAME = 0x4Eu;
};

}

#endif // SIMULATOR_H
