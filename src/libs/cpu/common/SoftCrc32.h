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

#ifndef SOFTCRC32_H
#define SOFTCRC32_H

#include "ICrc32.h"

namespace open_vario
{

/** \brief Software CRC-32 */
class SoftCrc32: public ICrc32
{
    public:

        /** \brief Constructor */
        SoftCrc32();


        /** \brief Configure the CRC-32 driver */
        virtual bool configure();

        /** \brief Reset the CRC-32 computation */
        virtual void reset() { m_crc = 0xFFFFFFFFu; }

        /** \brief Update the CRC-32 computation with a data buffer */
        virtual uint32_t update(const void* buffer, const size_t size);

        /** \brief Get the current value of the CRC-32 computation */
        virtual uint32_t value() const { return static_cast<uint32_t>(m_crc ^ 0xFFFFFFFFu); }


    private:

        /** \brief Current CRC-32 value */
        uint32_t m_crc;

        /** \brief Pre-computed values */
        static const uint32_t m_crc_table[256u];
};

}

#endif // SOFTCRC32_H
