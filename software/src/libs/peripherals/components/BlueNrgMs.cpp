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
{
    m_it_pin.registerListener(*this);
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
            struct HalWriteConfigDataReq
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
            } __attribute__((packed)) hal_write_config_data_req;
            uint8_t resp = 0xFFu;

            // Configure public address and role
            hal_write_config_data_req.offset = 0u;
            hal_write_config_data_req.length = sizeof(hal_write_config_data_req.data.public_address);
            memcpy(hal_write_config_data_req.data.public_address, m_config.ble_public_address, hal_write_config_data_req.length);
            ret = hciSendReceive(0xFC0Cu, &hal_write_config_data_req, 2u + hal_write_config_data_req.length, &resp, sizeof(resp));
            ret = ret && (resp == 0u);

            hal_write_config_data_req.offset = 0x2Du;
            hal_write_config_data_req.length = sizeof(hal_write_config_data_req.data.role);
            hal_write_config_data_req.data.role = 2u; // Slave and master - Only one connection - 12 KB of RAM retention
            ret = ret && hciSendReceive(0xFC0Cu, &hal_write_config_data_req, 2u + hal_write_config_data_req.length, &resp, sizeof(resp));
            ret = ret && (resp == 0u);    

            // Configure TX power level
            struct HalSetTxPowerLevelReq
            {
                uint8_t high_power_enabled;
                uint8_t pa_level;
            } __attribute__((packed)) hal_set_tx_power_level_req;

            hal_set_tx_power_level_req.high_power_enabled = 0u;
            hal_set_tx_power_level_req.pa_level = 3u;
            ret = ret && hciSendReceive(0xFC0Fu, &hal_set_tx_power_level_req, sizeof(hal_set_tx_power_level_req), &resp, sizeof(resp));
            ret = ret && (resp == 0u);

            // Initialize GATT
            ret = ret && hciSendReceive(0xFD01u, NULL, 0u, &resp, sizeof(resp));
            ret = ret && (resp == 0u);

            // Initialize GAP
            enum GapRole
            {
                GAP_ROLE_PERIPHERAL = 1u,
                GAP_ROLE_BROADCASTER = 2u,
                GAP_ROLE_CENTRAL = 3u,
                GAP_ROLE_OBSERVER = 4u
            };
            struct GapInitReq
            {
                uint8_t role;
                uint8_t privacy;
                uint8_t device_name_length;
            } __attribute__((packed)) gap_init_req;
            struct GapInitResp
            {
                uint8_t status;
                uint16_t service_handle;
                uint16_t device_name_handle;
                uint16_t appearance_handle;
            } __attribute__((packed)) gap_init_resp;
            
            gap_init_req.role = GAP_ROLE_PERIPHERAL;
            gap_init_req.privacy = 0u;
            gap_init_req.device_name_length = strlen("OpenVario");
            ret = ret && hciSendReceive(0xFC8Au, &gap_init_req, sizeof(gap_init_req), &gap_init_resp, sizeof(gap_init_resp));
            ret = ret && (gap_init_resp.status == 0u);

            // Set device name
            struct GattUpdateCharValueReq
            {
                uint16_t service_handle;
                uint16_t char_handle;
                uint8_t val_offset;
                uint8_t char_value_length;
                uint8_t value[32u];
            } __attribute__((packed)) gatt_update_char_value_req;

            gatt_update_char_value_req.service_handle = gap_init_resp.service_handle;
            gatt_update_char_value_req.char_handle = gap_init_resp.device_name_handle;
            gatt_update_char_value_req.val_offset = 0u;
            gatt_update_char_value_req.char_value_length = strlen("OpenVario");
            memcpy(gatt_update_char_value_req.value, "OpenVario", gatt_update_char_value_req.char_value_length);
            ret = ret && hciSendReceive(0xFD06u, &gatt_update_char_value_req, 6u + gatt_update_char_value_req.char_value_length, &resp, sizeof(resp));
            ret = ret && (resp == 0u);

            // Configure I/O capabilities
            enum IoCaps
            {
                IO_CAPS_DISPLAY_ONLY = 0u,
                IO_CAPS_DISPLAY_YESNO = 1u,
                IO_CAPS_KEYBOARD_ONLY = 2u,
                IO_CAPS_NO_IN_NO_OUT = 3u,
                IO_CAPS_KEYBOARD_DISPLAY = 4u
            };
            uint8_t io_caps = IO_CAPS_NO_IN_NO_OUT;
            ret = ret && hciSendReceive(0xFC85u, &io_caps, sizeof(io_caps), &resp, sizeof(resp));
            ret = ret && (resp == 0u);

            // Configure authentification requirements
            struct GapSetAuthRequirementReq
            {
                uint8_t mitm_mode;
                uint8_t oob_enable;
                uint8_t oob_data[16u];
                uint8_t min_encryption_key_size;
                uint8_t max_encryption_key_size;
                uint8_t use_fixed_pin;
                uint32_t fixed_pin;
                uint8_t bounding_mode;
            } __attribute__((packed)) gap_set_auth_requirement_req;

            gap_set_auth_requirement_req.mitm_mode = 1u;
            gap_set_auth_requirement_req.oob_enable = 0u;
            gap_set_auth_requirement_req.min_encryption_key_size = 7u;
            gap_set_auth_requirement_req.max_encryption_key_size = 16u;
            gap_set_auth_requirement_req.use_fixed_pin = 0u;
            gap_set_auth_requirement_req.fixed_pin = 121016u;
            gap_set_auth_requirement_req.bounding_mode = 1u;
            ret = ret && hciSendReceive(0xFC86u, &gap_set_auth_requirement_req, sizeof(gap_set_auth_requirement_req), &resp, sizeof(resp));
            ret = ret && (resp == 0u);
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
                                            memcpy(m_resp_buffer, &receive_buffer[6u], m_resp_buffer_size);

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

    // Try to reach the module at max 3 times
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

    struct ReadLocalVersionReq
    {
        uint8_t  status;
        uint8_t  hci_version;
        uint16_t hci_revision;
        uint8_t  lmp_pal_version;
        uint16_t manufacturer_name;
        uint16_t lmp_pal_subversion;
    } __attribute__((packed)) resp;
    
    ret  = hciSendReceive(0x1001u, NULL, 0u, &resp, sizeof(resp));
    if (ret)
    {
        hw_version = (resp.hci_revision >> 8u);
        fw_version = (resp.hci_revision & 0xFFu);                           // Major Version Number
        fw_subversion = (((resp.lmp_pal_subversion >> 4u) & 0x0Fu) << 4u);  // Minor Version Number
        fw_subversion |= (resp.lmp_pal_subversion & 0x0F);                  // Patch Version Number

        ret = true;
    }

    return ret;
}

