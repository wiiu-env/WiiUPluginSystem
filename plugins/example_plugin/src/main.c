#include "utils/logger.h"
#include <coreinit/filesystem.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <wups.h>
#include <wups/button_combo/api.h>
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

bool logFSOpen                                                                = true;
static WUPSButtonCombo_ComboHandle sPressDownButtonComboExampleHandle         = {};
static WUPSButtonCombo_ComboHandle sPressDownObserverButtonComboExampleHandle = {};
static WUPSButtonCombo_ComboHandle sHoldButtonComboExampleHandle              = {};
static WUPSButtonCombo_ComboHandle sHoldObserverExButtonComboExampleHandle    = {};
WUPSButtonCombo_Buttons DEFAULT_PRESS_DOWN_BUTTON_COMBO                       = WUPS_BUTTON_COMBO_BUTTON_L | WUPS_BUTTON_COMBO_BUTTON_R;
WUPSButtonCombo_Buttons DEFAULT_PRESS_HOLD_COMBO                              = WUPS_BUTTON_COMBO_BUTTON_L | WUPS_BUTTON_COMBO_BUTTON_R | WUPS_BUTTON_COMBO_BUTTON_DOWN;

/**
 * Callback that will be called if the config has been changed
 */
void logFSOpenChanged(ConfigItemBoolean *item, bool newValue) {
    DEBUG_FUNCTION_LINE_INFO("New value in logFSOpenChanged: %d", newValue);
    logFSOpen = newValue;
    // If the value has changed, we store it in the storage.
    WUPSStorageAPI_StoreBool(NULL, LOG_FS_OPEN_CONFIG_ID, logFSOpen);
}

WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle root) {
    {
        // Let's create a new category called "Settings"
        WUPSConfigCategoryHandle settingsCategory;
        WUPSConfigAPICreateCategoryOptionsV1 settingsCategoryOptions = {.name = "Settings"};
        if (WUPSConfigAPI_Category_Create(settingsCategoryOptions, &settingsCategory) !=
            WUPSCONFIG_API_RESULT_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to create settings category");
            return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
        }

        // Add a new item to this settings category
        if (WUPSConfigItemBoolean_AddToCategory(settingsCategory, LOG_FS_OPEN_CONFIG_ID, "Log FSOpen calls", true,
                                                logFSOpen, &logFSOpenChanged) != WUPSCONFIG_API_RESULT_SUCCESS) {
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
        if (WUPSConfigItemBoolean_AddToCategory(categoryLevel2, "stubInsideCategory",
                                                "This is stub item inside a nested category", false, false,
                                                NULL) != WUPSCONFIG_API_RESULT_SUCCESS) {
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
        if (WUPSConfigItemStub_AddToCategory(root, "This is stub item without category") !=
            WUPSCONFIG_API_RESULT_SUCCESS) {
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
        WUPSConfigAPIStatus multValuesRes = WUPSConfigItemMultipleValues_AddToCategory(
                root, "multival", "Multiple values", 0, 0, values, numOfElements, NULL);
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
    WUPSStorageAPI_SaveStorage(false);
}

void pressDownComboCallback(const WUPSButtonCombo_ControllerTypes triggeredBy, WUPSButtonCombo_ComboHandle, void *) {
    DEBUG_FUNCTION_LINE_INFO("Button combo has been pressed down by controller %s", WUPSButtonComboAPI_GetControllerTypeStr(triggeredBy));
}

void pressDownObserverComboCallback(const WUPSButtonCombo_ControllerTypes triggeredBy, WUPSButtonCombo_ComboHandle, void *) {
    DEBUG_FUNCTION_LINE_INFO("[OBSERVER] Button combo has been pressed down by controller %s", WUPSButtonComboAPI_GetControllerTypeStr(triggeredBy));
}

void holdComboCallback(const WUPSButtonCombo_ControllerTypes triggeredBy, WUPSButtonCombo_ComboHandle, void *) {
    DEBUG_FUNCTION_LINE_INFO("Button combo has been hold by controller %s", WUPSButtonComboAPI_GetControllerTypeStr(triggeredBy));
}

void holdObserverExComboCallback(const WUPSButtonCombo_ControllerTypes triggeredBy, WUPSButtonCombo_ComboHandle, void *) {
    DEBUG_FUNCTION_LINE_INFO("[OBSERVER] Button combo has been hold by controller %s", WUPSButtonComboAPI_GetControllerTypeStr(triggeredBy));
}

/**
    Gets called ONCE when the plugin was loaded.
**/
INITIALIZE_PLUGIN() {
    // Logging only works when compiled with `make DEBUG=1`. See the README for more information.
    initLogging();
    DEBUG_FUNCTION_LINE("INITIALIZE_PLUGIN of example_plugin!");

    WUPSConfigAPIOptionsV1 configOptions = {.name = "example_plugin"};
    if (WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback) !=
        WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("Failed to init config api");
    }

    WUPSStorageError storageRes;
    // Try to get value from storage
    if ((storageRes = WUPSStorageAPI_GetBool(NULL, LOG_FS_OPEN_CONFIG_ID, &logFSOpen)) ==
        WUPS_STORAGE_ERROR_NOT_FOUND) {
        // Add the value to the storage if it's missing.
        if (WUPSStorageAPI_StoreBool(NULL, LOG_FS_OPEN_CONFIG_ID, logFSOpen) != WUPS_STORAGE_ERROR_SUCCESS) {
            DEBUG_FUNCTION_LINE_ERR("Failed to store bool");
        }
    } else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("Failed to get bool %s (%d)", WUPSConfigAPI_GetStatusStr(storageRes), storageRes);
    } else {
        DEBUG_FUNCTION_LINE_ERR("Successfully read the value from storage: %d %s (%d)", logFSOpen,
                                WUPSConfigAPI_GetStatusStr(storageRes), storageRes);
    }
    WUPSStorageAPI_SaveStorage(false);

    // To register a button combo, we can use the C++ wrapper class "WUPSButtonComboAPI::ButtonCombo".
    // The combo will be added on construction of that wrapper, and removed again in the destructor. Use `std::move` to move it around.
    // Like the C++ config api there are two versions of all function, one that throws an exception on error and one that returns a std::optional but set an additional error parameter.

    {
        WUPSButtonCombo_ComboStatus comboStatus = WUPS_BUTTON_COMBO_COMBO_STATUS_INVALID_STATUS;
        // Create a button combo which detects if a combo has been pressed down on any controller.
        // This version will check for conflicts. It's useful to check for conflicts if you want to use that button combo for a global unique thing
        // that's always possible, like taking screenshots.
        const WUPSButtonCombo_Error err = WUPSButtonComboAPI_AddButtonComboPressDown(
                "Example Plugin: Press Down test",
                DEFAULT_PRESS_DOWN_BUTTON_COMBO, // L + R
                pressDownComboCallback,
                NULL,
                &sPressDownButtonComboExampleHandle, // We will use the handle in the config menu
                &comboStatus);
        if (err == WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
            // On success, we can check if the combo is actually active by checking the combo status.
            // If there is already another combo that conflicts with us, the status will be set to WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT
            switch (comboStatus) {
                case WUPS_BUTTON_COMBO_COMBO_STATUS_VALID:
                    DEBUG_FUNCTION_LINE_INFO("Button combo is valid and active");
                    break;
                case WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT:
                    DEBUG_FUNCTION_LINE_INFO("Conflict detected for button combo");
                    break;
                default:
                    DEBUG_FUNCTION_LINE_ERR("Invalid combo status");
                    break;
            }
        } else {
            DEBUG_FUNCTION_LINE_ERR("Failed to add press down button combo");
        }
        // To remove that button combo, we explicitly have to call "WUPSButtonComboAPI_RemoveButtonCombo", we'll do it in DEINITIALIZE_PLUGIN
    }
    {
        // --------------------------------------------------------------------------------------------------------------------------------------------------

        // But we can also create button combos without caring about conflicts.
        // E.g. when a new Aroma update is detected, the updater can be launched by holding the PLUS button. This should always be possible.
        // If we don't want to check for conflicts, we need to create a "PressDownObserver"
        WUPSButtonCombo_ComboStatus comboStatus = WUPS_BUTTON_COMBO_COMBO_STATUS_INVALID_STATUS;
        const WUPSButtonCombo_Error err         = WUPSButtonComboAPI_AddButtonComboPressDownObserver(
                        "Example Plugin: Press Down observer test",
                        DEFAULT_PRESS_DOWN_BUTTON_COMBO, // L + R Even though this is same combo as in buttonComboPressDown an observer will ignore conflicts.
                        pressDownObserverComboCallback,
                        NULL,
                        &sPressDownObserverButtonComboExampleHandle,
                        &comboStatus); // comboStatus will always be WUPS_BUTTON_COMBO_COMBO_STATUS_VALID for observers.

        if (err == WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
            // To remove that button combo, we explicitly have to call "WUPSButtonComboAPI_RemoveButtonCombo", we'll do it in DEINITIALIZE_PLUGIN
        } else {
            DEBUG_FUNCTION_LINE_ERR("Failed to add press down observer button combo");
        }
    }

    {
        // --------------------------------------------------------------------------------------------------------------------------------------------------

        // In case of a conflict, the function will return SUCCESS, but the combo status will be WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT

        // Let's create a button combo which will lead to a conflict. This time we want to check if a combo has been hold for 500ms. Conflicts are checked across
        // non-observer combo types.
        WUPSButtonCombo_ComboStatus comboStatus = WUPS_BUTTON_COMBO_COMBO_STATUS_INVALID_STATUS;
        WUPSButtonCombo_Error err               = WUPSButtonComboAPI_AddButtonComboHold(
                              "Example Plugin: Hold test",
                              DEFAULT_PRESS_HOLD_COMBO, // L+R+DPAD+DOWN. This combo includes the combo "L+R" of the buttonComboPressDown, so this will lead to a conflict.
                              500,                      // We need to hold that combo for 500ms
                              holdComboCallback,
                              NULL,
                              &sHoldButtonComboExampleHandle,
                              &comboStatus); // comboStatus will always be WUPS_BUTTON_COMBO_COMBO_STATUS_VALID for observers.

        if (err == WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
            // API returned "WUPS_BUTTON_COMBO_ERROR_SUCCESS", but we have a conflict because of the existing press down combo.
            switch (comboStatus) {
                case WUPS_BUTTON_COMBO_COMBO_STATUS_VALID:
                    DEBUG_FUNCTION_LINE_INFO("Button combo is valid and active");
                    break;
                case WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT:
                    DEBUG_FUNCTION_LINE_INFO("Conflict detected for button combo"); // <-- this is expected to happen
                    break;
                default:
                    DEBUG_FUNCTION_LINE_ERR("Invalid combo status");
                    break;
            }

            // Once combo is in the "WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT" state it can only be valid again, if the button combo or the controllerMask changes.
            // Other combos won't ever affect this state of this combo
            // We can easily update the button combo
            err = WUPSButtonComboAPI_UpdateButtonCombo(
                    sHoldButtonComboExampleHandle,
                    WUPS_BUTTON_COMBO_BUTTON_ZR | WUPS_BUTTON_COMBO_BUTTON_R | WUPS_BUTTON_COMBO_BUTTON_DOWN,
                    &comboStatus);
            if (err == WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
                DEBUG_FUNCTION_LINE_INFO("Updated button combo");
                // Check the comboStatus after updating the combo
                switch (comboStatus) {
                    case WUPS_BUTTON_COMBO_COMBO_STATUS_VALID:
                        DEBUG_FUNCTION_LINE_INFO("Button combo is valid and active"); // <-- this is expected to happen
                        break;
                    case WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT:
                        DEBUG_FUNCTION_LINE_INFO("Conflict detected for button combo");
                        break;
                    default:
                        DEBUG_FUNCTION_LINE_ERR("Invalid combo status");
                        break;
                }
            } else {
                DEBUG_FUNCTION_LINE_INFO("Failed to update button combo");
            }
            // To remove that button combo, we explicitly have to call "WUPSButtonComboAPI_RemoveButtonCombo", we'll do it in DEINITIALIZE_PLUGIN
        } else {
            DEBUG_FUNCTION_LINE_ERR("Failed to add press down observer button combo");
        }
    }
    {
        // --------------------------------------------------------------------------------------------------------------------------------------------------

        // To register a combo for just one controller, we can't use the helper function we're using above.
        // We have fill in the options instead:
        WUPSButtonCombo_ComboOptions options;
        options.metaOptions.label        = "Combo for WPAD_0";          // can be NULL
        options.callbackOptions.callback = holdObserverExComboCallback; // must not be NULL
        options.callbackOptions.context  = NULL;                        // can be NULL
        // We want to a "hold" combination where we have to hold for 100ms. Let's create an observer to not care about conflicts for this example plugin.
        options.buttonComboOptions.type               = WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD_OBSERVER;
        options.buttonComboOptions.optionalHoldForXMs = 100; // <-- will be ignored if the type is WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN*

        // Defines which button the combo is using and which controllers should be checked
        options.buttonComboOptions.basicCombo.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0;                     // We check for WPAD_0, but we could also do something like (WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0 | WUPS_BUTTON_COMBO_CONTROLLER_VPAD_0)
        options.buttonComboOptions.basicCombo.combo          = WUPS_BUTTON_COMBO_BUTTON_A | WUPS_BUTTON_COMBO_BUTTON_B; // <-- will be ignored if the type is WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN*

        WUPSButtonCombo_ComboStatus comboStatus = WUPS_BUTTON_COMBO_COMBO_STATUS_INVALID_STATUS;
        const WUPSButtonCombo_Error err         = WUPSButtonComboAPI_AddButtonCombo(&options,
                                                                                    &sHoldObserverExButtonComboExampleHandle,
                                                                                    &comboStatus); // comboStatus will always be WUPS_BUTTON_COMBO_COMBO_STATUS_VALID for observers.

        if (err == WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
            // To remove that button combo, we explicitly have to call "WUPSButtonComboAPI_RemoveButtonCombo", we'll do it in DEINITIALIZE_PLUGIN
        } else {
            DEBUG_FUNCTION_LINE_ERR("Failed to add hold observer button combo for WPAD_0");
        }
    }

    deinitLogging();
}

/**
    Gets called when the plugin will be unloaded.
**/
DEINITIALIZE_PLUGIN() {
    DEBUG_FUNCTION_LINE("DEINITIALIZE_PLUGIN of example_plugin!");
    WUPSButtonComboAPI_RemoveButtonCombo(sPressDownButtonComboExampleHandle);
    WUPSButtonComboAPI_RemoveButtonCombo(sPressDownObserverButtonComboExampleHandle);
    WUPSButtonComboAPI_RemoveButtonCombo(sHoldButtonComboExampleHandle);
    WUPSButtonComboAPI_RemoveButtonCombo(sHoldObserverExButtonComboExampleHandle);
    sPressDownButtonComboExampleHandle.handle         = NULL;
    sPressDownObserverButtonComboExampleHandle.handle = NULL;
    sHoldButtonComboExampleHandle.handle              = NULL;
    sHoldObserverExButtonComboExampleHandle.handle    = NULL;
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
DECL_FUNCTION(int, FSOpenFile, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle,
              int error) {
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
