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

// FS Access (replaces open/close/write/read etc. functions)
WUPS_FS_ACCESS()
// Overlay access
//WUPS_ALLOW_OVERLAY()

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

// Gets called whenever the status of the running application changes.
ON_APP_STATUS_CHANGED(status){
}

// Called whenever an application is ending
ON_APPLICATION_ENDING(){
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
