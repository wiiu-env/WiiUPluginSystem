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

#include <string>
#include <vector>
#include <wups/config/WUPSConfigItemMultipleValues.h>
#include <stdlib.h>     /* atoi */

WUPSConfigItemMultipleValues::WUPSConfigItemMultipleValues(std::string configID, std::string displayName, int32_t defaultValue, std::map<int32_t,std::string> values_, MultipleValuesChangedCallback callback) : WUPSConfigItem(configID,displayName)  {
    if(values_.size() == 0) {
        this->values[defaultValue] = "<ERROR>";
    } else {
        this->values = values_;
    }

    if (values.count(defaultValue) > 0) {
        this->defaultValue = defaultValue;
        valueIndex = 0;
        for (auto& kv : values) {
            if(defaultValue == kv.first) {
                break;
            }
            valueIndex++;
        }
    } else {
        for (auto& kv : values) {
            this->defaultValue = kv.first;
            break;
        }
        valueIndex = 0;
    }

    this->callback = callback;
}

WUPSConfigItemMultipleValues::~WUPSConfigItemMultipleValues() {

}

std::string WUPSConfigItemMultipleValues::getCurrentValueDisplay() {
    uint32_t index = 0;
    for (auto& kv : values) {
        if(index == valueIndex) {
            return "  " + kv.second;
        }
        index++;
    }
    return "";
}

std::string WUPSConfigItemMultipleValues::getCurrentValueSelectedDisplay() {
    if(values.size() == 0) {
        return "<ERROR>";
    }

    uint32_t index_max = values.size() -1;

    uint32_t index = 0;
    for (auto& kv : values) {
        if(index == valueIndex) {
            std::string s;
            if(valueIndex != 0) {
                s.append("< ");
            } else {
                s.append("  ");
            }
            s.append(kv.second);
            if(valueIndex != index_max) {
                s.append(" >");
            }
            return s;
        }
        index++;
    }
    return "<ERROR>";
}


void WUPSConfigItemMultipleValues::onSelected(bool isSelected) {
    // not needed.
}

void WUPSConfigItemMultipleValues::onButtonPressed(WUPSConfigButtons buttons) {
    if(buttons & WUPS_CONFIG_BUTTON_LEFT) {
        if(valueIndex != 0){
            valueIndex--;
        }
    }
    if(buttons & WUPS_CONFIG_BUTTON_RIGHT && values.size() > 0) {
        valueIndex++;
        if(valueIndex > (int32_t) values.size()-1) {
            valueIndex = values.size()-1;
        }
    }
}

bool WUPSConfigItemMultipleValues::isMovementAllowed() {
    return true;
}

std::string WUPSConfigItemMultipleValues::persistValue() {
    return std::to_string(this->valueIndex);
}

void WUPSConfigItemMultipleValues::loadValue(std::string persistedValue) {
    // Note: std::stoi would throw an exception on error. atoi leads to an undefined behavior, but we
    // check if the result is in range anyway.
    uint32_t newValueIndex = atoi(persistedValue.c_str());
    if(newValueIndex >= 0 && (newValueIndex +1) <= this->values.size()) {
        valueIndex = newValueIndex;
    }else{
        valueIndex = 0;
    }
}

void WUPSConfigItemMultipleValues::restoreDefault() {
    this->valueIndex = 0;
}

bool WUPSConfigItemMultipleValues::callCallback() {
    if(callback == NULL) {
        return false;
    }
    uint32_t index = 0;
    for (auto& kv : values) {
        if(index == valueIndex) {
            callback(this, kv.first);
            return true;
        }
        index++;
    }
    return false;
}
