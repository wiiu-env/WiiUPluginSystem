#include "utils/logger.h"
#include <coreinit/filesystem.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <wups.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <wups/config/WUPSConfigItemMultipleValues.h>
#include <wups/config/WUPSConfigItemStub.h>

/**
    Mandatory plugin information.
    If not set correctly, the loader will refuse to use the plugin.
**/
WUPS_PLUGIN_NAME("Example plugin");
WUPS_PLUGIN_DESCRIPTION("This is just an example plugin and will log the FSOpenFile function.");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("BSD");

#define LOG_FS_OPEN_CONFIG_ID "logFSOpen"

/**
    All of this defines can be used in ANY file.
    It's possible to split it up into multiple files.

**/

WUPS_USE_WUT_DEVOPTAB();            // Use the wut devoptabs
WUPS_USE_STORAGE("example_plugin"); // Unique id for the storage api

bool logFSOpen = true;

/**
 * Callback that will be called if the config has been changed
 */
void logFSOpenChanged(ConfigItemBoolean *item, bool newValue) {
    DEBUG_FUNCTION_LINE_INFO("New value in logFSOpenChanged: %d", newValue);
    logFSOpen = newValue;
    // If the value has changed, we store it in the storage.
    WUPS_StoreInt(NULL, LOG_FS_OPEN_CONFIG_ID, logFSOpen);
}

WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle root) {
    // We open the storage, so we can persist the configuration the user did.
    if (WUPS_OpenStorage() != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to open storage");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    {
        // Let's create a new category called "Settings"
        WUPSConfigCategoryHandle settingsCategory;
        WUPSConfigAPICreateCategoryOptionsV1 settingsCategoryOptions = {.name = "Settings"};
        if (WUPSConfigAPI_Category_Create(settingsCategoryOptions, &settingsCategory) != WUPSCONFIG_API_RESULT_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to create settings category");
            return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
        }

        // Add a new item to this settings category
        if (WUPSConfigItemBoolean_AddToCategory(settingsCategory, LOG_FS_OPEN_CONFIG_ID, "Log FSOpen calls", true, logFSOpen, &logFSOpenChanged) != WUPSCONFIG_API_RESULT_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to add item to category");
            return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
        }

        // Add the category to the root.
        if (WUPSConfigAPI_Category_AddCategory(root, settingsCategory) != WUPSCONFIG_API_RESULT_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to add category to root item");
        }
    }

    {
        // We can also have categories inside categories!
        WUPSConfigCategoryHandle categoryLevel1;
        WUPSConfigAPICreateCategoryOptionsV1 catLev1Options = {.name = "Category with subcategory"};
        if (WUPSConfigAPI_Category_Create(catLev1Options, &categoryLevel1) != WUPSCONFIG_API_RESULT_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to create categoryLevel1");
            return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
        }
        WUPSConfigCategoryHandle categoryLevel2;
        WUPSConfigAPICreateCategoryOptionsV1 catLev2Options = {.name = "Category inside category"};
        if (WUPSConfigAPI_Category_Create(catLev2Options, &categoryLevel2) != WUPSCONFIG_API_RESULT_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to create categoryLevel1");
            return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
        }
        if (WUPSConfigItemBoolean_AddToCategory(categoryLevel2, "stubInsideCategory", "This is stub item inside a nested category", false, false, NULL) != WUPSCONFIG_API_RESULT_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to add stub item to root category");
            return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
        }

        // add categoryLevel2 to categoryLevel1
        if (WUPSConfigAPI_Category_AddCategory(categoryLevel1, categoryLevel2) != WUPSCONFIG_API_RESULT_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to add category to root item");
            return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
        }

        // add categoryLevel2 to categoryLevel1
        if (WUPSConfigAPI_Category_AddCategory(root, categoryLevel1) != WUPSCONFIG_API_RESULT_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to add category to root item");
            return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
        }
    }
    {
        // We can also directly add items to the root category
        if (WUPSConfigItemStub_AddToCategory(root, "This is stub item without category") != WUPSCONFIG_API_RESULT_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to add stub item to root category");
            return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
        }
        ConfigItemMultipleValuesPair values[10];
        int numOfElements = sizeof(values) / sizeof(values[0]);
        for (int i = 0; i < numOfElements; i++) {
#define STR_SIZE 10
            char *str = (char *) malloc(STR_SIZE);
            if (!str) {
                OSFatal("Failed to allocate memory");
            }
            snprintf(str, STR_SIZE, "%d", i);
            values[i].value     = i;
            values[i].valueName = str;
        }
        WUPSConfigAPIStatus multValuesRes = WUPSConfigItemMultipleValues_AddToCategory(root, "multival", "Multiple values", 0, 0, values, numOfElements, NULL);
        for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
            free((void *) values[i].valueName);
        }
        if (multValuesRes != WUPSCONFIG_API_RESULT_SUCCESS) {
            return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
        }
    }

    return WUPSCONFIG_API_CALLBACK_RESULT_SUCCESS;
}

