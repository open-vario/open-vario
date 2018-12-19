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
