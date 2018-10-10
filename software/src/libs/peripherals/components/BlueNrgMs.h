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

#ifndef BLUENRGMS_H
#define BLUENRGMS_H

#include "FlagSet.h"
#include "IOutputPin.h"
#include "IInterruptPin.h"
#include "ITask.h"
#include "Mutex.h"

namespace open_vario
{

class ISpi;


/** \brief Interface for all BlueNRG-MS bluetooth module listeners implementations */
class IBlueNrgMsListener
{
    public:

        /** \brief Called when the module is connected to another device */
        virtual void bleConnected() = 0;

        /** \brief Called when the module is disconnected from another device */
        virtual void bleDisconnected() = 0;
};


/** \brief ST BlueNRG-MS bluetooth module */
class BlueNrgMs : public IInterruptPinListener, public ITaskStart
{
    public:

        /** \brief BlueNrg-MS hardware module configuration */
        struct HwConfig
        {
            /** \brief Bluetooth public address */
            uint8_t ble_public_address[6u];
            /** \brief DIV used to derive CSRK */
            uint16_t div;
            /** \brief Encryption root key used to derive LTK and CSRK */
            uint8_t er[16u];
            /** \brief Identity root key used to derive LTK and CSRK */
            uint8_t ir[16u];
        };


        /** \brief Constructor */
        BlueNrgMs(ISpi& spi, const uint8_t chip_select, IOutputPin& reset_pin, IInterruptPin& it_pin, ITask& rx_task);


        /** \brief Check if the module is present */
        bool probe();

        /** \brief Set the module hardware configuration (must be called before any method other than probe()) */
        void setHwConfig(const HwConfig& config) { m_config = config; }

        /** \brief Set the module's listener */
        bool setListener(IBlueNrgMsListener& listener) { m_listener = &listener; return true; }

        /** \brief Configure the module */
        bool configure();

        /** \brief Get the module version */
        bool getVersion(uint8_t& hw_version, uint8_t& fw_version, uint8_t& fw_subversion);

        /** \brief Set the module discoverable */
        bool setDiscoverable(const char* local_name);



        /** \brief Method which will be called at the task's startup */
        virtual void taskStart(void* const param);

        /** \brief Called when the interrupt on the pin is triggered */
        virtual void onPinInterrupt(const IIoPin::Level level);


    private:

        /** \brief SPI bus */
        ISpi& m_spi;

        /** \brief Chip select */
        const uint8_t m_chip_select;

        /** \brief Reset pin */
        IOutputPin& m_reset_pin;

        /** \brief Interrupt pin */
        IInterruptPin& m_it_pin;

        /** \brief Rx task */
        ITask& m_rx_task;

        /** \brief Flag set to synchronize with interrupt pin when data is ready in the module */
        FlagSet m_data_ready_flag;

        /** \brief Mutex */
        Mutex m_mutex;

        /** \brief Expected response opcode */
        uint16_t m_resp_opcode;

        /** \brief Buffer to store the expected response */
        void* m_resp_buffer;

        /** \brief Size of the buffer to store the expected response */
        uint8_t m_resp_buffer_size;

        /** \brief BlueNrg-MS module hardware configuration */
        HwConfig m_config;

        /** \brief Module's listener */
        IBlueNrgMsListener* m_listener;



        /** \brief Flag value when data is ready in the module */
        static const uint32_t DATA_READY_FLAG = 1u;

        /** \brief Flag value when response is available from the module */
        static const uint32_t RESP_READY_FLAG = 2u;


        /** \brief HCI polling timeout in milliseconds */
        static const uint32_t HCI_POLLING_TIMEOUT = 50u;

        /** \brief Packet type : HCI command */
        static const uint32_t PKT_TYPE_HCI_CMD = 0x01u;

        /** \brief Packet type : HCI event */
        static const uint32_t PKT_TYPE_HCI_EVT = 0x04u;

        /** \brief Packet type : HCI vendor */
        static const uint32_t PKT_TYPE_HCI_VENDOR = 0xFFu;



        /** \brief Send a HCI command to the module and wait for its response */
        bool hciSendReceive(const uint16_t opcode, const void* params, const uint8_t size, void* resp_params, uint8_t resp_size);

        /** \brief Send a HCI command to the module */
        bool hciSendCommand(const uint16_t opcode, const void* params, const uint8_t size);
};

}

#endif // BLUENRGMS_H
