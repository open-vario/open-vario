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

        /** \brief Called when an attribute as been modified by a client */
        virtual void attributeModified(const uint16_t handle, const uint8_t size, const uint8_t* const data) = 0;
};


/** \brief ST BlueNRG-MS bluetooth module */
class BlueNrgMs : public IInterruptPinListener
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

        /** \brief Attribute security permission */
        enum SecurityPermission
        {
            /** \brief None */
            SP_NONE = 0u,
            /** \brief Authentication required */
            SP_AUTHE_REQUIRED = 1u,
            /** \brief Authorization required */
            SP_AUTHO_REQUIRED = 2u,
            /** \brief Encryption required */
            SP_ENC_REQUIRED = 3u
        };

        /** \brief Attribute access permission */
        enum AccessPermission
        {
            /** \brief None */
            AP_NONE = 0u,
            /** \brief Read */
            AP_READ = 1u,
            /** \brief Write */
            AP_WRITE = 2u,
            /** \brief Read/Write */
            AP_READ_WRITE = 3u
        };


        /** \brief Constructor */
        BlueNrgMs(ISpi& spi, const uint8_t chip_select, IOutputPin& reset_pin, IInterruptPin& it_pin, ITask& rx_task);



        /** \brief Check if the module is present */
        bool probe();

        /** \brief Set the module hardware configuration (must be called before any method other than probe()) */
        void setHwConfig(const HwConfig& config) { m_config = config; }

        /** \brief Set the device name */
        void setDeviceName(const char* const name);

        /** \brief Set the module's listener */
        bool setListener(IBlueNrgMsListener& listener) { m_listener = &listener; return true; }

        /** \brief Configure the module */
        bool configure();

        /** \brief Get the module version */
        bool getVersion(uint8_t& hw_version, uint8_t& fw_version, uint8_t& fw_subversion);

        /** \brief Set the module discoverable */
        bool setDiscoverable();

        /** \brief Add a BLE service */
        bool addBleService(const uint8_t* uuid, const size_t uuid_size, const bool primary_service, const uint8_t max_attr_count, uint16_t& service_handle);

        /** \brief Include a BLE service into another BLE service */
        bool includeBleService(const uint16_t root_service_handle, const uint16_t service_handle, const uint8_t* uuid, const size_t uuid_size, uint16_t& included_handle);

        /** \brief Add a BLE characteristic */
        bool addBleCharacteristic(const uint16_t service_handle, const uint8_t* uuid, const size_t uuid_size, const uint8_t max_length, const bool is_fixed_length, const uint8_t properties, const SecurityPermission security_perms, uint16_t& char_handle);

        /** \brief Add a BLE characteristic descriptor */
        bool addBleCharacteristicDescriptor(const uint16_t service_handle, const uint16_t char_handle, const uint8_t* uuid, const size_t uuid_size, const void* value, const uint8_t length, const AccessPermission access_perms, const SecurityPermission security_perms, uint16_t& char_desc_handle);

        /** \brief Update the value of a BLE characteristic */
        bool updateBleCharacteristicValue(const uint16_t service_handle, const uint16_t char_handle, const void* value, const uint8_t length);


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

        /** \brief Device name */
        char m_device_name[32u];

        /** \brief GAP service handle */
        uint16_t m_gap_service_handle;

        /** \brief GAP device name characteristic handle */
        uint16_t m_gap_device_name_handle;

        /** \brief Buffer for commands */
        union
        {
            /** \brief Hal_Write_Config_Data command */
            struct HalWriteConfigData
            {
                uint8_t  offset;
                uint8_t  length;
                union
                {   
                    uint8_t public_address[6u];
                    uint16_t div;
                    uint8_t er[16u];
                    uint8_t it[16u];
                    uint8_t ll_without_host;
                    uint8_t role;
                } __attribute__((packed)) data;
            } __attribute__((packed)) hal_write_config_data;

            /** \brief Hal_Set_Tx_Power_Level command */
            struct HalSetTxPowerLevel
            {
                uint8_t high_power_enabled;
                uint8_t pa_level;
            } __attribute__((packed)) hal_set_tx_power_level;

            /** \brief Gap_Init command */
            struct GapInit
            {
                uint8_t role;
                uint8_t privacy;
                uint8_t device_name_length;
            } __attribute__((packed)) gap_init;

            /** \brief Gap_Set_IO_Capability command */
            struct GapSetIoCapability
            {
                uint8_t io_capability;
            } __attribute__((packed)) gap_set_io_capability;

            /** \brief Gap_Set_Auth_Requirement command */
            struct GapSetAuthRequirement
            {
                uint8_t mitm_mode;
                uint8_t oob_enable;
                uint8_t oob_data[16u];
                uint8_t min_encryption_key_size;
                uint8_t max_encryption_key_size;
                uint8_t use_fixed_pin;
                uint32_t fixed_pin;
                uint8_t bounding_mode;
            } __attribute__((packed)) gap_set_auth_requirement;

            /** \brief Gatt_Include_Service command */
            struct GattIncludeService
            {
                uint16_t service_handle;
                uint16_t include_start_handle;
                uint16_t include_end_handle;
                uint8_t uuid_type;
                uint8_t uuid[16u];
            } __attribute__((packed)) gatt_include_service;

            /** \brief Gatt_Update_Char_Value command */
            struct GattUpdateCharValue
            {
                uint16_t service_handle;
                uint16_t char_handle;
                uint8_t val_offset;
                uint8_t char_value_length;
                uint8_t value[122u];
            } __attribute__((packed)) gatt_update_char_value;

            /** \brief Byte buffer */
            uint8_t buffer[128u];

        } m_cmd;

        /** \brief Buffer for responses */
        union
        {
            /** \brief Status */
            uint8_t status;

            /** \brief Read_Local_Version_Information response */
            struct ReadLocalVersionInformation
            {
                uint8_t  status;
                uint8_t  hci_version;
                uint16_t hci_revision;
                uint8_t  lmp_pal_version;
                uint16_t manufacturer_name;
                uint16_t lmp_pal_subversion;
            } __attribute__((packed)) read_local_version_information;

            /** \brief Gap_Init response */
            struct GapInit
            {
                uint8_t status;
                uint16_t service_handle;
                uint16_t device_name_handle;
                uint16_t appearance_handle;
            } __attribute__((packed)) gap_init;

            /** \brief Gatt_Add_Serv response */
            struct GattAddServ
            {
                uint8_t status;
                uint16_t service_handle;
            } __attribute__((packed)) gatt_add_serv;

            /** \brief Gatt_Include_Service response */
            struct GattIncludeService
            {
                uint8_t status;
                uint16_t included_handle;
            } __attribute__((packed)) gatt_include_service;

            /** \brief Gatt_Add_Char response */
            struct GattAddChar
            {
                uint8_t status;
                uint16_t characteristic_handle;
            } __attribute__((packed)) gatt_add_char;

            /** \brief Gatt_Add_Char_Desc response */
            struct GattAddCharDesc
            {
                uint8_t status;
                uint16_t char_desc_handle;
            } __attribute__((packed)) gatt_add_char_desc;

        } m_resp;



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


        
        /** \brief Receive task method */
        void receiveTask(void* const param);

        /** \brief Send a HCI command to the module and wait for its response */
        bool hciSendReceive(const uint16_t opcode, const void* params, const uint8_t size, void* resp_params, uint8_t resp_size);

        /** \brief Send a HCI command to the module */
        bool hciSendCommand(const uint16_t opcode, const void* params, const uint8_t size);
};

}

#endif // BLUENRGMS_H
