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

#ifndef _WUPS_CONFIG_ITEM_BOOLEAN_H_
#define _WUPS_CONFIG_ITEM_BOOLEAN_H_

#include <string>
#include <vector>
#include <wups/config/WUPSConfigItem.h>

class WUPSConfigItemBoolean;

typedef void (*BooleanValueChangedCallback)(WUPSConfigItemBoolean *, bool);

class WUPSConfigItemBoolean : public WUPSConfigItem {
public:
    WUPSConfigItemBoolean(std::string configID, std::string displayName, bool defaultValue, BooleanValueChangedCallback callback);

    virtual ~WUPSConfigItemBoolean();

    /**
        Sets the name with will be displayed as "true" value
        \param name of the "true" value
    **/
    void setTrueValueName(std::string trueValName);

    /**
        Sets the name with will be displayed as "false" value
        \param name of the "false" value
    **/
    void setFalseValueName(std::string falseValName);

    /**
        Toggles the value. When it was true, it now false, when it was false, it's now true.
        Call the callback with the new value.
    **/
    virtual void toggleValue();

    virtual std::string getCurrentValueDisplay();

    virtual std::string getCurrentValueSelectedDisplay();

    virtual void onSelected(bool isSelected);

    virtual void onButtonPressed(WUPSConfigButtons buttons);

    virtual bool isMovementAllowed();

    virtual std::string persistValue();

    virtual void loadValue(std::string persistedValue);

    virtual void restoreDefault();

    virtual bool callCallback();

private:
    BooleanValueChangedCallback callback = NULL;
    bool value;
    bool defaultValue;
    std::string trueValName = "on";
    std::string falseValName = "off";
};

#endif
