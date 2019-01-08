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

#include "Task.h"

namespace open_vario
{


/** \brief Constructor */
Task::Task(const char* name, const uint8_t priority, uint8_t stack[], const size_t stack_size)
: m_task(nullptr)
, m_task_method()
, m_param(nullptr)
{
    (void)name;
    (void)priority;
    (void)stack;
    (void)stack_size;
}

/** \brief Start the task */
bool Task::start(const TaskMethod& task_method, void* const param)
{
    bool ret = false;

    if (m_task == nullptr)
    {
        m_task_method = task_method;
        m_param = param;
        m_task = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Task::taskEntry), this, 0, nullptr);
        ret = (m_task != INVALID_HANDLE_VALUE);
    }

    return ret;
}

/** \brief Entry function of the tasks */
DWORD Task::taskEntry(void* const param)
{
    Task& task = *reinterpret_cast<Task*>(param);
    task.m_task_method.invoke(static_cast<void*>(task.m_param));
    return 0;
}


}
