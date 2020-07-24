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

#ifndef OPENVARIOCONFIG_H
#define OPENVARIOCONFIG_H


//////////////////////////////////////////////////
//                  Open Vario                  //
//        Configuration groups and values       //
//////////////////////////////////////////////////


/** \brief Open vario configuration version : 1.0 */
#define OPEN_VARIO_CONFIG_VERSION 0x0100u




//////////////////////////////////////////////////
//                  Device                      //
//////////////////////////////////////////////////

/** \brief Configuration value group : device */
#define OV_CONFIG_GROUP_DEVICE                 0u

/** \brief Configuration value : device's name */
#define OV_CONFIG_VALUE_DEVICE_NAME            0u




//////////////////////////////////////////////////
//                  Sensors                     //
//////////////////////////////////////////////////

/** \brief Configuration value group : sensors */
#define OV_CONFIG_GROUP_SENSORS                 1u

/** \brief Configuration value : acquisition period */
#define OV_CONFIG_VALUE_SENSOR_ACQ_PERIOD       0u

/** \brief Configuration value : auto calibration with GNSS altitude */
#define OV_CONFIG_VALUE_SENSOR_GNSS_AUTO_CALIB  1u




//////////////////////////////////////////////////
//                  Altimeter                   //
//////////////////////////////////////////////////

/** \brief Configuration value group : altimeter */
#define OV_CONFIG_GROUP_ALTIMETER               2u

/** \brief Configuration value : reference altitude */
#define OV_CONFIG_VALUE_ALTI_REF_ALTI           0u

/** \brief Configuration value : reference temperature */
#define OV_CONFIG_VALUE_ALTI_REF_TEMP           1u

/** \brief Configuration value : reference pressure */
#define OV_CONFIG_VALUE_ALTI_REF_PRES           2u

/** \brief Configuration value : raw value filter depth */
#define OV_CONFIG_VALUE_ALTI_FILTER_DEPTH       3u

/** \brief Configuration value : offset for alti 1 */
#define OV_CONFIG_VALUE_ALTI_OFFSET_ALTI1       4u

/** \brief Configuration value : offset for alti 2 */
#define OV_CONFIG_VALUE_ALTI_OFFSET_ALTI2       5u

/** \brief Configuration value : offset for alti 3 */
#define OV_CONFIG_VALUE_ALTI_OFFSET_ALTI3       6u

/** \brief Configuration value : offset for alti 4 */
#define OV_CONFIG_VALUE_ALTI_OFFSET_ALTI4       7u




//////////////////////////////////////////////////
//                  Barometer                   //
//////////////////////////////////////////////////

/** \brief Configuration value group : barometer */
#define OV_CONFIG_GROUP_BAROMETER               3u

/** \brief Configuration value : raw value filter depth */
#define OV_CONFIG_VALUE_BARO_FILTER_DEPTH       0u




//////////////////////////////////////////////////
//                  Thermometer                 //
//////////////////////////////////////////////////

/** \brief Configuration value group : thermometer */
#define OV_CONFIG_GROUP_THERMOMETER            4u

/** \brief Configuration value : raw value filter depth */
#define OV_CONFIG_VALUE_THERMO_FILTER_DEPTH    0u




//////////////////////////////////////////////////
//                  Variometer                  //
//////////////////////////////////////////////////

/** \brief Configuration value group : variometer */
#define OV_CONFIG_GROUP_VARIOMETER              5u

/** \brief Configuration value : raw value filter depth */
#define OV_CONFIG_VALUE_VARIO_FILTER_DEPTH      0u




//////////////////////////////////////////////////
//                  Accelerometer               //
//////////////////////////////////////////////////

/** \brief Configuration value group : accelerometer */
#define OV_CONFIG_GROUP_ACCELEROMETER           6u

/** \brief Configuration value : raw value filter depth */
#define OV_CONFIG_VALUE_ACCELERO_FILTER_DEPTH   0u




