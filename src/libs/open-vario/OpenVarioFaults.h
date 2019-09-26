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

#ifndef OPENVARIOFAULTS_H
#define OPENVARIOFAULTS_H


//////////////////////////////////////////////////
//                  Open Vario                  //
//                Faults values                 //
//////////////////////////////////////////////////


/** \brief RTC KO */
#define OV_FAULT_RTC_KO                                 0x10u

/** \brief RTC out of range */
#define OV_FAULT_RTC_RANGE                              0x11u

/** \brief Configuration EEPROM KO */
#define OV_FAULT_CONFIG_EEPROM_KO                       0x12u

/** \brief Flight data NORFLASH KO */
#define OV_FAULT_FLIGHT_DATA_FLASH_KO                   0x13u



/** \brief Altimeter sensor KO */
#define OV_FAULT_SENSOR_ALTI_KO                         0x20u

/** \brief Altimeter sensor out of range */
#define OV_FAULT_SENSOR_ALTI_RANGE                      0x21u

/** \brief Barometer sensor KO */
#define OV_FAULT_SENSOR_BARO_KO                         0x22u

/** \brief Barometer sensor out of range */
#define OV_FAULT_SENSOR_BARO_RANGE                      0x23u

/** \brief Thermometer sensor KO */
#define OV_FAULT_SENSOR_THERMO_KO                       0x24u

/** \brief Thermometer sensor out of range */
#define OV_FAULT_SENSOR_THERMO_RANGE                    0x25u

/** \brief Variometer sensor KO */
#define OV_FAULT_SENSOR_VARIO_KO                        0x26u

/** \brief Variometer sensor out of range */
#define OV_FAULT_SENSOR_VARIO_RANGE                     0x27u

/** \brief Accelerometer sensor KO */
#define OV_FAULT_SENSOR_ACCEL_KO                        0x28u

/** \brief Accelerometer sensor out of range */
#define OV_FAULT_SENSOR_ACCEL_RANGE                     0x29u



/** \brief GNSS KO */
#define OV_FAULT_GNSS_KO                                0x30u

/** \brief GNSS no fix */
#define OV_FAULT_GNSS_NO_FIX                            0x31u



/** \brief Bluetooth Low Energy module KO */
#define OV_FAULT_BLE_KO                                 0x40u



/** \brief USB CDC KO */
#define OV_FAULT_USB_CDC_KO                             0x50u




#endif // OPENVARIOFAULTS_H
