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

#include <wups/config/WUPSConfigItemBoolean.h>
#include <vector>

WUPSConfigItemBoolean::WUPSConfigItemBoolean(std::string configID, std::string displayName, bool defaultValue, BooleanValueChangedCallback callback) : WUPSConfigItem(configID,displayName)  {
    this->defaultValue = defaultValue;
    this->value = defaultValue;
    this->callback = callback;
}

WUPSConfigItemBoolean::~WUPSConfigItemBoolean() {

}

void WUPSConfigItemBoolean::setTrueValueName(std::string trueValName) {
    this->trueValName = trueValName;
}

void WUPSConfigItemBoolean::setFalseValueName(std::string falseValName) {
    this->falseValName = falseValName;
}

std::string WUPSConfigItemBoolean::getCurrentValueDisplay() {
    if(value) {
        return "  " + trueValName;
    } else {
        return "  " + falseValName;
    }
}

std::string WUPSConfigItemBoolean::getCurrentValueSelectedDisplay() {
    if(value) {
        return "  " + trueValName + " >";
    } else {
        return "< " + falseValName;
    }
}

void WUPSConfigItemBoolean::onSelected(bool isSelected) {

}

void WUPSConfigItemBoolean::onButtonPressed(WUPSConfigButtons buttons) {
    if(buttons & WUPS_CONFIG_BUTTON_A) {
        toggleValue();
    }else if(buttons & WUPS_CONFIG_BUTTON_LEFT && !value){
        toggleValue();
    }else if((buttons & WUPS_CONFIG_BUTTON_RIGHT) && value){
        toggleValue();
    }
}

bool WUPSConfigItemBoolean::isMovementAllowed() {
    return true;
}

std::string WUPSConfigItemBoolean::persistValue() {
    if(value) {
        return "1";
    }
    return "0";
}

void WUPSConfigItemBoolean::loadValue(std::string persistedValue) {
    bool newValue = false;
    if(persistedValue.compare("1") == 0) {
        newValue = true;
    }
    value = newValue;
}

void WUPSConfigItemBoolean::toggleValue() {
    value = !value;
}

void WUPSConfigItemBoolean::restoreDefault() {
    this->value = this->defaultValue;
}

bool WUPSConfigItemBoolean::callCallback(){
    if(callback != NULL) {
        callback(this, this->value);
        return true;
    }
    return false;
}
