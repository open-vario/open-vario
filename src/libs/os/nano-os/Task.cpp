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
#include "nano-stl-conf.h"

namespace open_vario
{


/** \brief Constructor */
Task::Task(const char* name, const uint8_t priority, uint8_t stack[], const size_t stack_size)
: m_task()
, m_task_init_data()
, m_task_start(NULL)
, m_param(NULL)
{
    NANO_STL_MEMSET(&m_task_init_data, 0, sizeof(m_task_init_data));
    m_task_init_data.name = name;
    m_task_init_data.base_priority = priority;
    m_task_init_data.stack_origin = reinterpret_cast<nano_os_stack_t*>(stack);
    m_task_init_data.stack_size = stack_size / sizeof(nano_os_stack_t);
    m_task_init_data.task_func = Task::taskEntry;
    m_task_init_data.port_init_data.is_priviledged = true;
    m_task_init_data.port_init_data.use_fpu = false;
}

/** \brief Start the task */
bool Task::start(ITaskStart& task_start, void* const param)
{
    m_task_start = &task_start;
    m_param = param;
    m_task_init_data.param = this;

    const nano_os_error_t ret = NANO_OS_TASK_Create(&m_task, &m_task_init_data);
    return (ret == NOS_ERR_SUCCESS);
}

/** \brief Entry function of the tasks */
void* Task::taskEntry(void* const param)
{
    Task& task = *reinterpret_cast<Task*>(param);
    task.m_task_start->taskStart(task.m_param);
    return NULL;
}


}
