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
#ifndef _WUPS_CONFIG_ITEM_MULTIPLE_VALUES_H_
#define _WUPS_CONFIG_ITEM_MULTIPLE_VALUES_H_

#include <string>
#include <vector>
#include <map>
#include <wups/config/WUPSConfigItem.h>

typedef void (*MultipleValuesChangedCallback)(int32_t);

class WUPSConfigItemMultipleValues : public WUPSConfigItem {
public:
    WUPSConfigItemMultipleValues(std::string configID, std::string displayName, int32_t defaultValue, std::map<int32_t,std::string> values_, MultipleValuesChangedCallback callback);

    virtual ~WUPSConfigItemMultipleValues();

    virtual std::string getCurrentValueDisplay();

    virtual std::string getCurrentValueSelectedDisplay();

    virtual void onSelected(bool isSelected);

    virtual void onButtonPressed(WUPSConfigButtons buttons) ;

    virtual bool isMovementAllowed();

    virtual std::string persistValue();

    virtual void loadValue(std::string persistedValue);

    virtual void restoreDefault();

private:
    MultipleValuesChangedCallback callback = NULL;
    int32_t defaultValue;
    int32_t valueIndex = 0;
    std::map<int32_t,std::string> values;
};

#endif
