/****************************************************************************
 * Copyright (C) 2015 Dimok
 * Modified by Maschell, 2018 for WiiU plugin system loader
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/common.h"
#include "ConfigSettings.h"
#include <fs/CFile.hpp>
#include <fs/FSUtils.h>
#include <utils/StringTools.h>
#include <utils/logger.h>
#include <language/gettext.h>

#define VERSION_LINE        "# WiiUPluginSystem - plugin settings file v"
#define VALID_VERSION       1

ConfigSettings::ConfigSettings(std::string configPath, std::string filename, std::map<int32_t, std::string> defaultValues, std::map<int32_t, std::string> settingsNames) {
    bChanged = false;
    memset(&nullValue, 0, sizeof(nullValue));
    nullValue.strValue = new std::string();

    this->configPath = configPath;
    this->fileName = filename;
    this->defaultValues = defaultValues;
    this->settingsNames = settingsNames;

    this->Load();
}

ConfigSettings::~ConfigSettings() {
    for(uint32_t i = 0; i < settingsValues.size(); i++) {
        if(settingsValues[i].dataType == TypeString)
            delete settingsValues[i].strValue;
    }
    delete nullValue.strValue;
}

void ConfigSettings::SetDefault() {
    for(uint32_t i = 0; i < settingsValues.size(); i++) {
        if(settingsValues[i].dataType == TypeString) {
            delete settingsValues[i].strValue;
        }
    }

    for (auto & kv : settingsNames) {
        settingsNames[kv.first] = kv.second;
    }

    for (auto & kv : defaultValues) {
        SettingValue value;
        value.dataType = TypeString;
        value.strValue = new std::string(kv.second);
        settingsValues[kv.first] = value;
    }
}

bool ConfigSettings::Load() {
    //! Reset default path variables to the right device
    SetDefault();

    std::string filepath = configPath + "/";
    filepath += fileName;

    DEBUG_FUNCTION_LINE("Loading Configuration from %s\n",filepath.c_str());

    CFile file(filepath, CFile::ReadOnly);
    if (!file.isOpen()) {
        DEBUG_FUNCTION_LINE("Failed to open file\n");
        bChanged = true;
        return false;
    }

    std::string strBuffer;
    strBuffer.resize(file.size());
    file.read((uint8_t *) &strBuffer[0], strBuffer.size());
    file.close();

    //! remove all windows crap signs
    size_t position;
    while(1 && !strBuffer.empty()) {
        position = strBuffer.find('\r');
        if(position == std::string::npos)
            break;

        strBuffer.erase(position, 1);
    }

    std::vector<std::string> lines = StringTools::stringSplit(strBuffer, "\n");

    if(lines.empty() || !ValidVersion(lines[0])) {
        return false;
    }

    for(uint32_t i = 0; i < lines.size(); ++i) {
        std::vector<std::string> valueSplit = StringTools::stringSplit(lines[i], "=");
        if(valueSplit.size() != 2) {
            continue;
        }

        while((valueSplit[0].size() > 0) && valueSplit[0][0] == ' ') {
            valueSplit[0].erase(0, 1);
        }
        while((valueSplit[1].size() > 0) && valueSplit[1][ valueSplit[1].size() - 1 ] == ' ') {
            valueSplit[1].resize(valueSplit[1].size() - 1);
        }
        for(uint32_t n = 0; n < settingsNames.size(); n++) {
            if(settingsNames.empty()) {
                continue;
            }

            if(valueSplit[0] == settingsNames[n]) {
                switch(settingsValues[n].dataType) {
                case TypeBool:
                    settingsValues[n].bValue = atoi(valueSplit[1].c_str());
                    break;
                case TypeS8:
                    settingsValues[n].cValue = atoi(valueSplit[1].c_str());
                    break;
                case TypeU8:
                    settingsValues[n].ucValue = atoi(valueSplit[1].c_str());
                    break;
                case TypeS16:
                    settingsValues[n].sValue = atoi(valueSplit[1].c_str());
                    break;
                case TypeU16:
                    settingsValues[n].usValue = atoi(valueSplit[1].c_str());
                    break;
                case TypeS32:
                    settingsValues[n].iValue = atoi(valueSplit[1].c_str());
                    break;
                case TypeU32:
                    settingsValues[n].uiValue = strtoul(valueSplit[1].c_str(), 0, 10);
                    break;
                case TypeF32:
                    settingsValues[n].fValue = atof(valueSplit[1].c_str());
                    break;
                case TypeString:
                    if(settingsValues[n].strValue == NULL)
                        settingsValues[n].strValue = new std::string();

                    *settingsValues[n].strValue = valueSplit[1];
                    break;
                default:
                    break;
                }
            }
        }
    }

    return true;
}

bool ConfigSettings::ValidVersion(const std::string & versionString) {
    int32_t version = 0;

    if(versionString.find(VERSION_LINE) != 0) {
        return false;
    }

    version = atoi(versionString.c_str() + strlen(VERSION_LINE));

    return version == VALID_VERSION;
}

bool ConfigSettings::Reset() {
    this->SetDefault();
    bChanged = true;

    if (this->Save()) {
        return true;
    }

    return false;
}

int32_t ConfigSettings::getIdByName(std::string configID) {
    for (auto & kv : settingsNames) {
        if(configID.compare(kv.second) == 0) {
            return kv.first;
        }
    }
    return -1;
}

bool ConfigSettings::Save() {
    if(!bChanged) {
        DEBUG_FUNCTION_LINE("Nothing has changed, we can skip\n");
        return true;
    }

    FSUtils::CreateSubfolder(configPath.c_str());

    std::string filepath = configPath + "/";
    filepath += fileName;

    s32 res = open(filepath.c_str(), O_CREAT | O_TRUNC | O_WRONLY);
    close(res);

    CFile file(filepath, CFile::WriteOnly);
    if (!file.isOpen()) {
        DEBUG_FUNCTION_LINE("failed to open %s\n",filepath.c_str());
        return false;
    }

    file.fwrite("%s%i\n", VERSION_LINE, VALID_VERSION);

    for(uint32_t i = 0; i < settingsValues.size(); i++) {
        switch(settingsValues[i].dataType) {
        case TypeBool:
            file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].bValue);
            break;
        case TypeS8:
            file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].cValue);
            break;
        case TypeU8:
            file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].ucValue);
            break;
        case TypeS16:
            file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].sValue);
            break;
        case TypeU16:
            file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].usValue);
            break;
        case TypeS32:
            file.fwrite("%s=%i\n", settingsNames[i], settingsValues[i].iValue);
            break;
        case TypeU32:
            file.fwrite("%s=%u\n", settingsNames[i], settingsValues[i].uiValue);
            break;
        case TypeF32:
            file.fwrite("%s=%f\n", settingsNames[i], settingsValues[i].fValue);
            break;
        case TypeString:
            if(settingsValues[i].strValue != NULL)
                file.fwrite("%s=%s\n", settingsNames[i].c_str(), settingsValues[i].strValue->c_str());
            break;
        default:
            break;
        }
    }

    file.close();
    bChanged = false;

    return true;
}
