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
//                  Global                      //
//////////////////////////////////////////////////

/** \brief Configuration value group : global */
#define OV_CONFIG_GROUP_GLOBAL                 0u




//////////////////////////////////////////////////
//                  Sensors                     //
//////////////////////////////////////////////////

/** \brief Configuration value group : sensors */
#define OV_CONFIG_GROUP_SENSORS                 1u

/** \brief Configuration value : acquisition period */
#define OV_CONFIG_VALUE_SENSOR_ACQ_PERIOD       0u




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
//                  Simulator                   //
//////////////////////////////////////////////////

/** \brief Configuration value group : simulator */
#define OV_CONFIG_GROUP_SIMULATOR               10u

/** \brief Configuration value : simulator enable flag */
#define OV_CONFIG_VALUE_SIMU_ENABLED            0u
/** \brief Configuration value : simulator ip address */
#define OV_CONFIG_VALUE_SIMU_IP_ADDRESS         1u
/** \brief Configuration value : simulator port */
#define OV_CONFIG_VALUE_SIMU_PORT               2u



#endif // OPENVARIOCONFIG_H