//////////////////////////////////////////////////
//               Flight recorder                //
//////////////////////////////////////////////////

/** \brief Configuration value group : flight recorder */
#define OV_CONFIG_GROUP_FLIGHT_RECORDER         7u

/** \brief Configuration value : recording period of flight data */
#define OV_CONFIG_VALUE_FR_RECORD_PERIOD        0u

/** \brief Configuration value : flight detector enabled flag */
#define OV_CONFIG_VALUE_FR_FLIGHT_DETECTOR_EN   1u

/** \brief Configuration value : pressure recording enabled flag */
#define OV_CONFIG_VALUE_FR_PRESSURE_REC_EN      2u

/** \brief Configuration value : temperature recording enabled flag */
#define OV_CONFIG_VALUE_FR_TEMPERATURE_REC_EN   3u

/** \brief Configuration value : acceleration recording enabled flag */
#define OV_CONFIG_VALUE_FR_ACCELERATION_REC_EN  4u

/** \brief Configuration value : GNSS recording enabled flag */
#define OV_CONFIG_VALUE_FR_GNSS_REC_EN          5u




//////////////////////////////////////////////////
//                  Profile                     //
//////////////////////////////////////////////////

/** \brief Configuration value group : profile */
#define OV_CONFIG_GROUP_PROFILE                8u

/** \brief Configuration value : selected pilot */
#define OV_CONFIG_VALUE_PROFILE_PILOT          0u

/** \brief Configuration value : selected glider */
#define OV_CONFIG_VALUE_PROFILE_GLIDER         1u

/** \brief Configuration value : pilot 1 name */
#define OV_CONFIG_VALUE_PROFILE_PILOT1_NAME    2u

/** \brief Configuration value : pilot 2 name */
#define OV_CONFIG_VALUE_PROFILE_PILOT2_NAME    3u

/** \brief Configuration value : pilot 3 name */
#define OV_CONFIG_VALUE_PROFILE_PILOT3_NAME    4u

/** \brief Configuration value : glider 1 name */
#define OV_CONFIG_VALUE_PROFILE_GLIDER1_NAME   5u

/** \brief Configuration value : glider 2 name */
#define OV_CONFIG_VALUE_PROFILE_GLIDER2_NAME   6u

/** \brief Configuration value : glider 3 name */
#define OV_CONFIG_VALUE_PROFILE_GLIDER3_NAME   7u

/** \brief Configuration value : glider 4 name */
#define OV_CONFIG_VALUE_PROFILE_GLIDER4_NAME   8u

/** \brief Configuration value : glider 5 name */
#define OV_CONFIG_VALUE_PROFILE_GLIDER5_NAME   9u




//////////////////////////////////////////////////
//                   Alarms                     //
//////////////////////////////////////////////////

/** \brief Configuration value group : alarms */
#define OV_CONFIG_GROUP_ALARMS                  9u

/** \brief Configuration value : altitude alarm enabled */
#define OV_CONFIG_VALUE_ALARMS_ALTI_ENABLED     0u

/** \brief Configuration value : low altitude alarm threshold */
#define OV_CONFIG_VALUE_ALARMS_LALTI_THRESHOLD  1u

/** \brief Configuration value : high altitude alarm threshold */
#define OV_CONFIG_VALUE_ALARMS_HALTI_THRESHOLD  2u

/** \brief Configuration value : altitude alarm hysteresis */
#define OV_CONFIG_VALUE_ALARMS_ALTI_HYSTERESIS  3u

/** \brief Configuration value : temperature alarm enabled */
#define OV_CONFIG_VALUE_ALARMS_TEMP_ENABLED     4u

/** \brief Configuration value : low temperature alarm threshold */
#define OV_CONFIG_VALUE_ALARMS_LTEMP_THRESHOLD  5u

/** \brief Configuration value : high temperature alarm threshold */
#define OV_CONFIG_VALUE_ALARMS_HTEMP_THRESHOLD  6u

