/****************************************************************************
 * Copyright (C) 2015 Dimok
 *
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
#ifndef _CSETTINGS_H_
#define _CSETTINGS_H_

#include <string>
#include <stdio.h>
#include <dynamic_libs/os_types.h>
#include <vector>
#include "SettingsEnums.h"

class CSettings {
public:
    static CSettings *instance() {
        if(!settingsInstance)
            settingsInstance = new CSettings();

        return settingsInstance;
    }

    static void destroyInstance() {
        if(settingsInstance) {
            delete settingsInstance;
            settingsInstance = NULL;
        }
    }

    //!Set Default Settings
    void SetDefault();
    //!Load Settings
    bool Load();
    //!Save Settings
    bool Save();
    //!Reset Settings
    bool Reset();

    enum DataTypes {
        TypeNone,
        TypeBool,
        TypeS8,
        TypeU8,
        TypeS16,
        TypeU16,
        TypeS32,
        TypeU32,
        TypeF32,
        TypeString
    };


    enum SettingsIdx {
        INVALID = -1,
        AppLanguage,
        MAX_VALUE
    };

    static const uint8_t & getDataType(int32_t idx) {
        if(idx > INVALID && idx < MAX_VALUE)
            return instance()->settingsValues[idx].dataType;

        return instance()->nullValue.dataType;
    }

    static const bool & getValueAsBool(int32_t idx) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeBool)
            return instance()->settingsValues[idx].bValue;

        return instance()->nullValue.bValue;
    }
    static const int8_t & getValueAsS8(int32_t idx) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeS8)
            return instance()->settingsValues[idx].cValue;

        return instance()->nullValue.cValue;
    }
    static const uint8_t & getValueAsU8(int32_t idx) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeU8)
            return instance()->settingsValues[idx].ucValue;

        return instance()->nullValue.ucValue;
    }
    static const int16_t & getValueAsS16(int32_t idx) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeS16)
            return instance()->settingsValues[idx].sValue;

        return instance()->nullValue.sValue;
    }
    static const uint16_t & getValueAsU16(int32_t idx) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeU16)
            return instance()->settingsValues[idx].usValue;

        return instance()->nullValue.usValue;
    }
    static const int32_t & getValueAsS32(int32_t idx) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeS32)
            return instance()->settingsValues[idx].iValue;

        return instance()->nullValue.iValue;
    }
    static const uint32_t & getValueAsU32(int32_t idx) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeU32)
            return instance()->settingsValues[idx].uiValue;

        return instance()->nullValue.uiValue;
    }
    static const float & getValueAsF32(int32_t idx) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeF32)
            return instance()->settingsValues[idx].fValue;

        return instance()->nullValue.fValue;
    }
    static const std::string & getValueAsString(int32_t idx) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeString && instance()->settingsValues[idx].strValue)
            return *(instance()->settingsValues[idx].strValue);

        return *(instance()->nullValue.strValue);
    }

    static void setValueAsBool(int32_t idx, const bool & val) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeBool) {
            instance()->settingsValues[idx].bValue = val;
            instance()->bChanged = true;
        }
    }
    static void setValueAsS8(int32_t idx, const int8_t & val) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeS8) {
            instance()->settingsValues[idx].cValue = val;
            instance()->bChanged = true;
        }
    }
    static void setValueAsU8(int32_t idx, const uint8_t & val) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeU8) {
            instance()->settingsValues[idx].ucValue = val;
            instance()->bChanged = true;
        }
    }
    static void setValueAsS16(int32_t idx, const int16_t & val) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeS16) {
            instance()->settingsValues[idx].sValue = val;
            instance()->bChanged = true;
        }
    }
    static void setValueAsU16(int32_t idx, const uint16_t & val) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeU16) {
            instance()->settingsValues[idx].usValue = val;
            instance()->bChanged = true;
        }
    }
    static void setValueAsS32(int32_t idx, const int32_t & val) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeS32) {
            instance()->settingsValues[idx].iValue = val;
            instance()->bChanged = true;
        }
    }
    static void setValueAsU32(int32_t idx, const uint32_t & val) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeU32) {
            instance()->settingsValues[idx].uiValue = val;
            instance()->bChanged = true;
        }
    }
    static void setValueAsF32(int32_t idx, const float & val) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeF32) {
            instance()->settingsValues[idx].fValue = val;
            instance()->bChanged = true;
        }
    }
    static void setValueAsString(int32_t idx, const std::string & val) {
        if(idx > INVALID && idx < MAX_VALUE && instance()->settingsValues[idx].dataType == TypeString && instance()->settingsValues[idx].strValue) {
            *(instance()->settingsValues[idx].strValue) = val;
            instance()->bChanged = true;
        }
    }
protected:
    //!Constructor
    CSettings();
    //!Destructor
    ~CSettings();

    bool ValidVersion(const std::string & versionString);

    static CSettings *settingsInstance;

    typedef struct {
        uint8_t dataType;

        union {
            bool bValue;
            int8_t cValue;
            uint8_t ucValue;
            int16_t sValue;
            uint16_t usValue;
            int32_t iValue;
            uint32_t uiValue;
            float fValue;
            std::string *strValue;
        };
    } SettingValue;

    SettingValue nullValue;
    std::string configPath;
    std::vector<SettingValue> settingsValues;
    std::vector<const char*> settingsNames;

    bool bChanged;
};

#endif
