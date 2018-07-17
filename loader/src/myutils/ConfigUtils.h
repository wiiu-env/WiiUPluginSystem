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
#ifndef __CONFIG_UTILS_H_
#define __CONFIG_UTILS_H_

#include <wups.h>
#include <wups/config/WUPSConfig.h>
#include "ConfigInformation.h"

class ConfigUtils {

public:
    /**
        Loads the configuration files of all loaded plugins from the SDCard
        and triggers the "loadValue" if they differ the default value.
    **/
    static void loadConfigFromSD();

    /**
        Opens the configuration menu where plugins can be configured.
        Plugins need to implement the WUPS_GET_CONFIG() hook to show up in the menu.
        The menu will be rendered on the TV and DRC screen, with optimization for the DRC.
        If the menu is low, the menu may be only rendered to the DRC.
    **/
    static void openConfigMenu();

private:
    ConfigUtils() {}
    ~ConfigUtils() {}

    /**
        Opens the configuration menu for a list of WUPSConfig.
        If a configuration menu context could be created successfully, "configMenuOpenedCallback"
        will be called, with the configs as an argument.
    **/
    static void openConfigMenu(std::vector<WUPSConfig*> configs);

    /**
        The callback that renders the configuration menu.
        The args parameter expects a "std::vector<WUPSConfig*> *".
        Depending on the screen
    **/
    static void configMenuOpenedCallback(wups_overlay_options_type_t screen, void * args);

    /**
        Gets the ConfigInformation for all loaded plugin that have implemented
        the WUPS_GET_CONFIG() hook.

        This triggers creating ConfigInformation objects.
        ConfigInformation objects load the corresponding configuration file for a plugin
        from the sd card. It also calls the "loadValue" function if a loaded value differs from the
        default value. This behaviour may change in the future.
        See the ConfigInformation class for more information.
    **/
    static std::vector<ConfigInformation *> getConfigInformation();

    /**
        Delete a list of ConfigInformation.
        The destructor of "ConfigInformation" causes the configuration files for the plugin
        to be written to the sd card (if the value have changed). This behaviour may change
        in the future. See the ConfigInformation class for more information.
    **/
    static void deleteConfigInformation(std::vector<ConfigInformation *> configs);

};

#endif
