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

#include <map>
#include <string>
#include <wups/config/WUPSConfig.h>
#include "ConfigInformation.h"

ConfigInformation::ConfigInformation(WUPSConfig * config, std::string persistPath, std::string persistFileName) {
    this->config = config;
    this->persistPath = persistPath;
    this->persistFileName = persistFileName;
    createConfigSettings();
    loadValuesFromSD();
}

ConfigInformation::~ConfigInformation() {
    if(configSettings != NULL) {
        updateAndSaveSettings();
        delete configSettings;
        configSettings = NULL;
    }
    if(config != NULL) {
        delete config;
        config = NULL;
    }
}

bool ConfigInformation::createConfigSettings() {
    if(this->config == NULL) {
        return false;
    }

    if(this->configSettings != NULL) {
        delete this->configSettings;
    }

    std::map<int32_t,std::string> defaultValues;
    std::map<int32_t,std::string> settingsNames;
    int32_t index = 0;
    for (auto & curCat : this->config->getCategories()) {
        for (auto & curItem : curCat->getItems()) {
            std::string configID = curItem->getConfigID();
            std::string defaultValue = curItem->persistValue();

            defaultValues[index] = defaultValue;
            settingsNames[index] = configID;
            index++;
        }
    }

    configSettings = new ConfigSettings(persistPath, persistFileName, defaultValues, settingsNames);

    return (configSettings != NULL);
}

bool ConfigInformation::loadValuesFromSD() {
    if(this->config == NULL || this->configSettings == NULL) {
        return false;
    }

    for (auto & curCat : this->config->getCategories()) {
        for (auto & curItem : curCat->getItems()) {
            std::string configID = curItem->getConfigID();
            std::string prevValue = curItem->persistValue();
            std::string loadedValue = this->configSettings->getValueAsString(this->configSettings->getIdByName(configID));
            //DEBUG_FUNCTION_LINE("Loaded %s(%d) to %s. Previous value was %s \n",configID.c_str(),this->configSettings->getIdByName(configID), loadedValue.c_str(), prevValue.c_str());
            if(prevValue.compare(loadedValue) != 0) {
                //DEBUG_FUNCTION_LINE("Call loadValue\n");
                curItem->loadValue(loadedValue);
            }
        }
    }
    return true;
}

void ConfigInformation::updateAndSaveSettings() {
    if(this->config == NULL || this->configSettings == NULL) {
        return;
    }
    updateConfigSettings();
    configSettings->Save();
}

bool ConfigInformation::updateConfigSettings() {
    if(this->config == NULL || this->configSettings == NULL) {
        return false;
    }

    for (auto & curCat : this->config->getCategories()) {
        for (auto & curItem : curCat->getItems()) {
            std::string configID = curItem->getConfigID();
            std::string newValue = curItem->persistValue();
            this->configSettings->setValueAsString(this->configSettings->getIdByName(configID), newValue);
            //DEBUG_FUNCTION_LINE("Set %s(%d) to %s\n",configID.c_str(),this->configSettings->getIdByName(configID), newValue.c_str());
        }
    }
    return true;
}
