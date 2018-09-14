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

#ifndef _WUPS_CONFIG_ITEM_H_
#define _WUPS_CONFIG_ITEM_H_

#include <string>
#include <vector>

#define WUPS_CONFIG_BUTTON_NONE     0
#define WUPS_CONFIG_BUTTON_LEFT     (1<<0)
#define WUPS_CONFIG_BUTTON_RIGHT    (1<<1)
#define WUPS_CONFIG_BUTTON_UP       (1<<2)
#define WUPS_CONFIG_BUTTON_DOWN     (1<<3)
#define WUPS_CONFIG_BUTTON_A        (1<<4)
#define WUPS_CONFIG_BUTTON_B        (1<<5)
#define WUPS_CONFIG_BUTTON_ZL       (1<<6)
#define WUPS_CONFIG_BUTTON_ZR       (1<<7)
typedef int32_t WUPSConfigButtons;

class WUPSConfigItem {
public:
    /**
        Sets the display name of this WUPSConfigItem
        This is the value which will be shown in the configuration menu.
    **/
    virtual void setDisplayName(std::string displayName) {
        this->displayName = displayName;
    }

    /**
        \return Returns the display name of this WUPSConfigItem
    **/
    virtual std::string getDisplayName() {
        return this->displayName;
    }

    /**
        Sets the config ID name of this WUPSConfigItem.
        This config ID is used to persist the configuration values and needs
        to be unique in the context of this WUPSConfig.
        Items in different categories are NOT allowed to have the config ID.
    **/
    virtual void setConfigID(std::string configID) {
        this->configID = configID;
    }

    /**
        \return Returns the configID of this WUPSConfigItem.
    **/
    virtual std::string getConfigID() {
        return this->configID;
    }

    /**
        Returns a string that displays the current value.
        This string is shown next to the display name when the cursor is NOT on this item
    **/
    virtual std::string getCurrentValueDisplay() = 0;

    /**
        Returns a string that displays the current value when selected.
        This string is shown next to the display name when the cursor IS on this item
    **/
    virtual std::string getCurrentValueSelectedDisplay() = 0;

    /**
        Is called when the cursor enters or leaves the item.
        When the cursor enters the item, "isSelected" will be true.
        When the cursor leaves the item, "isSelected" will be false.
    **/
    virtual void onSelected(bool isSelected) = 0;

    /**
        Is called when a button is pressed while the cursor on this item.
        See the WUPSConfigButtons enum for possible values.
    **/
    virtual void onButtonPressed(WUPSConfigButtons buttons) = 0;

    /**
        When the cursor is on this item, the configuration menu asks this item
        if it's allowed to leave it.
        If it returns true, the item can be leaved.
        It it returns false, leaves is not allowed.
    **/
    virtual bool isMovementAllowed() = 0;

    /**
        Returns a string that represents the value of this item.
    **/
    virtual std::string persistValue() = 0;

    /**
        Loads a value for a given string that contains the persisted value.
        \param persistedValue A valued that the result of the persistValue function
    **/
    virtual void loadValue(std::string persistedValue) = 0;

    /**
        Restores the default value
    **/
    virtual void restoreDefault() = 0;


    /**
        Call callback with with current value.
        This function will be called whenever this item should call it's (optional) given
        callback with the current value.
        Returns true if a valid callback could be called
        Returns false if no callback was called (e.g. callback was NULL)
    **/
    virtual bool callCallback() = 0;

    WUPSConfigItem(std::string configID, std::string displayName){
        this->configID = configID;
        this->displayName = displayName;
    }

    virtual ~WUPSConfigItem(){
    }

private:
    std::string displayName;

    std::string configID;
};

#endif