/** \brief Configuration value : temperature alarm hysteresis */
#define OV_CONFIG_VALUE_ALARMS_TEMP_HYSTERESIS  7u

/** \brief Configuration value : vario alarm enabled */
#define OV_CONFIG_VALUE_ALARMS_VARIO_ENABLED    8u

/** \brief Configuration value : low vario alarm threshold */
#define OV_CONFIG_VALUE_ALARMS_LVARIO_THRESHOLD 9u

/** \brief Configuration value : high vario alarm threshold */
#define OV_CONFIG_VALUE_ALARMS_HVARIO_THRESHOLD 10u

/** \brief Configuration value : vario alarm hysteresis */
#define OV_CONFIG_VALUE_ALARMS_VARIO_HYSTERESIS 11u

/** \brief Configuration value : speed alarm enabled */
#define OV_CONFIG_VALUE_ALARMS_SPEED_ENABLED    12u

/** \brief Configuration value : low speed alarm threshold */
#define OV_CONFIG_VALUE_ALARMS_LSPEED_THRESHOLD 13u

/** \brief Configuration value : high speed alarm threshold */
#define OV_CONFIG_VALUE_ALARMS_HSPEED_THRESHOLD 14u

/** \brief Configuration value : speed alarm hysteresis */
#define OV_CONFIG_VALUE_ALARMS_SPEED_HYSTERESIS 15u

/** \brief Configuration value : acceleration alarm enabled */
#define OV_CONFIG_VALUE_ALARMS_ACCEL_ENABLED    16u

/** \brief Configuration value : acceleration alarm threshold */
#define OV_CONFIG_VALUE_ALARMS_ACCEL_THRESHOLD  17u

/** \brief Configuration value : acceleration alarm hysteresis */
#define OV_CONFIG_VALUE_ALARMS_ACCEL_HYSTERESIS 18u




//////////////////////////////////////////////////
//                 Bluetooth                    //
//////////////////////////////////////////////////

/** \brief Configuration value group : bluetooth low energy */
#define OV_CONFIG_GROUP_BLE                     10u

/** \brief Configuration value : BLE enabled flag */
#define OV_CONFIG_VALUE_BLE_ENABLED             0u

/** \brief Configuration value : BLE hardware address */
#define OV_CONFIG_VALUE_BLE_HW_ADDRESS          1u

/** \brief Configuration value : BLE update period */
#define OV_CONFIG_VALUE_BLE_UPDATE_PERIOD       2u



//////////////////////////////////////////////////
//                   GNSS                       //
//////////////////////////////////////////////////

/** \brief Configuration value group : GNSS */
#define OV_CONFIG_GROUP_GNSS                    11u

/** \brief Configuration value : GNSS enabled flag */
#define OV_CONFIG_VALUE_GNSS_ENABLED            0u

/** \brief Configuration value : acquisition period */
#define OV_CONFIG_VALUE_GNSS_ACQ_PERIOD         1u



//////////////////////////////////////////////////
//               Date and time                  //
//////////////////////////////////////////////////

/** \brief Configuration value group : date and time */
#define OV_CONFIG_GROUP_DATE_TIME              12u

/** \brief Configuration value : time zone */
#define OV_CONFIG_VALUE_DATE_TIME_TZ           0u

/** \brief Configuration value : gnss date time synchro */
#define OV_CONFIG_VALUE_DATE_TIME_GNSS_SYNCHRO 1u




//////////////////////////////////////////////////
//                  Simulator                   //
//////////////////////////////////////////////////

/** \brief Configuration value group : simulator */
#define OV_CONFIG_GROUP_SIMULATOR               14u

/** \brief Configuration value : simulator enable flag */
#define OV_CONFIG_VALUE_SIMU_ENABLED            0u
/** \brief Configuration value : simulator ip address */
#define OV_CONFIG_VALUE_SIMU_IP_ADDRESS         1u
/** \brief Configuration value : simulator port */
#define OV_CONFIG_VALUE_SIMU_PORT               2u






#endif // OPENVARIOCONFIG_H
