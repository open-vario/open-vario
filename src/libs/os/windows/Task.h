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

#ifndef TASK_H
#define TASK_H

#include "ITask.h"

#include <Windows.h>

namespace open_vario
{


/** \brief Task implementation for Windows */
class Task : public ITask
{
    public:

        /** \brief Constructor */
        Task(const char* name, const uint8_t priority, uint8_t stack[], const size_t stack_size);


        /** \brief Start the task */
        virtual bool start(const TaskMethod& task_method, void* const param);


    private:

        /** \brief Underlying Windows task handle */
        HANDLE m_task;

        /** \brief Task method */
        TaskMethod m_task_method;

        /** \bruef Task param */
        void* m_param;


        /** \brief Entry function of the tasks */
        static DWORD taskEntry(void* const param);
};


}

#endif // TASK_H
