/****************************************************************************
 * Copyright (C) 2018 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#ifndef _CONFIG_INFORMATION_H_
#define _CONFIG_INFORMATION_H_

#include <wups/config/WUPSConfig.h>
#include "settings/ConfigSettings.h"
#include <string>
#include <utils/logger.h>

class ConfigInformation {

public:
    /**
        Creates a ConfigInformation object for a given WUPSConfig.
        This also is responsible for persisting/loading the configurations to/from the SDCard.
        The configuration will be loaded created/loaded at "[persistPath]/[persistFileName]"
    **/
    ConfigInformation(WUPSConfig * config, std::string persistPath, std::string persistFileName);

    /**
        Deletes the given WUPSConfig
        Calls updateAndSaveSettings
        Deletes the created ConfigSettings
    **/
    ~ConfigInformation();

    void updateAndSaveSettings();

    WUPSConfig * getConfig() {
        return config;
    }

private:
    bool createConfigSettings();
    bool loadValuesFromSD();
    bool updateConfigSettings();

    WUPSConfig * config = NULL;
    ConfigSettings * configSettings = NULL;
    std::string persistPath;
    std::string persistFileName;
};


#endif
