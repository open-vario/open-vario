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

#ifndef BLEMANAGER_H
#define BLEMANAGER_H

#include "IBlePeripheralStack.h"
#include "ConfigManager.h"
#include "MinMaxConfigValue.h"
#include "TaskHelper.h"
#include "Timer.h"
#include "FlagSet.h"
#include "StaticVector.h"
#include "IdentificationService.h"
#include "FlightDataService.h"


namespace open_vario
{


/** \brief Bluetooth Low Energy manager */
class BleManager : public IBlePeripheralStackListener, public ITimerListener
{
    public:

        /** \brief Constructor */
        BleManager(ConfigManager& config_manager, IBlePeripheralStack& ble_stack, DeviceManager& device_manager);


        /** \brief Initialize the BLE manager */
        bool init();

        /** \brief Start the BLE manager */
        bool start();


        /** \brief Method which will be called when the timer elapsed */
        virtual void timerElapsed(ITimer& timer);

        /** \brief Called when a client device is connected to the stack */
        virtual void bleClientConnected();

        /** \brief Called when a client device is connected to the stack */
        virtual void bleClientDisconnected();
        

    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief BLE stack */
        IBlePeripheralStack& m_ble_stack;


        /** \brief Configuration values */
        ConfigValueGroup<3u> m_config_values;
        /** \brief Configuration value : BLE enabled flag */
        ConfigValue<bool> m_config_enabled;
        /** \brief Configuration value : BLE hardware address */
        ArrayConfigValue<uint8_t, IBlePeripheralStack::HW_ADDRESS_SIZE> m_config_hw_address;
        /** \brief Configuration value : BLE update period */
        MinMaxConfigValue<uint16_t> m_config_update_period;


        /** \brief Indicate if the BLE is enabled */
        bool m_enabled;

        /** \brief Update period in milliseconds */
        uint16_t m_update_period;

        /** \brief Update task */
        TaskHelper<2048u> m_task;

        /** \brief Timer to start periodic update */
        Timer m_update_timer;

        /** \brief Flagset to start update task */
        FlagSet m_update_flag_set;

        /** \brief Flag to start periodic update */
        static const uint32_t FLAG_PERIODIC_UPDATE = 1u;

        /** \brief Flag indicating a client device connection */
        static const uint32_t FLAG_CLIENT_CONNECTED = 2u;

        /** \brief Flag indicating a client device disconnection */
        static const uint32_t FLAG_CLIENT_DISCONNECTED = 4u;




        /** \brief BLE services */
        nano_stl::StaticVector<IBleService*, 10u> m_ble_services;


        /** \brief Identification service */
        IdentificationService m_identification_service;

        /** \brief Flight data service */
        FlightDataService m_flight_data_service;



        /** \brief BLE manager's task method */
        void task(void* const param);

};

}

#endif // BLEMANAGER_H
