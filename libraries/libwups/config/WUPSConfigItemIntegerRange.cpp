/****************************************************************************
 * Copyright (C) 2019 Maschell
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

#include <wups/config/WUPSConfigItemIntegerRange.h>
#include <vector>

WUPSConfigItemIntegerRange::WUPSConfigItemIntegerRange(std::string configID, std::string displayName, int32_t defaultValue, int32_t _minValue, int32_t _maxValue, IntegerRangeValueChangedCallback callback) : WUPSConfigItem(configID,displayName)  {
    this->defaultValue = defaultValue;
    this->value = defaultValue;
    this->minValue = _minValue;
    this->maxValue = _maxValue;
    this->callback = callback;
}

WUPSConfigItemIntegerRange::~WUPSConfigItemIntegerRange() {

}

std::string WUPSConfigItemIntegerRange::getCurrentValueDisplay() {
	return "  " + std::to_string(this->value);
}

std::string WUPSConfigItemIntegerRange::getCurrentValueSelectedDisplay() {
    if(value == this->minValue) {
        return "  " + std::to_string(this->value) + " >";
    } else if(value == this->maxValue){
        return "< " + std::to_string(this->value);
    }
	return "< " + std::to_string(this->value) + " >";
}

void WUPSConfigItemIntegerRange::onSelected(bool isSelected) {

}

void WUPSConfigItemIntegerRange::onButtonPressed(WUPSConfigButtons buttons) {
    if(buttons & WUPS_CONFIG_BUTTON_LEFT){
        this->value--;
    }else if((buttons & WUPS_CONFIG_BUTTON_RIGHT)){
        this->value++;
    }else if((buttons & WUPS_CONFIG_BUTTON_L)){
        this->value = this->value - 50;
    }else if((buttons & WUPS_CONFIG_BUTTON_R)){
        this->value = this->value + 50;
    }
	
	if(this->value < this->minValue){
		this->value = this->minValue;
	}else if(this->value > this->maxValue){
		this->value = this->maxValue;
	}
}

bool WUPSConfigItemIntegerRange::isMovementAllowed() {
    return true;
}

std::string WUPSConfigItemIntegerRange::persistValue() {    
    return std::to_string(this->value);
}

void WUPSConfigItemIntegerRange::loadValue(std::string persistedValue) {
    // Note: std::stoi would throw an exception on error. atoi leads to an undefined behavior, but we
    // check if the result is in range anyway.
    int32_t newValue = atoi(persistedValue.c_str());
	
	if(newValue < this->minValue){
		newValue = this->minValue;
	}else if(newValue > this->maxValue){
		newValue = this->maxValue;
	}
	
    this->value = newValue;
}

void WUPSConfigItemIntegerRange::restoreDefault() {
    this->value = this->defaultValue;
}

bool WUPSConfigItemIntegerRange::callCallback(){
    if(callback != NULL) {
        callback(this, this->value);
        return true;
    }
    return false;
}
