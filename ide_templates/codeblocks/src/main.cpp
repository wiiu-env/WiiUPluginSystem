#include <wups.h>
#include <malloc.h>
#include <string.h>

/**
    https://github.com/Maschell/WiiUPluginSystem/wiki/How-to-develope-a-new-plugin#meta-information
**/

// Mandatory plugin information.
WUPS_PLUGIN_NAME("Plugin template");
WUPS_PLUGIN_DESCRIPTION("Description");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("(USER)NAME");
WUPS_PLUGIN_LICENSE("BSD");

/**
    Hooks:
    https://github.com/Maschell/WiiUPluginSystem/wiki/Using-hooks
**/

/**

WUPS_USE_WUT_MALLOC()       // Use the wut malloc wrapper
WUPS_USE_WUT_NEWLIB()       // Use serveral function implementations
WUPS_USE_WUT_DEVOPTAB()     // Use wut devoptab for SD access
WUPS_USE_WUT_STDCPP()       // Use wut cpp wrappers

WUPS_USE_WUT_CRT()          // Use all of them

**/

WUPS_USE_WUT_MALLOC()       // Use the wut malloc wrapper

// Gets called once when the loader exits.
INITIALIZE_PLUGIN(){
}

// Called whenever an application was started.
ON_APPLICATION_START(){
}

// Called whenever functions where patched by the loader
ON_FUNCTIONS_PATCHED(){
}

// Get called when ever GX2_VSYNC() was called (on each frame)
ON_VYSNC(){
}
// Called whenever an application is ending
ON_APPLICATION_END(){
}

// Gets called once when the loader is loaded again at the plugins will be unloaded
DEINITIALIZE_PLUGIN(){
}

/**
    Function replacing.
    https://github.com/Maschell/WiiUPluginSystem/wiki/Function-replacing
**/

// Lets replace OSIsHomeButtonMenuEnabled to allows the home button anywhere. (Not a great idea.)
DECL_FUNCTION(bool, OSIsHomeButtonMenuEnabled) {
    return true;
}

// Replace it.
WUPS_MUST_REPLACE(OSIsHomeButtonMenuEnabled,                   WUPS_LOADER_LIBRARY_COREINIT,               OSIsHomeButtonMenuEnabled);