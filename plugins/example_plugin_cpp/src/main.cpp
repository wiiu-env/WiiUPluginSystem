#include "utils/logger.h"
#include <coreinit/filesystem.h>
#include <wups.h>
#include <wups/button_combo/api.h>
#include <wups/config/WUPSConfigCategory.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <wups/config/WUPSConfigItemButtonCombo.h>
#include <wups/config/WUPSConfigItemIPAddress.h>
#include <wups/config/WUPSConfigItemIntegerRange.h>
#include <wups/config/WUPSConfigItemMultipleValues.h>
#include <wups/config/WUPSConfigItemStub.h>
#include <wups/config_api.h>

#include <forward_list>

#include <malloc.h>

/**
    Mandatory plugin information.
    If not set correctly, the loader will refuse to use the plugin.
**/
WUPS_PLUGIN_NAME("Example plugin C++");
WUPS_PLUGIN_DESCRIPTION("This is just an example plugin written in C++");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("BSD");

#define LOG_FS_OPEN_CONFIG_ID             "logFSOpen"
#define BUTTON_COMBO_PRESS_DOWN_CONFIG_ID "pressDownItem"
#define BUTTON_COMBO_HOLD_CONFIG_ID       "holdItem"
#define OTHER_EXAMPLE_BOOL_CONFIG_ID      "otherBoolItem"
#define OTHER_EXAMPLE2_BOOL_CONFIG_ID     "other2BoolItem"
#define INTEGER_RANGE_EXAMPLE_CONFIG_ID   "intRangeExample"
#define MULTIPLE_VALUES_EXAMPLE_CONFIG_ID "multValueExample"
#define IP_ADDRESS_CONFIG_ID              "ipAddressExample"

/**
    All of this defines can be used in ANY file.
    It's possible to split it up into multiple files.

**/

WUPS_USE_WUT_DEVOPTAB();                // Use the wut devoptabs
WUPS_USE_STORAGE("example_plugin_cpp"); // Unique id for the storage api

enum ExampleOptions {
    EXAMPLE_OPTION_1 = 0,
    EXAMPLE_OPTION_2 = 1,
    EXAMPLE_OPTION_3 = 2,
};

#define LOF_FS_OPEN_DEFAULT_VALUE     true
#define INTEGER_RANGE_DEFAULT_VALUE   10
#define MULTIPLE_VALUES_DEFAULT_VALUE EXAMPLE_OPTION_2
#define IP_ADDRESS_DEFAULT_VALUE      0x2a2a2a2a

bool sLogFSOpen                    = LOF_FS_OPEN_DEFAULT_VALUE;
int sIntegerRangeValue             = INTEGER_RANGE_DEFAULT_VALUE;
ExampleOptions sExampleOptionValue = MULTIPLE_VALUES_DEFAULT_VALUE;
uint32_t sIPAddress                = IP_ADDRESS_DEFAULT_VALUE;

static std::forward_list<WUPSButtonComboAPI::ButtonCombo> sButtonComboInstances;

static WUPSButtonCombo_ComboHandle sPressDownExampleHandle(nullptr);
static WUPSButtonCombo_ComboHandle sHoldExampleHandle(nullptr);

WUPSButtonCombo_Buttons DEFAULT_PRESS_DOWN_BUTTON_COMBO = WUPS_BUTTON_COMBO_BUTTON_L | WUPS_BUTTON_COMBO_BUTTON_R;
WUPSButtonCombo_Buttons DEFAULT_PRESS_HOLD_COMBO        = WUPS_BUTTON_COMBO_BUTTON_L | WUPS_BUTTON_COMBO_BUTTON_R | WUPS_BUTTON_COMBO_BUTTON_DOWN;

/**
 * Callback that will be called if the config has been changed
 */
void boolItemChanged(ConfigItemBoolean *item, bool newValue) {
    DEBUG_FUNCTION_LINE_INFO("New value in boolItemChanged: %d", newValue);
    if (std::string_view(LOG_FS_OPEN_CONFIG_ID) == item->identifier) {
        sLogFSOpen = newValue;
        // If the value has changed, we store it in the storage.
        WUPSStorageAPI::Store(item->identifier, newValue);
    } else if (std::string_view(OTHER_EXAMPLE_BOOL_CONFIG_ID) == item->identifier) {
        DEBUG_FUNCTION_LINE_ERR("Other bool value has changed to %d", newValue);
    } else if (std::string_view(OTHER_EXAMPLE2_BOOL_CONFIG_ID) == item->identifier) {
        DEBUG_FUNCTION_LINE_ERR("Other2 bool value has changed to %d", newValue);
    }
}

