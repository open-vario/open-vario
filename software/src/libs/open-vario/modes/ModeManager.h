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

#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include "TaskHelper.h"

namespace open_vario
{

class HmiManager;

/** \brief Mode manager */
class ModeManager : public ITaskStart
{
    public:

        /** \brief Constructor */
        ModeManager(HmiManager& hmi_manager);


        /** \brief Start the mode manager */
        virtual bool start();

        
        /** \brief Method which will be called at the task's startup */
        virtual void taskStart(void* const param);

    private:

        /** \brief Thread */
        TaskHelper<1024u> m_task;

        /** \brief HMI manager */
        HmiManager& m_hmi_manager;
};

}

#endif // MODEMANAGER_H
