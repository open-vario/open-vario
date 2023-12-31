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

#ifndef ITASK_H
#define ITASK_H

#include "Delegate.h"

namespace open_vario
{

/** \brief Macro to create a class method delegate from a private class method */
#define TASK_METHOD(class_name, method) ITask::TaskMethod::create<class_name, &class_name::method>(*this)



/** \brief Interface for all tasks implementations */
class ITask
{
    public:

        /** \brief Task method */
        typedef nano_stl::Delegate<void, void*> TaskMethod;


        /** \brief Start the task */
        virtual bool start(const TaskMethod& task_method, void* const param) = 0;

};


}

#endif // ITASK_H
