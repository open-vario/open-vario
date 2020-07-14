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

#ifndef UARTDIAGNOSTICLINK_H
#define UARTDIAGNOSTICLINK_H

#include "IDiagnosticLink.h"
#include "IUart.h"

namespace open_vario
{



/** \brief Diagnostic links through UART */
class UartDiagnosticLink : public IDiagnosticLink
{
    public:

        /** \brief Constructor */
        UartDiagnosticLink(IUart& uart);


        /** \brief Initialize the link */
        virtual bool init() override;

        /** \brief Register a listener */
        virtual void registerListener(IDiagnosticLinkListener& listener) override { m_listener = &listener; }

        /** \brief Check if the link is available */
        virtual bool isAvailable() override { return m_is_available; }

        /** \brief Write data through the diagnostic link */
        virtual bool write(const uint8_t* const data, const uint32_t size) override;

        /** \brief Read data from the diagnostic link */
        virtual bool read(uint8_t* const data, const uint32_t size, const uint32_t timeout) override;



        /** \brief Enable link */
        void enableLink();

        /** \brief Disable link */
        void disableLink();

    private:

        /** \brief UART device */
        IUart& m_uart;

        /** \brief Listener */
        IDiagnosticLinkListener* m_listener;

        /** \brief Indicate if link is available */
        bool m_is_available;

};

}

#endif // UARTDIAGNOSTICLINK_H
