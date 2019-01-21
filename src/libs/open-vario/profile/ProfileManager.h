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

#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include "ConfigManager.h"
#include "MinMaxConfigValue.h"

namespace open_vario
{


/** \brief Profile manager */
class ProfileManager : public IConfigValueListener
{
    public:

        /** \brief Constructor */
        ProfileManager(ConfigManager& config_manager);


        /** \brief Initialize the profile manager */
        bool init();


        /** \brief Select the pilot */
        bool setPilot(const uint8_t pilot);

        /** \brief Get the selected pilot */
        uint8_t getPilot() const { return m_pilot; }

        /** \brief Get the selected pilot's name */
        const char* getPilotName() const { return m_pilot_name; }

        /** \brief Select the glider */
        bool setGlider(const uint8_t glider);

        /** \brief Get the selected glider */
        uint8_t getGlider() const { return m_glider; }

        /** \brief Get the selected glider's name */
        const char* getGliderName() const { return m_glider_name; }

        

        /** \brief Called when a configuration value has been modified */
        virtual void onConfigValueChange(const IConfigValue& config_value);

    private:


        /** \brief Maximum umber of chars for a pilot's name */
        static const uint8_t MAX_PILOT_NAME_SIZE = 32u;

        /** \brief Maximum umber of chars for a glider's name */
        static const uint8_t MAX_GLIDER_NAME_SIZE = 32u;

        /** \brief Maximum number of pilots */
        static const uint8_t MAX_PILOT_COUNT = 3u;

        /** \brief Maximum number of gliders */
        static const uint8_t MAX_GLIDER_COUNT = 4u;


        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief Configuration values */
        ConfigValueGroup<10u> m_config_values;
        /** \brief Configuration value : selected pilot */
        MinMaxConfigValue<uint8_t> m_config_pilot;
        /** \brief Configuration value : selected glider */
        MinMaxConfigValue<uint8_t> m_config_glider;
        /** \brief Configuration value : pilot 1 name */
        StringConfigValue<MAX_PILOT_NAME_SIZE> m_config_pilot1;
        /** \brief Configuration value : pilot 2 name */
        StringConfigValue<MAX_PILOT_NAME_SIZE> m_config_pilot2;
        /** \brief Configuration value : pilot 3 name */
        StringConfigValue<MAX_PILOT_NAME_SIZE> m_config_pilot3;
        /** \brief Configuration value : glider 1 name */
        StringConfigValue<MAX_GLIDER_NAME_SIZE> m_config_glider1;
        /** \brief Configuration value : glider 2 name */
        StringConfigValue<MAX_GLIDER_NAME_SIZE> m_config_glider2;
        /** \brief Configuration value : glider 3 name */
        StringConfigValue<MAX_GLIDER_NAME_SIZE> m_config_glider3;
        /** \brief Configuration value : glider 4 name */
        StringConfigValue<MAX_GLIDER_NAME_SIZE> m_config_glider4;
        /** \brief Configuration value : glider 5 name */
        StringConfigValue<MAX_GLIDER_NAME_SIZE> m_config_glider5;


        /** \brief Selected pilot */
        uint8_t m_pilot;

        /** \brief Selected glider */
        uint8_t m_glider;

        /** \brief Selected pilot's name */
        char m_pilot_name[MAX_PILOT_NAME_SIZE + 1u];

        /** \brief Selected glider's name */
        char m_glider_name[MAX_GLIDER_NAME_SIZE + 1u];



        /** \brief Update the pilot's name */
        bool updatePilotName();

        /** \brief Update the glider's name */
        bool updateGliderName();

};

}

#endif // PROFILEMANAGER_H
