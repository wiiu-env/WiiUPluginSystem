#include <coreinit/filesystem.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <malloc.h>
#include <string.h>
#include <utils/logger.h>
#include <whb/libmanager.h>
#include <whb/log_cafe.h>
#include <whb/log_module.h>
#include <whb/log_udp.h>
#include <wups.h>
#include <wups/config/WUPSConfigItemBoolean.h>

/**
    Mandatory plugin information.
    If not set correctly, the loader will refuse to use the plugin.
**/
WUPS_PLUGIN_NAME("Example plugin");
WUPS_PLUGIN_DESCRIPTION("This is just an example plugin and will log the FSOpenFile function.");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("BSD");

/**
    All of this defines can be used in ANY file.
    It's possible to split it up into multiple files.

**/

WUPS_USE_WUT_DEVOPTAB();            // Use the wut devoptabs
WUPS_USE_STORAGE("example_plugin"); // Unqiue id for the storage api

bool logFSOpen = true;

/**
    Get's called ONCE when the loader exits, but BEFORE the ON_APPLICATION_START gets called or functions are overridden.
**/
INITIALIZE_PLUGIN() {
    if (!WHBLogModuleInit()) {
        WHBLogCafeInit();
        WHBLogUdpInit();
    }
    DEBUG_FUNCTION_LINE("INITIALIZE_PLUGIN of example_plugin!");

    // Open storage to read values
    WUPSStorageError storageRes = WUPS_OpenStorage();
    if (storageRes != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to open storage %s (%d)",
                            WUPS_GetStorageStatusStr(storageRes), storageRes);
    } else {
        // Try to get value from storage
        if ((storageRes = WUPS_GetBool(nullptr, "logFSOpen", &logFSOpen)) == WUPS_STORAGE_ERROR_NOT_FOUND) {
            // Add the value to the storage if it's missing.
            if (WUPS_StoreBool(nullptr, "logFSOpen", logFSOpen) != WUPS_STORAGE_ERROR_SUCCESS) {
                DEBUG_FUNCTION_LINE("Failed to store bool");
            }
        } else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS) {
            DEBUG_FUNCTION_LINE("Failed to get bool %s (%d)", WUPS_GetStorageStatusStr(storageRes), storageRes);
        }

        // Close storage
        if (WUPS_CloseStorage() != WUPS_STORAGE_ERROR_SUCCESS) {
            DEBUG_FUNCTION_LINE("Failed to close storage");
        }
    }
}

/**
    Gets called when the plugin loader is re-entered => when the plugin is unloaded. 
	The overridden functions are restored before this is getting called.
**/
DEINITIALIZE_PLUGIN() {
    DEBUG_FUNCTION_LINE("DEINITIALIZE_PLUGIN of example_plugin!");
}

/**
    Gets called when an application starts.
	This is called BEFORE the functions are overridden.
	Make sure to initialize all functions you're using in the overridden functions!
**/
ON_APPLICATION_START() {
    if (!WHBLogModuleInit()) {
        WHBLogCafeInit();
        WHBLogUdpInit();
    }

    DEBUG_FUNCTION_LINE("ON_APPLICATION_START of example_plugin!");
}

/**
    Gets called when an application request to exit.
**/
ON_APPLICATION_REQUESTS_EXIT() {
    DEBUG_FUNCTION_LINE("ON_APPLICATION_REQUESTS_EXIT of example_plugin!");
}

void logFSOpenChanged(ConfigItemBoolean *item, bool newValue) {
    DEBUG_FUNCTION_LINE("New value in logFSOpenChanged: %d", newValue);
    logFSOpen = newValue;
    // If the value has changed, we store it in the storage.
    WUPS_StoreInt(nullptr, "logFSOpen", logFSOpen);
}

WUPS_GET_CONFIG() {
    // We open the storage so we can persist the configuration the user did.
    if (WUPS_OpenStorage() != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to open storage");
        return 0;
    }

    WUPSConfigHandle config;
    WUPSConfig_CreateHandled(&config, "Example Plugin");

    WUPSConfigCategoryHandle cat;
    WUPSConfig_AddCategoryByNameHandled(config, "Logging", &cat);

    WUPSConfigItemBoolean_AddToCategoryHandled(config, cat, "logFSOpen", "Log FSOpen calls", logFSOpen, &logFSOpenChanged);

    return config;
}

WUPS_CONFIG_CLOSED() {
    // Save all changes
    if (WUPS_CloseStorage() != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to close storage");
    }
}

/**
    This defines a function replacement.
    It allows to replace the system function with an own function. 
    So whenever a game / application calles an overridden function, your function gets called instead.
    
    Currently it's only possible to override functions that are loaded from .rpl files of OSv10 (00050010-1000400A).
    
    Signature of this macro:
    DECL_FUNCTION(  RETURN_TYPE, ARBITRARY_NAME_OF_FUNCTION , ARGS_SEPERATED_BY_COMMA){
        //Your code goes here.
    }
    
    Within this macro, two more function get declare you can use.
    my_ARBITRARY_NAME_OF_FUNCTION and real_FSOpenFile
    
    RETURN_TYPE my_ARBITRARY_NAME_OF_FUNCTION(ARGS_SEPERATED_BY_COMMA):
        is just name of the function that gets declared in this macro. 
        It has the same effect as calling the overridden function directly.
    
    RETURN_TYPE real_ARBITRARY_NAME_OF_FUNCTION(ARGS_SEPERATED_BY_COMMA):
        is the name of the function, that leads to function that was overridden.
        Use this to call the original function that will be overridden.
        CAUTION: Other plugins may already have manipulated the the return value or arguments.
        
    
    Use this macro for each function you want to override

**/
DECL_FUNCTION(int, FSOpenFile, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error) {
    int result = real_FSOpenFile(pClient, pCmd, path, mode, handle, error);
    if (logFSOpen) {
        DEBUG_FUNCTION_LINE("FSOpenFile called for folder %s! Result %d", path, result);
    }
    return result;
}

/**
This tells the loader which functions from which library (.rpl) should be replaced with which function from this file.
The list of possible libraries can be found in the wiki. (In general it's WUPS_LOADER_LIBRARY_ + the name of the RPL in caps lock)

WUPS_MUST_REPLACE(FUNCTION_NAME_IN_THIS_FILE,   NAME_OF_LIB_WHICH_CONTAINS_THIS_FUNCTION,   NAME_OF_FUNCTION_TO_OVERRIDE)

Define this for each function you want to override.
**/
WUPS_MUST_REPLACE(FSOpenFile, WUPS_LOADER_LIBRARY_COREINIT, FSOpenFile);