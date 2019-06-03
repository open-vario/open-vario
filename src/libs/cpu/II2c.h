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

#ifndef II2C_H
#define II2C_H

#include <cstdint>
#include <cstddef>

namespace open_vario
{


/** \brief Interface for all I2C implementations */
class II2c
{
    public:
    
        /** \brief I2C transfer description */
        struct XFer
        {
            /** \brief Constructor */
            XFer()
            : data(nullptr)
            , size(0u)
            , read(true)
            , stop_cond(true)
            , next(nullptr)
            {}

            /** \brief Buffer */
            uint8_t* data;
            /** \brief Buffer size in bytes */
            uint32_t size;
            /** \brief Read data on I2C bus */
            bool read;
            /** \brief Trigger a stop condition at the end of the transfer */
            bool stop_cond;
            /** \brief Next transfer */
            const XFer* next;
        };

        /** \brief I2C errors */
        enum I2cError
        {
            /** \brief No error */
            I2C_SUCCESS,
            /** \brief Arbitration lost */
            I2C_ERR_ARB_LOST,
            /** \brief Bus error */
            I2C_ERR_BUS_ERROR,
            /** \brief Nack received */
            I2C_ERR_NACK,
            /** \brief Other */
            I2C_ERR_OTHER
        };


        /** \brief Configure the I2C */
        virtual bool configure() = 0;

        /** \brief Transfer data through the I2C */
        virtual bool xfer(const uint8_t slave_address, const XFer& xfer, I2cError& error) = 0;

};

}

#endif // II2C_H
