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

#include "BlueNrgMs.h"
#include "ISpi.h"
#include "IOs.h"
#include "nano-stl-conf.h"

#include <cstring>

namespace open_vario
{


/** \brief Constructor */
BlueNrgMs::BlueNrgMs(ISpi& spi, const uint8_t chip_select, IOutputPin& reset_pin, IInterruptPin& it_pin, ITask& rx_task)
: m_spi(spi)
, m_chip_select(chip_select)
, m_reset_pin(reset_pin)
, m_it_pin(it_pin)
, m_rx_task(rx_task)
, m_data_ready_flag(0u)
, m_mutex()
, m_resp_opcode(0u)
, m_resp_buffer(NULL)
, m_resp_buffer_size(0u)
, m_config()
, m_listener(NULL)
, m_device_name()
, m_gap_service_handle(0u)
, m_gap_device_name_handle(0u)
, m_cmd()
, m_resp()
{
    m_it_pin.registerListener(*this);
    setDeviceName("BlueNrgMs");
}

/** \brief Set the device name */
void BlueNrgMs::setDeviceName(const char* const name)
{
    NANO_STL_STRNCPY(m_device_name, name, sizeof(m_device_name));
}

/** \brief Configure the module */
bool BlueNrgMs::configure()
{
    bool ret = false;

    // Check if the module is present
    if (probe())
    {
        // Start the rx task
        ret = m_rx_task.start(*this, NULL);
        if (ret)
        {
            // Configure public address and role
            m_cmd.hal_write_config_data.offset = 0u;
            m_cmd.hal_write_config_data.length = sizeof(m_cmd.hal_write_config_data.data.public_address);
            NANO_STL_MEMCPY(m_cmd.hal_write_config_data.data.public_address, m_config.ble_public_address, m_cmd.hal_write_config_data.length);
            ret = hciSendReceive(0xFC0Cu, &m_cmd.hal_write_config_data, 2u + m_cmd.hal_write_config_data.length, &m_resp.status, sizeof(m_resp.status));
            ret = ret && (m_resp.status == 0u);

            m_cmd.hal_write_config_data.offset = 0x2Du;
            m_cmd.hal_write_config_data.length = sizeof(m_cmd.hal_write_config_data.data.role);
            m_cmd.hal_write_config_data.data.role = 2u; // Slave and master - Only one connection - 12 KB of RAM retention
            ret = ret && hciSendReceive(0xFC0Cu, &m_cmd.hal_write_config_data, 2u + m_cmd.hal_write_config_data.length, &m_resp.status, sizeof(m_resp.status));
            ret = ret && (m_resp.status == 0u);

            // Configure TX power level
            m_cmd.hal_set_tx_power_level.high_power_enabled = 0u;
            m_cmd.hal_set_tx_power_level.pa_level = 3u;
            ret = ret && hciSendReceive(0xFC0Fu, &m_cmd.hal_set_tx_power_level, sizeof(m_cmd.hal_set_tx_power_level), &m_resp.status, sizeof(m_resp.status));
            ret = ret && (m_resp.status == 0u);

            // Initialize GATT
            ret = ret && hciSendReceive(0xFD01u, NULL, 0u, &m_resp.status, sizeof(m_resp.status));
            ret = ret && (m_resp.status == 0u);

            // Initialize GAP
            enum GapRole
            {
                GAP_ROLE_PERIPHERAL = 1u,
                GAP_ROLE_BROADCASTER = 2u,
                GAP_ROLE_CENTRAL = 3u,
                GAP_ROLE_OBSERVER = 4u
            };            
            m_cmd.gap_init.role = GAP_ROLE_PERIPHERAL;
            m_cmd.gap_init.privacy = 0u;
            m_cmd.gap_init.device_name_length = NANO_STL_STRNLEN(m_device_name, sizeof(m_device_name));
            ret = ret && hciSendReceive(0xFC8Au, &m_cmd.gap_init, sizeof(m_cmd.gap_init), &m_resp.gap_init, sizeof(m_resp.gap_init));
            ret = ret && (m_resp.gap_init.status == 0u);
            if (ret)
            {
                m_gap_service_handle = m_resp.gap_init.service_handle;
                m_gap_device_name_handle = m_resp.gap_init.device_name_handle;
            }

            // Set device name
            ret = ret && updateBleCharacteristicValue(m_gap_service_handle, m_gap_device_name_handle, m_device_name, m_cmd.gap_init.device_name_length);

            // Configure I/O capabilities
            enum IoCaps
            {
                IO_CAPS_DISPLAY_ONLY = 0u,
                IO_CAPS_DISPLAY_YESNO = 1u,
                IO_CAPS_KEYBOARD_ONLY = 2u,
                IO_CAPS_NO_IN_NO_OUT = 3u,
                IO_CAPS_KEYBOARD_DISPLAY = 4u
            };
            m_cmd.gap_set_io_capability.io_capability = IO_CAPS_NO_IN_NO_OUT;
            ret = ret && hciSendReceive(0xFC85u, &m_cmd.gap_set_io_capability, sizeof(m_cmd.gap_set_io_capability), &m_resp.status, sizeof(m_resp.status));
            ret = ret && (m_resp.status == 0u);

            // Configure authentification requirements
            m_cmd.gap_set_auth_requirement.mitm_mode = 1u;
            m_cmd.gap_set_auth_requirement.oob_enable = 0u;
            m_cmd.gap_set_auth_requirement.min_encryption_key_size = 7u;
            m_cmd.gap_set_auth_requirement.max_encryption_key_size = 16u;
            m_cmd.gap_set_auth_requirement.use_fixed_pin = 0u;
            m_cmd.gap_set_auth_requirement.fixed_pin = 121016u;
            m_cmd.gap_set_auth_requirement.bounding_mode = 1u;
            ret = ret && hciSendReceive(0xFC86u, &m_cmd.gap_set_auth_requirement, sizeof(m_cmd.gap_set_auth_requirement), &m_resp.status, sizeof(m_resp.status));
            ret = ret && (m_resp.status == 0u);
        }
    }

    return ret;
}

/** \brief Method which will be called at the task's startup */
void BlueNrgMs::taskStart(void* const param)
{
    uint8_t receive_buffer[128u];

    // Task loop
    while (true)
    {
        // Wait for data ready interrupt from the module
        uint32_t flag_mask = DATA_READY_FLAG;
        if (m_data_ready_flag.wait(flag_mask, true, IOs::getInstance().getInfiniteTimeoutValue()))
        {
            // Lock access to chip
            m_mutex.lock();

            bool ret;
            const uint32_t start_time = IOs::getInstance().getMsTimestamp();
            do
            {
                uint8_t data[] = { 0x0Bu, 0x00u, 0x00u, 0x00u, 0x00u};

                ISpi::XFer spi_xfer;
                spi_xfer.write_data = data;
                spi_xfer.read_data = data;
                spi_xfer.size = sizeof(data);
                spi_xfer.cs = m_chip_select;
                spi_xfer.keep_cs_active = true;
                ret = m_spi.xfer(spi_xfer);
                if (ret)
                {
                    ret = ((data[0u] == 0x02u) && (data[3u] != 0u));
                }
                if (ret)
                {
                    // Receive all the available data
                    spi_xfer.write_data = NULL;
                    spi_xfer.read_data = receive_buffer;
                    spi_xfer.size = data[3u];
                    spi_xfer.keep_cs_active = false;
                    ret = m_spi.xfer(spi_xfer);
                    if (ret)
                    {
                        // Decode data
                        if (receive_buffer[0u] == PKT_TYPE_HCI_EVT)
                        {
                            // Event type
                            const uint8_t data_len = receive_buffer[2u];
                            switch (receive_buffer[1u])
                            {
                                case 0x05u: // Evt_Disconn_Complete
                                {
                                    m_listener->bleDisconnected();
                                    break;
                                }

                                case 0x0Eu: // Evt_Cmd_Complete
                                {
                                    // Extract opcode
                                    const uint16_t opcode = receive_buffer[4u] + (receive_buffer[5u] << 8u);
                                    if (opcode == m_resp_opcode)
                                    {
                                        // Copy received data
                                        const uint8_t resp_len = data_len - 3u;
                                        if (resp_len <= m_resp_buffer_size)
                                        {
                                            m_resp_buffer_size = resp_len;
                                            NANO_STL_MEMCPY(m_resp_buffer, &receive_buffer[6u], m_resp_buffer_size);

                                            // Signal that response is ready
                                            m_data_ready_flag.set(RESP_READY_FLAG, false);
                                        }
                                    }
                                    break;
                                }

                                case 0x0Fu: // Evt_Cmd_Status
                                {
                                    break;
                                }

                                case 0x3Eu: // Meta event
                                {
                                    // Subevent
                                    switch (receive_buffer[3u])
                                    {
                                        case 0x01: // Evt_LE_Conn_Complete
                                        {
                                            m_listener->bleConnected();
                                            break;
                                        }

                                        default:
                                        {
                                            // Ignore event
                                            break;
                                        }
                                    }
                                    break;
                                }

                                case 0x10u: // Evt_Hardware_Error
                                {
                                    break;
                                }

                                case 0xFFu: // Vendor
                                {
                                    // Extract eventcode
                                    const uint16_t ecode = receive_buffer[3u] + (receive_buffer[4u] << 8u);
                                    switch (ecode)
                                    {
                                        case 0x0C01u: // Evt_Blue_Gatt_Attribute_modified
                                        {
                                            const uint16_t attribute_handle = receive_buffer[7u] + (receive_buffer[8u] << 8u);
                                            const uint8_t attribute_length = receive_buffer[9u];
                                            m_listener->attributeModified(attribute_handle - 1u, attribute_length, &receive_buffer[12u]);
                                            break;
                                        }

                                        default:
                                        {
                                            // Ignore event
                                            break;
                                        }
                                    }
                                    break;
                                }

                                default:
                                {
                                    // Ignore
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    // No data available
                    m_spi.releaseCs();
                }
            }
            while (!ret && ((IOs::getInstance().getMsTimestamp() - start_time) < HCI_POLLING_TIMEOUT));

            // Unlock access to chip
            m_mutex.unlock();
        }
    }
}

/** \brief Called when the interrupt on the pin is triggered */
void BlueNrgMs::onPinInterrupt(const IIoPin::Level level)
{
    // Signal that data is ready to be read in the module
    m_data_ready_flag.set(DATA_READY_FLAG, true);
}

/** \brief Check if the module is present */
bool BlueNrgMs::probe()
{
    // Reset module
    m_reset_pin.setLow();    
    IOs::getInstance().waitMs(100u);
    m_it_pin.enableInterrupt();
    m_reset_pin.setHigh();

    // Wait module wake up
    IOs::getInstance().waitMs(100u);

    // Try to reach the module
    bool ret;
    const uint32_t start_time = IOs::getInstance().getMsTimestamp();
    do
    {
        uint8_t data[] = { 0x0Bu, 0x00u, 0x00u, 0x00u, 0x00u};

        ISpi::XFer spi_xfer;
        spi_xfer.write_data = data;
        spi_xfer.read_data = data;
        spi_xfer.size = sizeof(data);
        spi_xfer.cs = m_chip_select;
        ret = m_spi.xfer(spi_xfer);
        if (ret)
        {
            ret = (data[0u] == 0x02u);
        }
    }
    while (!ret && ((IOs::getInstance().getMsTimestamp() - start_time) < HCI_POLLING_TIMEOUT));

    return ret;
}

/** \brief Get the module version */
bool BlueNrgMs::getVersion(uint8_t& hw_version, uint8_t& fw_version, uint8_t& fw_subversion)
{
    bool ret;
    
    ret  = hciSendReceive(0x1001u, NULL, 0u, &m_resp.read_local_version_information, sizeof(m_resp.read_local_version_information));
    if (ret && (m_resp.read_local_version_information.status == 0u))
    {
        hw_version = (m_resp.read_local_version_information.hci_revision >> 8u);
        fw_version = (m_resp.read_local_version_information.hci_revision & 0xFFu);                           // Major Version Number
        fw_subversion = (((m_resp.read_local_version_information.lmp_pal_subversion >> 4u) & 0x0Fu) << 4u);  // Minor Version Number
        fw_subversion |= (m_resp.read_local_version_information.lmp_pal_subversion & 0x0F);                  // Patch Version Number

        ret = true;
    }

    return ret;
}

/** \brief Set the module discoverable */
bool BlueNrgMs::setDiscoverable()
{
    bool ret;

    uint8_t index = 0x00u;

    // Advertising_Event_Type
    m_cmd.buffer[index] = 0u;
    index++;

    // Adv_Interval_Min
    m_cmd.buffer[index] = (0u & 0xFFu);
    m_cmd.buffer[index + 1u] = ((0u >> 8u) & 0xFFu);
    index += 2u;

    // Adv_Interval_Max
    m_cmd.buffer[index] = (0u & 0xFFu);
    m_cmd.buffer[index + 1u] = ((0u >> 8u) & 0xFFu);
    index += 2u;

    // Address_Type
    m_cmd.buffer[index] = 0u;
    index++;

    // Adv_Filter_Policy
    m_cmd.buffer[index] = 0u;
    index++;

    // Local_Name_Length
    m_cmd.buffer[index] = NANO_STL_STRNLEN(m_device_name, sizeof(m_device_name)) + 1u;
    index++;

    // AD_TYPE_COMPLETE_LOCAL_NAME
    m_cmd.buffer[index] = 0x09u;
    index++;

    // Local_Name
    NANO_STL_MEMCPY(&m_cmd.buffer[index], m_device_name, m_cmd.buffer[index - 2u] - 1u);
    index += m_cmd.buffer[index - 2u] - 1u;

    // Service_UUID_Length
    m_cmd.buffer[index] = 0u;
    index++;

    // Service_UUID_List
    // (empty)

    // Slave_Conn_Interval_Min
    m_cmd.buffer[index] = (0u & 0xFFu);
    m_cmd.buffer[index + 1u] = ((0u >> 8u) & 0xFFu);
    index += 2u;

    // Slave_Conn_Interval_Max
    m_cmd.buffer[index] = (0u & 0xFFu);
    m_cmd.buffer[index + 1u] = ((0u >> 8u) & 0xFFu);
    index += 2u;

    ret = hciSendReceive(0xFC83u, &m_cmd.buffer[0u], index, &m_resp.status, sizeof(m_resp.status));
    ret = ret && (m_resp.status == 0u);

    return ret;
}

/** \brief Add a BLE service */
bool BlueNrgMs::addBleService(const uint8_t* uuid, const size_t uuid_size, const bool primary_service, const uint8_t max_attr_count, uint16_t& service_handle)
{
    bool ret;

    uint8_t index = 0x00u;

    // Service_UUID_Type
    m_cmd.buffer[index] = ((uuid_size == 2u) ? 1u : 2u);
    index++;

    // Service_UUID
    NANO_STL_MEMCPY(&m_cmd.buffer[index], uuid, uuid_size);
    index += uuid_size;

    // Service_Type
    m_cmd.buffer[index] = (primary_service ? 1u : 2u);
    index++;

    // Max_Attribute_ Records
    m_cmd.buffer[index] = 3 * max_attr_count + 1;
    index++;

    ret = hciSendReceive(0xFD02u, &m_cmd.buffer[0u], index, &m_resp.gatt_add_serv, sizeof(m_resp.gatt_add_serv));        
    
    ret = ret && (m_resp.gatt_add_serv.status == 0u);
    if (ret)
    {
        service_handle = m_resp.gatt_add_serv.service_handle;
    }

    return ret;
}

/** \brief Include a BLE service into another BLE service */
bool BlueNrgMs::includeBleService(const uint16_t root_service_handle, const uint16_t service_handle, const uint8_t* uuid, const size_t uuid_size, uint16_t& included_handle)
{
    bool ret;

    m_cmd.gatt_include_service.service_handle = root_service_handle;
    m_cmd.gatt_include_service.include_start_handle = service_handle;
    m_cmd.gatt_include_service.include_end_handle = service_handle;
    m_cmd.gatt_include_service.uuid_type = ((uuid_size == 2u) ? 1u : 2u);
    NANO_STL_MEMCPY(&m_cmd.gatt_include_service.uuid, uuid, uuid_size);

    ret = hciSendReceive(0xFD03u, &m_cmd.gatt_include_service, sizeof(m_cmd.gatt_include_service) - ((uuid_size == 2u) ? 14u : 0u), &m_resp.gatt_include_service, sizeof(m_resp.gatt_include_service));        

    ret = ret && (m_resp.gatt_include_service.status == 0u);
    if (ret)
    {
        included_handle = m_resp.gatt_include_service.included_handle;
    }

    return ret;
}

/** \brief Add a BLE characteristic */
bool BlueNrgMs::addBleCharacteristic(const uint16_t service_handle, const uint8_t* uuid, const size_t uuid_size, const uint8_t max_length, const bool is_fixed_length, const uint8_t properties, const SecurityPermission security_perms, uint16_t& char_handle)
{
    bool ret;

    uint8_t index = 0x00u;

    // Service_Handle
    m_cmd.buffer[index] = (service_handle & 0xFFu);
    m_cmd.buffer[index + 1u] = ((service_handle >> 8u) & 0xFFu);
    index += sizeof(service_handle);

    // Char_UUID_Type
    m_cmd.buffer[index] = ((uuid_size == 2u) ? 1u : 2u);
    index++;

    // Char_UUID
    NANO_STL_MEMCPY(&m_cmd.buffer[index], uuid, uuid_size);
    index += uuid_size;

    // Char_Value_Length
    m_cmd.buffer[index] = max_length;
    index++;

    // Char_Properties
    m_cmd.buffer[index] = properties;
    index++;

    // Security_Permissions
    m_cmd.buffer[index] = static_cast<uint8_t>(security_perms);
    index++;

    // Gatt_Evt_Mask
    m_cmd.buffer[index] = 1u;
    index++;

    // Encryption_Key_Size
    m_cmd.buffer[index] = 7u;
    index++;

    // isVariable
    m_cmd.buffer[index] = (is_fixed_length ? 0u : 1u);
    index++;

    ret = hciSendReceive(0xFD04u, &m_cmd.buffer[0u], index, &m_resp.gatt_add_char, sizeof(m_resp.gatt_add_char));        

    ret = ret && (m_resp.gatt_add_char.status == 0u);
    if (ret)
    {
        char_handle = m_resp.gatt_add_char.characteristic_handle;
    }

    return ret;
}

/** \brief Add a BLE characteristic descriptor */
bool BlueNrgMs::addBleCharacteristicDescriptor(const uint16_t service_handle, const uint16_t char_handle, const uint8_t* uuid, const size_t uuid_size, const void* value, const uint8_t length, const AccessPermission access_perms, const SecurityPermission security_perms, uint16_t& char_desc_handle)
{
    bool ret;
    uint8_t index = 0u;

    // Service_Handle
    m_cmd.buffer[index] = (service_handle & 0xFFu);
    m_cmd.buffer[index + 1u] = ((service_handle >> 8u) & 0xFFu);
    index += sizeof(service_handle);

    // Char_Handle
    m_cmd.buffer[index] = (char_handle & 0xFFu);
    m_cmd.buffer[index + 1u] = ((char_handle >> 8u) & 0xFFu);
    index += sizeof(char_handle);

    // Char_Desc_UUID type
    m_cmd.buffer[index] = ((uuid_size == 2u) ? 1u : 2u);
    index++;

    // Char_Desc_UUID
    NANO_STL_MEMCPY(&m_cmd.buffer[index], uuid, uuid_size);
    index += uuid_size;

    // Char_Desc_Value_Max_Length
    m_cmd.buffer[index] = length;
    index++;

    // Char_Desc_Value_Length
    m_cmd.buffer[index] = length;
    index++;

    // Char_Desc_Value
    NANO_STL_MEMCPY(&m_cmd.buffer[index], value, length);
    index += length;

    // Security_Permissions
    m_cmd.buffer[index] = static_cast<uint8_t>(security_perms);
    index++;

    // Access_Permissions
    m_cmd.buffer[index] = static_cast<uint8_t>(access_perms);
    index++;

    // Gatt_Event_Mask
    m_cmd.buffer[index] = 1u;
    index++;

    // Encryption_Key_Size
    m_cmd.buffer[index] = 7u;
    index++;

    // isVariable
    m_cmd.buffer[index] = 0u;
    index++;

    ret = hciSendReceive(0xFD05u, &m_cmd.buffer[0u], index, &m_resp.gatt_add_char_desc, sizeof(m_resp.gatt_add_char_desc));        

    ret = ret && (m_resp.gatt_add_char_desc.status == 0u);
    if (ret)
    {
        char_desc_handle = m_resp.gatt_add_char_desc.char_desc_handle;
    }

    return ret;
}

/** \brief Update the value of a BLE characteristic */
bool BlueNrgMs::updateBleCharacteristicValue(const uint16_t service_handle, const uint16_t char_handle, const void* value, const uint8_t length)
{
    bool ret = false;

    if (length <= sizeof(m_cmd.gatt_update_char_value.value))
    {
        m_cmd.gatt_update_char_value.service_handle = service_handle;
        m_cmd.gatt_update_char_value.char_handle = char_handle;
        m_cmd.gatt_update_char_value.val_offset = 0u;
        m_cmd.gatt_update_char_value.char_value_length = length;
        NANO_STL_MEMCPY(m_cmd.gatt_update_char_value.value, value, length);

        ret = hciSendReceive(0xFD06u, &m_cmd.gatt_update_char_value, sizeof(m_cmd.gatt_update_char_value) - (sizeof(m_cmd.gatt_update_char_value.value) - length), &m_resp.status, sizeof(m_resp.status));
        ret = ret && (m_resp.status == 0u);
    }

    return ret;
}


/** \brief Send a HCI command to the module and wait for its response */
bool BlueNrgMs::hciSendReceive(const uint16_t opcode, const void* params, const uint8_t size, void* resp_params, uint8_t resp_size)
{
    bool ret;

    // Lock access to chip
    m_mutex.lock();

    // Send command
    ret  = hciSendCommand(opcode, params, size);

    // Prepare for the response
    m_resp_opcode = opcode;
    m_resp_buffer = resp_params;
    m_resp_buffer_size = resp_size;

    // Unlock access to the chip
    m_mutex.unlock();
    if (ret)
    {
        // Wait for the response
        uint32_t flag_mask = RESP_READY_FLAG;
        ret = m_data_ready_flag.wait(flag_mask, true, 1000u);
    }

    return ret;
}

/** \brief Send a HCI command to the module */
bool BlueNrgMs::hciSendCommand(const uint16_t opcode, const void* params, const uint8_t size)
{
    // Wait module ready or wake up module
    bool ret;
    const uint32_t start_time = IOs::getInstance().getMsTimestamp();
    do
    {
        uint8_t data[] = { 0x0Au, 0x00u, 0x00u, 0x00u, 0x00u};

        ISpi::XFer spi_xfer;
        spi_xfer.write_data = data;
        spi_xfer.read_data = data;
        spi_xfer.size = sizeof(data);
        spi_xfer.cs = m_chip_select;
        spi_xfer.keep_cs_active = true;
        ret = m_spi.xfer(spi_xfer);
        if (ret)
        {
            ret = ((data[0u] == 0x02u) && (data[1u] >= (size + 3u))); 
        }
        if (!ret)
        {
            m_spi.releaseCs();
        }
    }
    while (!ret && ((IOs::getInstance().getMsTimestamp() - start_time) < HCI_POLLING_TIMEOUT));
    if (ret)
    {
        // Data part
        ISpi::XFer spi_xfer_data;
        spi_xfer_data.write_data = reinterpret_cast<const uint8_t*>(params);
        spi_xfer_data.size = size;
        spi_xfer_data.cs = m_chip_select;

        // Header part
        const uint8_t header[] = { 
                                    PKT_TYPE_HCI_CMD,
                                    static_cast<uint8_t>(opcode & 0xFF), 
                                    static_cast<uint8_t>((opcode >> 8u) & 0xFFu), 
                                    size 
                                };
        ISpi::XFer spi_xfer_header;
        spi_xfer_header.write_data = header;
        spi_xfer_header.size = sizeof(header);
        if (size != 0u)
        {
            spi_xfer_header.cs = m_chip_select;
            spi_xfer_header.keep_cs_active = true;
            spi_xfer_header.next = &spi_xfer_data;
        }

        ret = m_spi.xfer(spi_xfer_header);
    }

    return ret;
}

}