void integerRangeItemChanged(ConfigItemIntegerRange *item, int newValue) {
    DEBUG_FUNCTION_LINE_INFO("New value in integerRangeItemChanged: %d", newValue);
    // If the value has changed, we store it in the storage.
    if (std::string_view(LOG_FS_OPEN_CONFIG_ID) == item->identifier) {
        sIntegerRangeValue = newValue;
        // If the value has changed, we store it in the storage.
        WUPSStorageAPI::Store(item->identifier, newValue);
    }
}

void multipleValueItemChanged(ConfigItemIntegerRange *item, uint32_t newValue) {
    DEBUG_FUNCTION_LINE_INFO("New value in multipleValueItemChanged: %d", newValue);
    // If the value has changed, we store it in the storage.
    if (std::string_view(MULTIPLE_VALUES_EXAMPLE_CONFIG_ID) == item->identifier) {
        sExampleOptionValue = (ExampleOptions) newValue;
        // If the value has changed, we store it in the storage.
        WUPSStorageAPI::Store(item->identifier, sExampleOptionValue);
    }
}

void buttonComboItemChanged(ConfigItemButtonCombo *item, uint32_t newValue) {
    DEBUG_FUNCTION_LINE_INFO("New value in buttonComboItemChanged: %d for %s", newValue, item->identifier);
}

void ipAddressItemChangedConfig(ConfigItemIPAddress *item, uint32_t newValue) {
    if (std::string_view(IP_ADDRESS_CONFIG_ID) == item->identifier) {
        sIPAddress = newValue;
    }
    DEBUG_FUNCTION_LINE_INFO("New value in ipAddressItemChangedConfig: %d for %s", newValue, item->identifier);
}

WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle) {
    // To use the C++ API, we create new WUPSConfigCategory from the root handle!
    WUPSConfigCategory root = WUPSConfigCategory(rootHandle);

    // The functions of the Config API come in two variants: One that throws an exception, and another one which doesn't
    // To use the Config API without exception see the example below this try/catch block.
    try {
        // Then we can simply create a new category
        auto functionPatchesCat = WUPSConfigCategory::Create("function patches");

        // Add a boolean item to this newly created category
        functionPatchesCat.add(WUPSConfigItemBoolean::Create(LOG_FS_OPEN_CONFIG_ID, "Log FSOpen calls",
                                                             LOF_FS_OPEN_DEFAULT_VALUE, sLogFSOpen,
                                                             boolItemChanged));

        // And finally move that category to the root category.
        // Note: "functionPatchesCat" can NOT be changed after adding it to root.
        root.add(std::move(functionPatchesCat));

        // We can also add items directly to root!
        root.add(WUPSConfigItemBoolean::Create(OTHER_EXAMPLE_BOOL_CONFIG_ID, "Just another bool item",
                                               false, false,
                                               boolItemChanged));

        // You can also add an item which just displays any text.
        root.add(WUPSConfigItemStub::Create("This item is just displaying some text"));

        // It's also possible to create and item to select an integer from a range.
        root.add(WUPSConfigItemIntegerRange::Create(INTEGER_RANGE_EXAMPLE_CONFIG_ID, "Item for selecting an integer between 0 and 50",
                                                    INTEGER_RANGE_DEFAULT_VALUE, sIntegerRangeValue,
                                                    0, 50,
                                                    &integerRangeItemChanged));

        // To change a button combo, we can use the ButtonCombo ConfigItem
        root.add(WUPSConfigItemButtonCombo::Create(BUTTON_COMBO_PRESS_DOWN_CONFIG_ID, "Press Down button combo",
                                                   DEFAULT_PRESS_DOWN_BUTTON_COMBO, sPressDownExampleHandle,
                                                   buttonComboItemChanged));
        root.add(WUPSConfigItemButtonCombo::Create(BUTTON_COMBO_HOLD_CONFIG_ID, "Hold button combo",
                                                   DEFAULT_PRESS_HOLD_COMBO, sHoldExampleHandle,
                                                   buttonComboItemChanged));


        // To select value from an enum WUPSConfigItemMultipleValues fits the best.
        constexpr WUPSConfigItemMultipleValues::ValuePair possibleValues[] = {
                {EXAMPLE_OPTION_1, "Option 1"},
                {EXAMPLE_OPTION_2, "Option 2"},
                {EXAMPLE_OPTION_3, "Option 3"},
        };

        // It comes in two variants.
        // - "WUPSConfigItemMultipleValues::CreateFromValue" will take a default and current **value**
        // - "WUPSConfigItemMultipleValues::CreateFromIndex" will take a default and current **index**
        root.add(WUPSConfigItemMultipleValues::CreateFromValue(MULTIPLE_VALUES_EXAMPLE_CONFIG_ID, "Select an option!",
                                                               MULTIPLE_VALUES_DEFAULT_VALUE, sExampleOptionValue,
                                                               possibleValues,
                                                               nullptr));


        root.add(WUPSConfigItemIPAddress::Create(IP_ADDRESS_CONFIG_ID,
                                                 "IP address example",
                                                 IP_ADDRESS_DEFAULT_VALUE,
                                                 sIPAddress,
                                                 &ipAddressItemChangedConfig));

        // It's also possible to have nested categories
        auto nc1 = WUPSConfigCategory::Create("Category inside root");
        auto nc2 = WUPSConfigCategory::Create("Category inside subcategory 1");
        auto nc3 = WUPSConfigCategory::Create("Category inside subcategory 2");

        nc3.add(WUPSConfigItemStub::Create("Item inside subcategory 3"));
        nc2.add(WUPSConfigItemStub::Create("Item inside subcategory 2"));
        nc1.add(WUPSConfigItemStub::Create("Item inside subcategory 1"));

        nc2.add(std::move(nc3));
        nc1.add(std::move(nc2));
        root.add(std::move(nc1));
    } catch (std::exception &e) {
        DEBUG_FUNCTION_LINE_ERR("Creating config menu failed: %s", e.what());
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    // In case we don't like exception, we can use the API as well.
    // If we add a "WUPSConfigAPIStatus" reference to the API calls, the function won't throw an exception.
    // Instead, it will return std::optionals and write the result into the WUPSConfigAPIStatus.
    WUPSConfigAPIStatus err;
    auto categoryOpt = WUPSConfigCategory::Create("Just another Category", err);
    if (!categoryOpt) {
        DEBUG_FUNCTION_LINE_ERR("Failed to create category: %s", WUPSConfigAPI_GetStatusStr(err));
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    auto boolItemOpt = WUPSConfigItemBoolean::Create(OTHER_EXAMPLE2_BOOL_CONFIG_ID, "Just another bool item",
                                                     false, false,
                                                     boolItemChanged,
                                                     err);
    if (!boolItemOpt) {
        DEBUG_FUNCTION_LINE_ERR("Failed to create bool item: %s", WUPSConfigAPI_GetStatusStr(err));
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    // Add bool item to category
    if (!categoryOpt->add(std::move(*boolItemOpt), err)) {
        DEBUG_FUNCTION_LINE_ERR("Failed to add bool item to category: %s", WUPSConfigAPI_GetStatusStr(err));
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    // Add category to root.
    if (!root.add(std::move(*categoryOpt), err)) {
        DEBUG_FUNCTION_LINE_ERR("Failed to add category to root: %s", WUPSConfigAPI_GetStatusStr(err));
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    return WUPSCONFIG_API_CALLBACK_RESULT_SUCCESS;
}

void ConfigMenuClosedCallback() {
    WUPSStorageAPI::SaveStorage();
}

/**
    Gets called ONCE when the plugin was loaded.
**/
INITIALIZE_PLUGIN() {
    // Logging only works when compiled with `make DEBUG=1`. See the README for more information.
    initLogging();
    DEBUG_FUNCTION_LINE("INITIALIZE_PLUGIN of example_plugin!");

    WUPSConfigAPIOptionsV1 configOptions = {.name = "example_plugin_cpp"};
    if (WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback) != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("Failed to init config api");
    }

    WUPSStorageError storageRes;
    if ((storageRes = WUPSStorageAPI::GetOrStoreDefault(LOG_FS_OPEN_CONFIG_ID, sLogFSOpen, LOF_FS_OPEN_DEFAULT_VALUE)) != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("GetOrStoreDefault failed: %s (%d)", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }
    if ((storageRes = WUPSStorageAPI::SaveStorage()) != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("GetOrStoreDefault failed: %s (%d)", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }

    // To register a button combo, we can use the C++ wrapper class "WUPSButtonComboAPI::ButtonCombo".
    // The combo will be added on construction of that wrapper, and removed again in the destructor. Use `std::move` to move it around.
    // Like the C++ config api there are two versions of all function, one that throws an exception on error and one that returns a std::optional but set an additional error parameter.

    // Example of the exception throwing API
    try {
        WUPSButtonCombo_ComboStatus comboStatus;
        // Create a button combo which detects if a combo has been pressed down on any controller.
        // This version will check for conflicts. It's useful to check for conflicts if you want to use that button combo for a global unique thing
        // that's always possible, like taking screenshots.
        WUPSButtonComboAPI::ButtonCombo buttonComboPressDown = WUPSButtonComboAPI::CreateComboPressDown(
                "Example Plugin: Press Down test",
                DEFAULT_PRESS_DOWN_BUTTON_COMBO, // L + R
                [](const WUPSButtonCombo_ControllerTypes triggeredBy, WUPSButtonCombo_ComboHandle, void *) {
                    DEBUG_FUNCTION_LINE_INFO("Button combo has been pressed down by controller %s", WUPSButtonComboAPI::GetControllerTypeStr(triggeredBy));
                },
                nullptr,
                comboStatus);
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
        // We want to save the handle, so we can use it for the config menu
        sPressDownExampleHandle = buttonComboPressDown.getHandle();
        // BUT. We need to make sure to keep that button combo instance. Otherwise, the combo will be removed.
        // We save it in this list, which gets cleared in DEINITIALIZE_PLUGIN
        sButtonComboInstances.emplace_front(std::move(buttonComboPressDown));

        // --------------------------------------------------------------------------------------------------------------------------------------------------

        // But we can also create button combos without caring about conflicts.
        // E.g. when a new Aroma update is detected, the updater can be launched by holding the PLUS button. This should always be possible.

        // If we don't want to check for conflicts, we need to create a "PressDownObserver"
        WUPSButtonComboAPI::ButtonCombo buttonComboPressDownObserver = WUPSButtonComboAPI::CreateComboPressDownObserver(
                "Example Plugin: Press Down observer test",
                DEFAULT_PRESS_DOWN_BUTTON_COMBO, // L + R Even though this is same combo as in buttonComboPressDown an observer will ignore conflicts.
                [](const WUPSButtonCombo_ControllerTypes triggeredBy, WUPSButtonCombo_ComboHandle, void *) {
                    DEBUG_FUNCTION_LINE_INFO("[OBSERVER] Button combo has been pressed down by controller %s", WUPSButtonComboAPI::GetControllerTypeStr(triggeredBy));
                },
                nullptr,
                comboStatus); // comboStatus will always be WUPS_BUTTON_COMBO_COMBO_STATUS_VALID for observers.

        // Let's move this instance into the list as well. But in this case we don't need the handle
        sButtonComboInstances.emplace_front(std::move(buttonComboPressDownObserver));

        // --------------------------------------------------------------------------------------------------------------------------------------------------

        // In case of a conflict, the button combo instance will be returned, but the combo status will be WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT

        // Let's create a button combo which will lead to a conflict. This time we want to check if a combo has been hold for 500ms. Conflicts are checked across
        // non-observer combo types.
        WUPSButtonComboAPI::ButtonCombo buttonComboHold = WUPSButtonComboAPI::CreateComboHold(
                "Example Plugin: Hold test",
                DEFAULT_PRESS_HOLD_COMBO, // L+R+DPAD+DOWN. This combo includes the combo "L+R" of the buttonComboPressDown, so this will lead to a conflict.
                500,                      // We need to hold that combo for 500ms
                [](const WUPSButtonCombo_ControllerTypes triggeredBy, WUPSButtonCombo_ComboHandle, void *) {
                    DEBUG_FUNCTION_LINE_INFO("Button combo has been hold for 500ms by controller %s", WUPSButtonComboAPI::GetControllerTypeStr(triggeredBy));
                },
                nullptr,
                comboStatus); // comboStatus will always be WUPS_BUTTON_COMBO_COMBO_STATUS_VALID for observers.

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

        // Once combo is in the "WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT" state it can only be valid again, if the combo or the controllerMask changes. Other combo won't ever affect this state of this combo
        // We can easily update the button combo
        if (const auto res = buttonComboHold.UpdateButtonCombo(WUPS_BUTTON_COMBO_BUTTON_ZR | WUPS_BUTTON_COMBO_BUTTON_R | WUPS_BUTTON_COMBO_BUTTON_DOWN, comboStatus); res != WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
            DEBUG_FUNCTION_LINE_INFO("Failed to update button combo");
        } else {
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
        }

        // We want to save the handle, so we can use it for the config menu
        sHoldExampleHandle = buttonComboHold.getHandle();
        // Let's move this instance into the list as well. But in this case we don't need the handle
        sButtonComboInstances.emplace_front(std::move(buttonComboHold));

        // --------------------------------------------------------------------------------------------------------------------------------------------------

        // If we want to create combo for only a specific controller (or specific controllers), we have to use the "Ex" functions.

        // If we don't want to check for conflicts, we need to create a "PressDownObserver", but this time we have to provide a controllerMask and if it's a observer.
        WUPSButtonComboAPI::ButtonCombo buttonComboPressDownExObserver = WUPSButtonComboAPI::CreateComboPressDownEx(
                "Example Plugin: Press Down observer test",
                WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0, // Define which controllers should be checked. Could be something (WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0 | WUPS_BUTTON_COMBO_CONTROLLER_VPAD).
                DEFAULT_PRESS_DOWN_BUTTON_COMBO,     // L + R Even though this is same combo as in buttonComboPressDown an observer will ignore conflicts.
                [](const WUPSButtonCombo_ControllerTypes triggeredBy, WUPSButtonCombo_ComboHandle, void *) {
                    DEBUG_FUNCTION_LINE_INFO("[OBSERVER WPAD_0] Button combo has been pressed down by controller %s", WUPSButtonComboAPI::GetControllerTypeStr(triggeredBy));
                },
                nullptr,
                true,         // we want an observer
                comboStatus); // comboStatus will always be WUPS_BUTTON_COMBO_COMBO_STATUS_VALID for observers.

        // Let's move this instance into the list as well. But in this case we don't need the handle
        sButtonComboInstances.emplace_front(std::move(buttonComboPressDownExObserver));
    } catch (std::exception &e) {
        DEBUG_FUNCTION_LINE_ERR("Caught exception: %s", e.what());
    }


    // But we can also use the version which doesn't throw any exceptions
    WUPSButtonCombo_ComboStatus comboStatus;
    WUPSButtonCombo_Error comboError;

    // We add a "WUPSButtonCombo_Error" parameter at the end, the function will not throw any exception, but return a std::optional instead.
    // Create an observer, because we don't care for conflicts.
    std::optional<WUPSButtonComboAPI::ButtonCombo> buttonComboPressDownObserverOpt = WUPSButtonComboAPI::CreateComboPressDownObserver(
            "Example Plugin: Press Down test 2",
            WUPS_BUTTON_COMBO_BUTTON_X | WUPS_BUTTON_COMBO_BUTTON_Y,
            [](const WUPSButtonCombo_ControllerTypes triggeredBy, WUPSButtonCombo_ComboHandle, void *) {
                DEBUG_FUNCTION_LINE_INFO("[OBSERVER] Other button combo has been pressed down by controller %s", WUPSButtonComboAPI::GetControllerTypeStr(triggeredBy));
            },
            nullptr,
            comboStatus,
            comboError);
    if (buttonComboPressDownObserverOpt && comboError == WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
        // Creating was successful! Let's move it to the list as well.
        sButtonComboInstances.emplace_front(std::move(*buttonComboPressDownObserverOpt));
    }

    deinitLogging();
}

/**
    Gets called when the plugin will be unloaded.
**/
DEINITIALIZE_PLUGIN() {
    // Remove all button combos from this plugin.
    sButtonComboInstances.clear();
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
    if (sLogFSOpen) {
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