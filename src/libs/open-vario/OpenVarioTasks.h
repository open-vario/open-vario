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

#ifndef OPENVARIOTASKS_H
#define OPENVARIOTASKS_H


//////////////////////////////////////////////////
//                  Open Vario                  //
//          Tasks priorities (0 = lowest)       //
//////////////////////////////////////////////////


/** \brief Priority of the flight recorder's task */
#define OV_TASK_PRIO_FLIGHT_RECORDER        2u

/** \brief Priority of the BLE manager's task */
#define OV_TASK_PRIO_BLE_MANAGER            3u

/** \brief Priority of the GNSS manager's task */
#define OV_TASK_PRIO_GNSS_MANAGER           4u

/** \brief Priority of the sensors manager's task */
#define OV_TASK_PRIO_SENSOR_MANAGER         5u

/** \brief Priority of the mode manager's task */
#define OV_TASK_PRIO_MODE_MANAGER           10u


#endif // OPENVARIOTASKS_H