void ConfigMenuClosedCallback() {
    // Save all changes
    if (WUPS_CloseStorage() != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("Failed to close storage");
    }
}

/**
    Gets called ONCE when the plugin was loaded.
**/
INITIALIZE_PLUGIN() {
    // Logging only works when compiled with `make DEBUG=1`. See the README for more information.
    initLogging();
    DEBUG_FUNCTION_LINE("INITIALIZE_PLUGIN of example_plugin!");

    WUPSConfigAPIOptionsV1 configOptions = {.name = "example_plugin"};
    if (WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback) != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("Failed to init config api");
    }

    // Open storage to read values
    WUPSStorageError storageRes = WUPS_OpenStorage();
    if (storageRes != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to open storage %s (%d)", WUPS_GetStorageStatusStr(storageRes), storageRes);
    } else {
        // Try to get value from storage
        if ((storageRes = WUPS_GetBool(NULL, LOG_FS_OPEN_CONFIG_ID, &logFSOpen)) == WUPS_STORAGE_ERROR_NOT_FOUND) {
            // Add the value to the storage if it's missing.
            if (WUPS_StoreBool(NULL, LOG_FS_OPEN_CONFIG_ID, logFSOpen) != WUPS_STORAGE_ERROR_SUCCESS) {
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
    deinitLogging();
}

/**
    Gets called when the plugin will be unloaded.
**/
DEINITIALIZE_PLUGIN() {
    DEBUG_FUNCTION_LINE("DEINITIALIZE_PLUGIN of example_plugin!");
}

/**
    Gets called when an application starts.
**/
ON_APPLICATION_START() {
    initLogging();

    DEBUG_FUNCTION_LINE("ON_APPLICATION_START of example_plugin!");
}

/**
 * Gets called when an application actually ends
 */
ON_APPLICATION_ENDS() {
    deinitLogging();
}

/**
    Gets called when an application request to exit.
**/
ON_APPLICATION_REQUESTS_EXIT() {
    DEBUG_FUNCTION_LINE_INFO("ON_APPLICATION_REQUESTS_EXIT of example_plugin!");
}

/**
    This defines a function replacement.
    It allows to replace the system function with an own function. 
    So whenever a game / application calls an overridden function, your function gets called instead.
    
    Currently it's only possible to override functions that are loaded from .rpl files of OSv10 (00050010-1000400A).
    
    Signature of this macro:
    DECL_FUNCTION(  RETURN_TYPE, ARBITRARY_NAME_OF_FUNCTION , ARGS_SEPERATED_BY_COMMA){
        //Your code goes here.
    }
    
    Within this macro, two more function get declare you can use.
    my_ARBITRARY_NAME_OF_FUNCTION and real_ARBITRARY_NAME_OF_FUNCTION
    
    RETURN_TYPE my_ARBITRARY_NAME_OF_FUNCTION(ARGS_SEPERATED_BY_COMMA):
        is just name of the function that gets declared in this macro. 
        It has the same effect as calling the overridden function directly.
    
    RETURN_TYPE real_ARBITRARY_NAME_OF_FUNCTION(ARGS_SEPERATED_BY_COMMA):
        is the name of the function, that leads to function that was overridden.
        Use this to call the original function that will be overridden.
        CAUTION: Other plugins may already have manipulated the return value or arguments.

    Use this macro for each function you want to override
**/
DECL_FUNCTION(int, FSOpenFile, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error) {
    int result = real_FSOpenFile(pClient, pCmd, path, mode, handle, error);
    if (logFSOpen) {
        DEBUG_FUNCTION_LINE_INFO("FSOpenFile called for folder %s! Result %d", path, result);
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