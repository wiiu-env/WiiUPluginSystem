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
#ifndef _CONFIG_SETTINGS_H_
#define _CONFIG_SETTINGS_H_

#include <string>
#include <stdio.h>
#include <dynamic_libs/os_types.h>
#include <vector>
#include <map>

class ConfigSettings {
public:
    //!Constructor
    ConfigSettings(std::string configPath, std::string filename, std::map<int32_t, std::string> defaultValues, std::map<int32_t, std::string> settingsNames);
    //!Destructor
    ~ConfigSettings();

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

    const uint8_t & getDataType(int32_t idx) {
        if(isValidId(idx)) {
            return settingsValues[idx].dataType;
        }
        return nullValue.dataType;
    }

    const bool & getValueAsBool(int32_t idx) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeBool) {
            return settingsValues[idx].bValue;
        }
        return nullValue.bValue;
    }
    const int8_t & getValueAsS8(int32_t idx) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeS8) {
            return settingsValues[idx].cValue;
        }
        return nullValue.cValue;
    }
    const uint8_t & getValueAsU8(int32_t idx) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeU8) {
            return settingsValues[idx].ucValue;
        }
        return nullValue.ucValue;
    }
    const int16_t & getValueAsS16(int32_t idx) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeS16) {
            return settingsValues[idx].sValue;
        }
        return nullValue.sValue;
    }
    const uint16_t & getValueAsU16(int32_t idx) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeU16) {
            return settingsValues[idx].usValue;
        }
        return nullValue.usValue;
    }
    const int32_t & getValueAsS32(int32_t idx) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeS32) {
            return settingsValues[idx].iValue;
        }
        return nullValue.iValue;
    }
    const uint32_t & getValueAsU32(int32_t idx) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeU32) {
            return settingsValues[idx].uiValue;
        }
        return nullValue.uiValue;
    }
    const float & getValueAsF32(int32_t idx) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeF32) {
            return settingsValues[idx].fValue;
        }
        return nullValue.fValue;
    }
    const std::string & getValueAsString(int32_t idx) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeString && settingsValues[idx].strValue) {
            return *(settingsValues[idx].strValue);
        }
        return *(nullValue.strValue);
    }

    void setValueAsBool(int32_t idx, const bool & val) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeBool) {
            if(settingsValues[idx].bValue != val) {
                settingsValues[idx].bValue = val;
                bChanged = true;
            }
        }
    }

    void setValueAsS8(int32_t idx, const int8_t & val) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeS8) {
            if(settingsValues[idx].cValue != val) {
                settingsValues[idx].cValue = val;
                bChanged = true;
            }
        }
    }

    void setValueAsU8(int32_t idx, const uint8_t & val) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeU8) {
            if(settingsValues[idx].ucValue != val) {
                settingsValues[idx].ucValue = val;
                bChanged = true;
            }
        }
    }

    void setValueAsS16(int32_t idx, const int16_t & val) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeS16) {
            if(settingsValues[idx].sValue != val) {
                settingsValues[idx].sValue = val;
                bChanged = true;
            }
        }
    }

    void setValueAsU16(int32_t idx, const uint16_t & val) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeU16) {
            if(settingsValues[idx].usValue != val) {
                settingsValues[idx].usValue = val;
                bChanged = true;
            }
        }
    }

    void setValueAsS32(int32_t idx, const int32_t & val) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeS32) {
            if(settingsValues[idx].iValue != val) {
                settingsValues[idx].iValue = val;
                bChanged = true;
            }
        }
    }

    void setValueAsU32(int32_t idx, const uint32_t & val) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeU32) {
            if(settingsValues[idx].uiValue != val) {
                settingsValues[idx].uiValue = val;
                bChanged = true;
            }

        }
    }

    void setValueAsF32(int32_t idx, const float & val) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeF32) {
            if(settingsValues[idx].fValue != val) {
                settingsValues[idx].fValue = val;
                bChanged = true;
            }
        }
    }

    void setValueAsString(int32_t idx, const std::string & val) {
        if(isValidId(idx) && settingsValues[idx].dataType == TypeString && settingsValues[idx].strValue) {
            if(val.compare(*(settingsValues[idx].strValue)) != 0) { // Only update if the value changed.
                *(settingsValues[idx].strValue) = val;
                bChanged = true;
            }
        }
    }

    int32_t getIdByName(std::string configID);

protected:
    bool ValidVersion(const std::string & versionString);

    bool isValidId(int32_t idx) {
        return (settingsValues.count(idx) > 0);
    }

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
    std::string fileName;

    std::map<int32_t, SettingValue> settingsValues;

    std::map<int32_t, std::string> defaultValues;
    std::map<int32_t, std::string> settingsNames;


    bool bChanged;
};

#endif
