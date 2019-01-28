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

#include "BleTest.h"
#include "IOs.h"
#include "BleService.h"
#include "BleCharacteristic.h"
#include "BleUuid16.h"
#include "BleUuid128.h"

extern "C" void updateDouble(double& d)
{
    d += 0.125;
}

namespace open_vario
{


/** \brief Constructor */
BleTest::BleTest(BlueNrgMs& blue_nrg)
: m_blue_nrg(blue_nrg)
, m_blue_nrg_stack(m_blue_nrg)
, m_task("BLE test", 5u)
{
    m_blue_nrg_stack.setListener(*this);
}

/** \brief Start the task */
bool BleTest::start()
{
    ITask::TaskMethod task_method = TASK_METHOD(BleTest, taskStart);
    return m_task.start(task_method, nullptr);
}

/** \brief Method which will be called at the task's startup */
void BleTest::taskStart(void* const param)
{
    (void)param;

    static const uint8_t uuid_service1_arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    static BleUuid128 uuid_service_1(uuid_service1_arr);
    static BleService<0u, 2u> service1("Service 1", uuid_service_1);

    //static const uint8_t serv1_char1_uuid_arr[] = {71, 72, 73, 74, 75, 76, 77, 87, 79, 80, 81, 82, 83, 84, 185, 16};
    //static BleUuid128 serv1_char1_uuid(serv1_char1_uuid_arr);
    static BleUuid16 serv1_char1_uuid(0x1000u);
    static BleCharacteristic<uint8_t, 0u> serv1_char1("Serv1 Char1", serv1_char1_uuid, true, IBleCharacteristic::PROP_READ);
    service1.addCharacteristic(serv1_char1);
    
    static BleUuid16 serv1_char2_uuid(0x1001u);
    static BleCharacteristic<double, 0u> serv1_char2("Serv1 Char2", serv1_char2_uuid, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_WRITE | IBleCharacteristic::PROP_NOTIFY);
    service1.addCharacteristic(serv1_char2);
    

    static const uint8_t uuid_service2_arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160};
    static BleUuid128 uuid_service_2(uuid_service2_arr);
    static BleService<1u, 1u> service2("Service 2", uuid_service_2);

    static BleUuid16 serv2_char1_uuid(0x2000u);
    static BleCharacteristic<uint32_t, 0u> serv2_char1("Serv2 Char1", serv2_char1_uuid, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_WRITE_WO_RESP | IBleCharacteristic::PROP_NOTIFY);
    service2.addCharacteristic(serv2_char1);


    static const uint8_t uuid_service3_arr[] = {11, 21, 31, 41, 51, 61, 71, 81, 91, 101, 111, 121, 131, 141, 151, 161};
    static BleUuid128 uuid_service_3(uuid_service3_arr);
    static BleService<0u, 1u> service3("Service 3", uuid_service_3);
    service2.addService(service3);

    static BleUuid16 serv3_char1_uuid(0x3000u);
    static BleCharacteristic<int16_t, 0u> serv3_char1("Serv3 Char1", serv3_char1_uuid, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_WRITE);
    service3.addCharacteristic(serv3_char1);

    static StaticVector<IBleService*, 3u> ble_services;
    ble_services.pushBack(&service1);
    ble_services.pushBack(&service2);
    ble_services.pushBack(&service3);
    m_blue_nrg_stack.setBleServices(ble_services);

    BlueNrgMsStack::DeviceConfiguration device_config;
    device_config.hw_address[0u] = 0x12u;
    device_config.hw_address[1u] = 0x34u;
    device_config.hw_address[2u] = 0x00u;
    device_config.hw_address[3u] = 0xE1u;
    device_config.hw_address[4u] = 0x80u;
    device_config.hw_address[5u] = 0x04u;
    strcpy(device_config.name, "OpenVario");
    m_blue_nrg_stack.setDeviceConfiguration(device_config);

    m_blue_nrg_stack.start();

    // Task loop
    uint8_t char1_val = 0u;
    double char2_val = 1.0;
    uint32_t char3_val = 0u;
    int16_t char4_val = -12345;
    while (true)
    {
        IOs::getInstance().waitMs(500u);
        char1_val++;
        serv1_char1.update(char1_val);

        char2_val += 0.2512234455331;
        serv1_char2.update(char2_val);

        char3_val++;
        serv2_char1.update(char3_val);

        char4_val++;
        serv3_char1.update(char4_val);
    }
}

/** \brief Called when a client device is connected to the stack */
void BleTest::bleClientConnected()
{

}

/** \brief Called when a client device is connected to the stack */
void BleTest::bleClientDisconnected()
{

}


}
