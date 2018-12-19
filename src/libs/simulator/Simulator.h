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

#include "ConfigManager.h"
#include "ConfigValueGroup.h"
#include "ConfigValue.h"
#include "TaskHelper.h"
#include "UdpSocket.h"

namespace open_vario
{


/** \brief Simulator */
class Simulator : public ITaskStart
{
    public:

        /** \brief Constructor */
        Simulator(ConfigManager& config_manager);


        /** \brief Configure the simulator */
        bool configure();

        /** \brief Start the simulator */
        bool start();


        /** \brief Method which will be called at the task's startup */
        virtual void taskStart(void* const param);

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
      
};

}

#endif // SIMULATOR_H