/** \brief Set the module discoverable */
bool BlueNrgMs::setDiscoverable(const char* local_name)
{
    bool ret;

    struct GapSetDiscoverableReq
    {
        uint8_t advertising_event_type;
        uint16_t adv_interval_min;
        uint16_t adv_interval_max;
        uint8_t address_type;
        uint8_t adv_filter_policy;
        uint8_t local_name_length;
        uint8_t service_uuid_length;
        uint16_t slave_conn_interval_min;
        uint16_t slave_conn_interval_max;
    } __attribute__((packed)) gap_set_discoverable_req;
    uint8_t resp = 0xFFu;
    uint8_t index = 0x00u;
    uint8_t gap_set_discoverable_req_buffer[40u];

    gap_set_discoverable_req.advertising_event_type = 0u;
    gap_set_discoverable_req.adv_interval_min = 0u;
    gap_set_discoverable_req.adv_interval_max = 0u;
    gap_set_discoverable_req.address_type = 0u;
    gap_set_discoverable_req.adv_filter_policy = 0u;
    gap_set_discoverable_req.local_name_length = strlen(local_name) + 1u;
    gap_set_discoverable_req.service_uuid_length = 0u;
    gap_set_discoverable_req.slave_conn_interval_min = 0u;
    gap_set_discoverable_req.slave_conn_interval_max = 0u;

    gap_set_discoverable_req_buffer[index] = gap_set_discoverable_req.advertising_event_type;
    index++;

    gap_set_discoverable_req_buffer[index] = (gap_set_discoverable_req.adv_interval_min & 0xFFu);
    gap_set_discoverable_req_buffer[index + 1u] = ((gap_set_discoverable_req.adv_interval_min >> 8u) & 0xFFu);
    index += 2u;

    gap_set_discoverable_req_buffer[index] = (gap_set_discoverable_req.adv_interval_max & 0xFFu);
    gap_set_discoverable_req_buffer[index + 1u] = ((gap_set_discoverable_req.adv_interval_max >> 8u) & 0xFFu);
    index += 2u;

    gap_set_discoverable_req_buffer[index] = gap_set_discoverable_req.address_type;
    index++;

    gap_set_discoverable_req_buffer[index] = gap_set_discoverable_req.adv_filter_policy;
    index++;

    gap_set_discoverable_req_buffer[index] = gap_set_discoverable_req.local_name_length;
    index++;

    gap_set_discoverable_req_buffer[index] = 0x09u;
    index++;

    memcpy(&gap_set_discoverable_req_buffer[index], local_name, gap_set_discoverable_req.local_name_length);
    index += gap_set_discoverable_req.local_name_length - 1u;

    gap_set_discoverable_req_buffer[index] = gap_set_discoverable_req.service_uuid_length;
    index++;

    gap_set_discoverable_req_buffer[index] = (gap_set_discoverable_req.slave_conn_interval_min & 0xFFu);
    gap_set_discoverable_req_buffer[index + 1u] = ((gap_set_discoverable_req.slave_conn_interval_min >> 8u) & 0xFFu);
    index += 2u;

    gap_set_discoverable_req_buffer[index] = (gap_set_discoverable_req.slave_conn_interval_max & 0xFFu);
    gap_set_discoverable_req_buffer[index + 1u] = ((gap_set_discoverable_req.slave_conn_interval_max >> 8u) & 0xFFu);
    index += 2u;

    ret = hciSendReceive(0xFC83u, &gap_set_discoverable_req_buffer, index, &resp, sizeof(resp));
    ret = ret && (resp == 0u);

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
    // Wait module ready
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
