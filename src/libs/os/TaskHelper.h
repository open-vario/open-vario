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

#ifndef TASKHELPER_H
#define TASKHELPER_H

#include "Task.h"

namespace open_vario
{


/** \brief Task helper */
template <uint32_t STACK_SIZE>
class TaskHelper : public Task
{
    public:

        /** \brief Constructor */
        TaskHelper(const char* name, const uint8_t priority)
        : Task(name, priority, m_stack, STACK_SIZE)
        {}


    private:

        /** \brief Task's stack */
        uint8_t m_stack[STACK_SIZE];
};


}

#endif // TASKHELPER_H
