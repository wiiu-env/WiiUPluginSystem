---
title: Register to the configuration menu
tags: [features]
keywords: configuration, settings, menu, overlay, config
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_configuration_menu.html
folder: dev
topnav: topnav
---

Plugins can register add their configuration to the configuration menu. This menu can be opened during gameplay to configure plugins.

## Register configuration
To register a configuration to the plugin loader, the plugin need use the `WUPS_GET_CONFIG` macro. **This macro does only work in a C++ Context**.  
Usage:
```C++
#include <wups/config.h>

WUPS_GET_CONFIG() {
    WUPSConfig* config = new WUPSConfig("Example plugin");

    // setting up the the config

    return config;
}
```
The macro needs to return a **heap pointer** to a [WUPSConfig](https://github.com/Maschell/WiiUPluginSystem/blob/master/wups_include/wups/config/WUPSConfig.h) object. The loader will take of deleting the object.

### Structure of a `WUPSConfig`
A `WUPSConfig` is the top-level object which contains all information needed to created a configuration menu for a plugin. It contains __categories__ which themself contain the actual __items__. The structure is illustrated in the following:
```
WUPSConfig ("HID to VPAD")
    |
    |--> WUPSConfigCategory ("Main")
    |            |
    |            |--> WUPSConfigItem ("Enable rumble")
    |            |--> WUPSConfigItem ("Enable network client")
    |
    |--> WUPSConfigCategory ("Controller Mapping")
                 |
                 |--> WUPSConfigItem ("Gamepad mapping")
                 |--> WUPSConfigItem ("Pro controller 1 mapping")
   [...]
```
A detailed documentation of all functions of a `WUPSConfig` can be found in the [header](https://github.com/Maschell/WiiUPluginSystem/blob/master/wups_include/wups/config/WUPSConfig.h).  

### Adding a category - WUPSConfigCategory
As said before, the `WUPSConfig` contains one or more categories. To create a category, you simply use the `WUPSConfigCategory * addCategory(std::string categoryName)` on a `WUPSConfig` object.
```C++
#include <wups/config.h>

WUPS_GET_CONFIG() {
    WUPSConfig* config = new WUPSConfig("Example plugin");
    //Create a new category
    WUPSConfigCategory* catOther = config->addCategory("Other");
    // setting up the the config

    return config;
}
```
It's also possible to add an existing `WUPSConfigCategory` using `WUPSConfigCategory * addCategory(WUPSConfigCategory * category)`. A detailed documentation of all functions can be found in the 
[header](https://github.com/Maschell/WiiUPluginSystem/blob/master/wups_include/wups/config/WUPSConfigCategory.h).  
You don't have to take care of deleting the created `WUPSConfigCategory`, the plugin loader will do that for you.

### Adding an item - WUPSConfigItem
Each category can hold multiple items. These items need be inhereted from the abstract `WUPSConfigItem` class.  
WUPS provide serveral implementations that can be used, but it's also possible to use an own implementation. See the [WUPSConfigItem](https://github.com/Maschell/WiiUPluginSystem/blob/master/wups_include/wups/config/WUPSConfigItem.h) class for details on the functions that need to be implemented.
Example usage using a boolean item (`WUPSConfigItemBoolean`, [provided by WUPS](https://github.com/Maschell/WiiUPluginSystem/blob/master/wups_include/wups/config/WUPSConfigItemBoolean.h)):
```C++
#include <wups/config.h>

// callback that will be called when the value changes.
void rumbleConfigChanged(bool newValue) {
    DEBUG_FUNCTION_LINE("New rumble value %d\n",newValue);
}

WUPS_GET_CONFIG() {
    WUPSConfig* config = new WUPSConfig("Example plugin");
    //Create a new category
    WUPSConfigCategory* catOther = config->addCategory("Other");
    
    // Add a boolean item to the category
    //                item Type                | config id | displayed name | default value | onChangeCallback.
    catOther->addItem(new WUPSConfigItemBoolean("rumble", "Enable rumble", true,              rumbleConfigChanged));
    
    return config;
}
```
You don't have to take care of deleting the created `WUPSConfigItem`, the plugin loader will do that for you.

## Automatic save/load of configurations
The Wii U Plugin System tries automatically persist the configurations to the SD Card. It created configuration files next to the plugins. When you have for example a `sd:/wiiu/plugins/hidtovpad.wups`, a 
`sd:/wiiu/plugins/hidtovpad.cfg` will be created.  
Everytime you start or switch an application, this configuration will be read from the sd card. In case the loaded value is different from the default on, the "changed"-callback of an item will be called. 
**If possible, sure to always pass the current value as default value**. Otherwise the callback will be called more than nessecary (each time you switch an application or open the menu).

Everytime the configuration menu is opened, the configuration from the sd card will be read. Everytime you close it, the changes will be written to the sd card.  
To persist a configuration, each item has an "configID". Using this, the values can be saved as key/value pairs, where the key is the configID. The value is provided by the `WUPSConfigItem` implementation, 
which need to implement a `std::string persist()` and `void loadValue(std::string persistedValue)` function. See the [WUPSConfigItem](https://github.com/Maschell/WiiUPluginSystem/blob/master/wups_include/wups/config/WUPSConfigItem.h) class for more information.

The idea is to have the following workflow.
* Choose a default value for a configuration item in case no configuration was saved before.
* Save it in a variable which lays in the .data section. This allows the variable to keep the value even when the  running application switches.
* Update this variable inside the valueChanged callback.
* Use this value to initalize the `WUPSConfigItem`

Example using the `WUPSConfigItemBoolean`:

```C++
#include <wups/config.h>
#include <wups/config/WUPSConfigItemBoolean.h>

// Save a variable into the data section so it'll survive changing the apps.
bool gRumbleActive __attribute__((section(".data"))) = false;

// callback that will be called when the value changes.
void rumbleConfigChanged(bool newValue) {
    DEBUG_FUNCTION_LINE("New rumble value %d\n",newValue);
    
    // Update
    gRumbleActive = newValue;
}

WUPS_GET_CONFIG() {
    WUPSConfig* config = new WUPSConfig("Example plugin");
    //Create a new category
    WUPSConfigCategory* catOther = config->addCategory("Other");
    
    // Add a boolean item to the category. As default value we choose the current value.
    //                item Type                | config id | displayed name | default value | onChangeCallback.
    catOther->addItem(new WUPSConfigItemBoolean("rumble", "Enable rumble",    gRumbleActive,              rumbleConfigChanged));
    
    return config;
}
```

## Provided `WUPSConfigItem` implementations
The plugin system provides some item implementations which can be used.

### WUPSConfigItemBoolean - simple boolean values
This item can be used for simple boolean values (e.g. on/off switches).

**Can be found in:**
`#include <wups/config/WUPSConfigItemBoolean.h>`

**Constructor:**
```C++
typedef void (*BooleanValueChangedCallback)(bool);

WUPSConfigItemBoolean(std::string configID, std::string displayName, bool defaultValue, BooleanValueChangedCallback callback);
```
* _configID_: 
  ID which is used to save the state of the item. Needs to be unique across the whole plugin config
* _displayName_:
  Text that will displayed to the describe this item on the configuration menu.
* _defaultValue_:
  The default value of the item.
* callback:
  Callback that will be called (with the new value) whenever the value changes. This is also the case when the _loaded value_ is different from the _default value_
  
**Notes:**  
On default the text for a "true"-vale is "on", and for a "false"-value if "off". You can change it by using `void setTrueValueName(std::string trueValName)` or 
`void setFalseValueName(std::string falseValName)` on a `WUPSConfigItemBoolean` object.

**Usage example:**
```C++

#include <wups.h>
#include <wups/config.h>
#include <wups/config/WUPSConfigItemBoolean.h>

// callback that will be called when the value changes.
void rumbleConfigChanged(bool newValue) {
    DEBUG_FUNCTION_LINE("New rumble value %d\n",newValue);
}

WUPS_GET_CONFIG() {
    WUPSConfig* config = new WUPSConfig("Example plugin");
    WUPSConfigCategory* catOther = config->addCategory("Other");
    
    // Add a boolean item to the category
    //                item Type                | config id | displayed name | default value | onChangeCallback.
    catOther->addItem(new WUPSConfigItemBoolean("rumble", "Enable rumble", true,              rumbleConfigChanged));
    
    return config;
}
```
See the class definition for more information:   
https://github.com/Maschell/WiiUPluginSystem/blob/master/wups_include/wups/config/WUPSConfigItemBoolean.h
  
### WUPSConfigItemMultipleValues - Multiple choice out of multiple labeled values
This item can be used to create a item which multiple, pre-defined values. Each value has it's own label (provided as value/label pairs). 

**Can be found in:**
`#include <wups/config/WUPSConfigItemMultipleValues.h>`

**Constructor:**
```C++
typedef void (*MultipleValuesChangedCallback)(int32_t);

WUPSConfigItemMultipleValues(std::string configID, std::string displayName, int32_t defaultValue, std::map<int32_t,std::string> values_, MultipleValuesChangedCallback callback);
```
* _configID_: 
  ID which is used to save the state of the item. Needs to be unique across the whole plugin config.
* _displayName_:
  Text that will displayed to the describe this item on the configuration menu.
* _defaultValue_:
  The default value of the item.
* _values_:
  Map which provides a list of possible values and their label.
* callback:
  Callback that will be called (with the new value) whenever the value changes. This is also the case when the _loaded value_ is different from the _default value_

**Notes:**  
If the _default value_ is not a part of the provided value list, the first element will be used as default value.

**Usage example:**
```C++
#include <wups.h>
#include <wups/config.h>
#include <wups/config/WUPSConfigItemMultipleValues.h>

#define RESOLUTION_240P 1
#define RESOLUTION_360P 2
#define RESOLUTION_480P 3

void resolutionChanged(int32_t newResolution) {
    DEBUG_FUNCTION_LINE("Resolution changed to value %d \n",newResolution);    
}

WUPS_GET_CONFIG() {
    WUPSConfig* config = new WUPSConfig("Streaming Plugin");
    WUPSConfigCategory* catOther = config->addCategory("Main");

    std::map<int32_t,std::string> resolutionValues;
    resolutionValues[RESOLUTION_240P] = "240p";
    resolutionValues[RESOLUTION_360P] = "360p";
    resolutionValues[RESOLUTION_480P] = "480p";

    //                    item Type                    | config id  | displayed name        | default value  onChangeCallback.
    catOther->addItem(new WUPSConfigItemMultipleValues("resolution", "Streaming resolution", RESOLUTION_240P, resolutionValues, resolutionChanged));

    return config;
}
```

See the class definition for more information:   
https://github.com/Maschell/WiiUPluginSystem/blob/master/wups_include/wups/config/WUPSConfigItemMultipleValues.h
  
{% include links.html %}
