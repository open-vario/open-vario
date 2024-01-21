
#include "stm32wb5mm_ble_stack.h"
#include "app_ble.h"
#include "ble_vs_codes.h"
#include "stm32_seq.h"

extern "C"
{
#include "ble_gatt_aci.h"
}

namespace ov
{

/** @brief Singleton */
static stm32wb5mm_ble_stack* s_ble_stack;

/** @brief Constructor */
stm32wb5mm_ble_stack::stm32wb5mm_ble_stack() : m_thread(), m_services(nullptr), m_services_count(0u), m_is_ready(false)
{
    s_ble_stack = this;
}

/** @brief Start the stack */
bool stm32wb5mm_ble_stack::start(i_ble_service* services[], size_t services_count)
{
    // Save services list
    m_services       = services;
    m_services_count = services_count;

    // Start thread
    auto thread_func = ov::thread_func::create<stm32wb5mm_ble_stack, &stm32wb5mm_ble_stack::thread_func>(*this);
    bool ret         = m_thread.start(thread_func, "BLE", 6u, nullptr);

    return ret;
}

/** @brief Indicate if a device is connected */
bool stm32wb5mm_ble_stack::is_device_connected()
{
    auto status = APP_BLE_Get_Server_Connection_Status();
    bool ret    = (status == APP_BLE_CONNECTED_CLIENT) || (status == APP_BLE_CONNECTED_SERVER);
    return ret;
}

/** @brief BLE thread */
void stm32wb5mm_ble_stack::thread_func(void*)
{
    // Start BLE stack
    APPE_Tl_Init();

    // Thread loop
    while (true)
    {
        // BLE sequencer
        UTIL_SEQ_Run(0xFFFFFFFFu);

        // Create user services once the stack is ready
        if (APP_BLE_Is_Ready())
        {
            if (!m_is_ready)
            {
                if (m_services)
                {
                    SVCCTL_RegisterSvcHandler(&stm32wb5mm_ble_stack::service_event_handler);

                    for (size_t i = 0u; i < m_services_count; i++)
                    {
                        create_service(m_services[i]);
                    }
                }
                m_is_ready = true;
            }
        }
    }
}

/** @brief Create a BLE service */
void stm32wb5mm_ble_stack::create_service(i_ble_service* service)
{
    // Create service
    Service_UUID_t svc_uuid   = {};
    uint16_t       svc_handle = 0;
    memcpy(svc_uuid.Service_UUID_128, service->get_uuid(), sizeof(svc_uuid.Service_UUID_128));
    (void)aci_gatt_add_service(UUID_TYPE_128,
                               &svc_uuid,
                               PRIMARY_SERVICE,
                               1u + (3u * service->get_chars_count()), // Max_Attribute_Records
                               &svc_handle);
    service->set_handle(svc_handle);

    // Create characteristics
    Char_UUID_t            char_uuid   = {};
    uint16_t               char_handle = 0;
    i_ble_characteristic** chars       = service->get_chars();
    for (size_t i = 0; i < service->get_chars_count(); i++)
    {
        i_ble_characteristic* characteristic = chars[i];
        memcpy(char_uuid.Char_UUID_128, characteristic->get_uuid(), sizeof(char_uuid.Char_UUID_128));
        (void)aci_gatt_add_char(svc_handle,
                                UUID_TYPE_128,
                                &char_uuid,
                                characteristic->get_size(),
                                characteristic->get_properties(),
                                ATTR_PERMISSION_NONE,
                                GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP, // gattEvtMask
                                16u,                                          // encryKeySize
                                1u,                                           // isVariable: 1
                                &char_handle);
        characteristic->set_handle(char_handle);

        characteristic->register_stack_event_handler(
            i_ble_characteristic::event_handler::create<stm32wb5mm_ble_stack, &stm32wb5mm_ble_stack::on_characteristic_updated_handler>(
                *this));
    }
}

/** @brief Find a BLE characteristic from its handle */
i_ble_characteristic* stm32wb5mm_ble_stack::find_characteristic(uint16_t handle)
{
    for (size_t i = 0u; i < m_services_count; i++)
    {
        i_ble_service*         service = m_services[i];
        i_ble_characteristic** chars   = service->get_chars();
        for (size_t j = 0; j < service->get_chars_count(); j++)
        {
            i_ble_characteristic* characteristic = chars[j];
            if (characteristic->get_handle() == handle)
            {
                return characteristic;
            }
        }
    }

    return nullptr;
}

/** @brief Handler called when a characteristic value must be updated in the BLE stack */
bool stm32wb5mm_ble_stack::on_characteristic_updated_handler(i_ble_characteristic& characteristic, const void* value, size_t new_size)
{
    bool ret = false;

    bool retry         = false;
    int  retries_count = 0;
    do
    {
        int err = aci_gatt_update_char_value(characteristic.get_service().get_handle(),
                                             characteristic.get_handle(),
                                             0,        // charValOffset
                                             new_size, // charValueLen
                                             reinterpret_cast<const uint8_t*>(value));
        if ((err == BLE_STATUS_SUCCESS) || (err == BLE_STATUS_SEC_PERMISSION_ERROR))
        {
            ret = true;
        }
        else
        {
            retries_count++;
            if (retries_count < 3)
            {
                retry = true;
                ov::this_thread::sleep_for(20u);
            }
            else
            {
                retry = false;
            }
        }
    } while (retry);

    return ret;
}

/** @brief Service event handler */
SVCCTL_EvtAckStatus_t stm32wb5mm_ble_stack::service_event_handler(void* p_evt)
{
    SVCCTL_EvtAckStatus_t ret        = SVCCTL_EvtNotAck;
    hci_uart_pckt*        uart_pckt  = reinterpret_cast<hci_uart_pckt*>(p_evt);
    hci_event_pckt*       event_pckt = reinterpret_cast<hci_event_pckt*>(uart_pckt->data);

    // Handle event
    switch (event_pckt->evt)
    {
        case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
        {
            // Stack event
            evt_blecore_aci* blecore_evt = reinterpret_cast<evt_blecore_aci*>(event_pckt->data);
            switch (blecore_evt->ecode)
            {
                case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
                {
                    // Write request
                    aci_gatt_write_permit_req_event_rp0* write_perm_req;
                    write_perm_req = reinterpret_cast<aci_gatt_write_permit_req_event_rp0*>(blecore_evt->data);

                    // Get corresponding characteristic
                    i_ble_characteristic* characteristic = s_ble_stack->find_characteristic(write_perm_req->Attribute_Handle - 1u);
                    if (characteristic)
                    {
                        // Check if the new value is valid
                        bool write_authorized = characteristic->update_value_from_stack(write_perm_req->Data, write_perm_req->Data_Length);

                        // Send response
                        (void)aci_gatt_write_resp(write_perm_req->Connection_Handle,
                                                  write_perm_req->Attribute_Handle,
                                                  static_cast<uint8_t>(!write_authorized), // write_status = 0 (no error))
                                                  0u,                                      // err_code
                                                  write_perm_req->Data_Length,
                                                  &write_perm_req->Data[0]);

                        // Event has been handled
                        ret = SVCCTL_EvtAckFlowEnable;
                    }
                }
                break;

                default:
                    // Ignore
                    break;
            }
        }
        break;

        default:
            // Ignore
            break;
    }

    return ret;
}

} // namespace ov

/** @brief Idle task for BLE */
extern "C" void UTIL_SEQ_Idle()
{
    ov::this_thread::sleep_for(100u);
}
