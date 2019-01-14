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

#ifndef SIMUEEPROM_H
#define SIMUEEPROM_H

#include "IEeprom.h"
#include "File.h"

#include <string>

namespace open_vario
{


/** \brief Simulated EEPROM */
class SimuEeprom : public IEeprom
{
    public:

        /** \brief Contructor */
        SimuEeprom(const std::string& binfile_path, const uint16_t size);


        /** \brief Get the size of the EEPROM in bytes */
        virtual uint16_t getSize() const { return m_size; }

        /** \brief Configure the EEPROM */
        virtual bool configure();

        /** \brief Read data from the EEPROM */
        virtual bool read(const uint16_t address, uint8_t data[], const uint16_t size);

        /** \brief Write data to the EEPROM */
        virtual bool write(const uint16_t address, const uint8_t data[], const uint16_t size);

    private:

        /** \brief Eeprom's binary file path */
        const std::string m_binfile_path;

        /** \brief Eeprom's size */
        const uint16_t m_size;

        /** \brief Eeprom's binary file */
        File m_file;
        
};

}

#endif // SIMUEEPROM_H
