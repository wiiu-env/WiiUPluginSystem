---
title: Plugin creation overview
tags: 
keywords: plugin creation, overview, templates, meta information
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_plugin_creation_overview.html
folder: dev
topnav: topnav
---

## Requirements:

Before you can develop a plugin, you need to set up your environment:  
* **Required:** [Setting up the Toolchain](dev_toolchain_setup)
* **Required:** [Installing the required libraries](dev_required_libraries)
* _Optional:_   [Setting up an IDE](dev_ide_setup)

## Templates /  Examples

An example plugin can be found [here](https://github.com/Maschell/WiiUPluginSystem/tree/master/plugins/example_plugin).
A template for Code::Blocks can be found [here](dev_project_template_in_code_blocks).

Both ways provide a Makefile that is compatible with the plugin system.

The [WUPSPluginPlayground](https://github.com/Maschell/WUPSPluginPlayground) provides some simple plugins which can be used for reference. 
For most features of a plugin can use there is a corresponding "testing" application.

## Developing the plugin
This part provides all information you need for developing a new plugin.
### Meta information
Multiple macros will define the meta information of the plugin. These are **mandatory**. 
All macros take a string as an argument.   

Here is an overview over all meta information macros:  

|  Macro | Description |
|---|---|
| WUPS_PLUGIN_NAME | Name of the plugin |
| WUPS_PLUGIN_DESCRIPTION  | Short description of the plugin |
| WUPS_PLUGIN_VERSION  | Plugin version |
| WUPS_PLUGIN_AUTHOR | Author(s) of the plugin |
| WUPS_PLUGIN_LICENSE | The license of the plugin |

The plugin also stores some "hidden" meta information that will be generated automatically. More information can be found: here (TODO)

### Makefile
More information on adjusting the Makefile can be found here:  
https://github.com/Maschell/WiiUPluginSystem/wiki/Plugin-Makefile

### Use system libraries

System libraries are linked dynamically with each application and we can use them too in homebrew.
A small guide on how to use them can be found [here](dev_using_system_functions)

### Inherit functions from the plugin loader.
In order to gain access to the file system (e.g. sd card), use overlays or get kernel access you have to add certain macros to your plugin.  
Details can be found [here](dev_inherit_features)

### Hooks
The plugin can set up several hooks which will be called by the plugin loader.  
Details can be found [here](dev_plugin_hooks)

### Function replacing
The plugin provides an easy way to override existing SDK functions. Detailed information can  be found here:  
Details can be found [here](dev_function_replacing)

# Simple example
This is one of the simplest plugin possible:  
``` C
#include <wups.h>
#include <nsysnet/socket.h>
#include <utils/logger.h>
#include <coreinit/filesystem.h>

// Mandatory plugin information.
WUPS_PLUGIN_NAME("Plugin template");
WUPS_PLUGIN_DESCRIPTION("Description");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("USERNAME");
WUPS_PLUGIN_LICENSE("BSD");

// Gets called once the loader exists.
ON_APPLICATION_START(){
    socket_lib_init();
    log_init();
    DEBUG_FUNCTION_LINE("ON_APPLICATION_PLUGIN\n");
}

// Lets replace FSOpenFile
DECL_FUNCTION(int, FSOpenFile, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error) {
    int result = real_FSOpenFile(pClient, pCmd, path, mode, handle, error);

    log_printf("FSOpenFile called for folder %s! Result %d\n",path,result);
    return result;
}

// Replace it.
WUPS_MUST_REPLACE(FSOpenFile, WUPS_LOADER_LIBRARY_COREINIT, FSOpenFile);
```
This code does several things. 
At first it includes the wups header `<wups.h>` which is needed to use any WUPS related functions / macros, `<utils/logger.h>` is from libutils and helps logging. 
The other includes come from wut and will let us use the system libraries. After that, the meta information are set. 
The `ON_APPLICATION_START` hook is used to log a short messages every time an application was started. 
At the end the `FSOpenFile` function of the `coreinit.rpl` is replaced.
A example plugin with detailed comments can be found here (requires [wut](https://github.com/decaf-emu/wut)):  
https://github.com/Maschell/WiiUPluginSystem/blob/master/plugins/example_plugin/src/main.cpp

{% include links.html %}
