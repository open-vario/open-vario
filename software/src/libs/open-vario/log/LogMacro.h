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

#ifndef LOGMACRO_H
#define LOGMACRO_H


#ifndef OPENVARIO_DISABLE_LOGS

#include "IOpenVarioApp.h"

/** \brief Log a debug message */
#define LOG_DEBUG(msg, ...) IOpenVarioApp::getInstance().getLogger().log(Log::LL_DEBUG, (msg), ##__VA_ARGS__)

/** \brief Log an info message */
#define LOG_INFO(msg, ...) IOpenVarioApp::getInstance().getLogger().log(Log::LL_INFO, (msg), ##__VA_ARGS__)

/** \brief Log an error message */
#define LOG_ERROR(msg, ...) IOpenVarioApp::getInstance().getLogger().log(Log::LL_ERROR, (msg), ##__VA_ARGS__)

#else

/** \brief Log a debug message */
#define LOG_DEBUG(msg, ...)

/** \brief Log an info message */
#define LOG_INFO(msg, ...)

/** \brief Log an error message */
#define LOG_ERROR(msg, ...)

#endif // OPENVARIO_DISABLE_LOGS

#endif // LOGMACRO_H
