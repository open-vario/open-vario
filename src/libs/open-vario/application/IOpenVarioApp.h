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

#ifndef IOPENVARIOAPP_H
#define IOPENVARIOAPP_H

#include "IOs.h"
#include "IOpenVarioBoard.h"
#include "ILogger.h"
#include "IAltimeter.h"
#include "IBarometer.h"
#include "IThermometer.h"
#include "IVariometer.h"

namespace open_vario
{


/** \brief Interface for all open-vario applications implementations */
class IOpenVarioApp
{
    public:

        /** \brief Get the version string */
        virtual const char* getVersion() = 0;

        /** \brief Get the Operating System */
        virtual IOs& getOs() = 0;

        /** \brief Get the board */
        virtual IOpenVarioBoard& getBoard() = 0;

        /** \brief Get the logger */
        virtual ILogger& getLogger() = 0;

        /** \brief Get the altimeter */
        virtual IAltimeter& getAltimeter() = 0;

        /** \brief Get the barometer */
        virtual IBarometer& getBarometer() = 0;

        /** \brief Get the thermometer */
        virtual IThermometer& getThermometer() = 0;

        /** \brief Get the variometer */
        virtual IVariometer& getVariometer() = 0;


        /** \brief Initialize the application */
        virtual bool init(uint8_t argc, char* argv[]) = 0;

        /** \brief Start the application */
        virtual bool start() = 0;


        /** \brief Singleton to retrieve the unique instance of the application */
        static IOpenVarioApp& getInstance();
};

}

#endif // IOPENVARIOAPP_H
