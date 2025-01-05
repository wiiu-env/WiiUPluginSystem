#include <coreinit/debug.h>
#include <coreinit/thread.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <wups.h>
#include <wups/button_combo/api.h>
#include <wups/config/WUPSConfigItemButtonCombo.h>

namespace {
    const char *getButtonChar(const WUPSButtonCombo_Buttons value) {
        std::string combo;
        if (value & WUPS_BUTTON_COMBO_BUTTON_A) {
            return "\ue000";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_B) {
            return "\ue001";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_X) {
            return "\ue002";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_Y) {
            return "\ue003";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_L) {
            return "\ue083";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_R) {
            return "\ue084";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_ZL) {
            return "\ue085";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_ZR) {
            return "\ue086";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_UP) {
            return "\ue079";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_DOWN) {
            return "\ue07A";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_LEFT) {
            return "\ue07B";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_RIGHT) {
            return "\ue07C";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_STICK_L) {
            return "\ue081";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_STICK_R) {
            return "\ue082";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_PLUS) {
            return "\ue045";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_MINUS) {
            return "\ue046";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_TV) {
            return "\ue089";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_RESERVED_BIT) {
            return "\ue01E";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_1) {
            return "\uE047";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_2) {
            return "\uE048";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_C) {
            return "\uE04A";
        }
        if (value & WUPS_BUTTON_COMBO_BUTTON_Z) {
            return "\uE04B";
        }
        return "";
    }

    std::string getComboAsString(const uint32_t value) {
        char comboString[60] = {};

        for (uint32_t i = 0; i < 32; i++) {
            uint32_t bitMask = 1 << i;
            if (value & bitMask) {
                auto val = getButtonChar(static_cast<WUPSButtonCombo_Buttons>(bitMask));
                if (val[0] != '\0') {
                    strcat(comboString, val);
                    strcat(comboString, "+");
                }
            }
        }
        std::string res(comboString);
        if (res.ends_with("+")) {
            res.pop_back();
        }
        return res;
    }

    WUPSButtonCombo_Error checkForHold(ConfigItemButtonCombo *item) {
        const auto oldCombo = item->currentButtonCombo;

        auto buttonCombo = static_cast<WUPSButtonCombo_Buttons>(0);

        // This call is **blocking** it only returns if a button combo was detected or abort was triggered
        WUPSButtonCombo_DetectButtonComboOptions options;
        options.controllerMask   = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
        options.abortButtonCombo = item->detectAbortButton;
        options.holdAbortForInMs = item->detectAbortHoldButtonInMs;
        options.holdComboForInMs = item->detectComboHoldDurationInMs;
        if (const auto res = WUPSButtonComboAPI_DetectButtonCombo_Blocking(&options, &buttonCombo); res != WUPS_BUTTON_COMBO_ERROR_SUCCESS && res != WUPS_BUTTON_COMBO_ERROR_ABORTED) {
            OSReport("WUPSButtonComboAPI_DetectButtonCombo_Blocking returned %s\n", WUPSButtonComboAPI_GetStatusStr(res));
            return res;
        }

        WUPSButtonCombo_ComboStatus status = WUPS_BUTTON_COMBO_COMBO_STATUS_INVALID_STATUS;
        if (const auto res = WUPSButtonComboAPI_UpdateButtonCombo(item->comboHandle, buttonCombo, &status); res != WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
            OSReport("Failed to update combo info\n");
            return WUPS_BUTTON_COMBO_ERROR_SUCCESS;
        }
        if (status != WUPS_BUTTON_COMBO_COMBO_STATUS_VALID) {
            // (Try) to restore "old" button combo

            if (WUPSButtonComboAPI_UpdateButtonCombo(item->comboHandle, oldCombo, &status) != WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
                OSReport("Failed to update combo info\n");
            }
            return WUPS_BUTTON_COMBO_ERROR_UNKNOWN_ERROR;
        }

        item->currentButtonCombo = buttonCombo;
        return WUPS_BUTTON_COMBO_ERROR_SUCCESS;
    }

    int32_t getCurrentValueDisplayGeneric(void *context, bool isSelected, char *out_buf, int32_t out_size) {
        auto *item = static_cast<ConfigItemButtonCombo *>(context);

        WUPSButtonCombo_ComboStatus comboStatus = WUPS_BUTTON_COMBO_COMBO_STATUS_INVALID_STATUS;
        if (const auto res = WUPSButtonComboAPI_GetButtonComboStatus(item->comboHandle, &comboStatus); res != WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
            OSReport("WUPSConfigItemButtonCombo_getCurrentValueDisplayGeneric: GetButtonComboStatus returned %s\n", WUPSButtonComboAPI_GetStatusStr(res));
        }

        switch (item->itemState) {
            case WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_INVALID_HANDLE: {
                snprintf(out_buf, out_size, "ERROR: Invalid combo handle");
                return 0;
            }
            case WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_PREPARE_FOR_HOLD:
            case WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_WAIT_FOR_HOLD: {
                if (item->itemState == WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_PREPARE_FOR_HOLD) {
                    item->itemState = WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_WAIT_FOR_HOLD;
                    snprintf(out_buf, out_size, "<Hold new combo for %dms; hold %s to abort>", item->detectComboHoldDurationInMs, getButtonChar(item->detectAbortButton));
                    return 0;
                }
                if (const auto res = checkForHold(item); res == WUPS_BUTTON_COMBO_ERROR_SUCCESS || res == WUPS_BUTTON_COMBO_ERROR_ABORTED) {
                    item->itemState = WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_NONE;
                } else {
                    item->itemState = WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_PREPARE_FOR_HOLD;
                    snprintf(out_buf, out_size, "ERROR: Conflict detected. Try again");
                    OSSleepTicks(OSMillisecondsToTicks(2000));
                    return 0;
                }
                break;
            }
            case WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_NONE:
                break;
        }

        if (isSelected) {
            snprintf(out_buf, out_size, "(Press \ue000 to change) %s%s", comboStatus == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID ? "" : "[CONFLICT] ", getComboAsString(item->currentButtonCombo).c_str());
        } else {
            snprintf(out_buf, out_size, "%s%s", comboStatus == WUPS_BUTTON_COMBO_COMBO_STATUS_VALID ? "" : "[CONFLICT] ", getComboAsString(item->currentButtonCombo).c_str());
        }
        return 0;
    }
} // namespace

static int32_t WUPSConfigItemButtonCombo_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    return getCurrentValueDisplayGeneric(context, false, out_buf, out_size);
}

static void WUPSConfigItemButtonCombo_onCloseCallback(void *context) {
    auto *item = static_cast<ConfigItemButtonCombo *>(context);
    if (item->comboComboAtCreation != item->currentButtonCombo && item->valueChangedCallback != nullptr) {
        reinterpret_cast<ButtonComboValueChangedCallback>(item->valueChangedCallback)(item, item->currentButtonCombo);
    }
}

static void WUPSConfigItemButtonCombo_onInput(void *context, WUPSConfigSimplePadData input) {
    auto *item = static_cast<ConfigItemButtonCombo *>(context);
    if (item->itemState == WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_NONE) {
        if ((input.buttons_d & WUPS_CONFIG_BUTTON_A) == WUPS_CONFIG_BUTTON_A) {
            item->itemState = WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_PREPARE_FOR_HOLD;
        }
    }
}

static int32_t
WUPSConfigItemButtonCombo_getCurrentValueSelectedDisplay(void *context, char *out_buf, int32_t out_size) {
    return getCurrentValueDisplayGeneric(context, true, out_buf, out_size);
}

static void WUPSConfigItemButtonCombo_restoreDefault(void *context) {
    auto *item = static_cast<ConfigItemButtonCombo *>(context);

    if (const auto res = WUPSButtonComboAPI_UpdateButtonCombo(item->comboHandle, item->defaultButtonCombo, nullptr); res != WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
        OSReport("WUPSConfigItemButtonCombo_restoreDefault: Failed to update combo info\n");
        return;
    }
    item->currentButtonCombo = item->defaultButtonCombo;
}

static bool WUPSConfigItemButtonCombo_isMovementAllowed(void *context) {
    const auto *item = static_cast<ConfigItemButtonCombo *>(context);
    return item->itemState != WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_PREPARE_FOR_HOLD && item->itemState != WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_WAIT_FOR_HOLD;
}

static void WUPSConfigItemButtonCombo_Cleanup(ConfigItemButtonCombo *item) {
    if (!item) {
        return;
    }
    free(item->identifier);
    free(item);
}

static void WUPSConfigItemButtonCombo_onDelete(void *context) {
    WUPSConfigItemButtonCombo_Cleanup(static_cast<ConfigItemButtonCombo *>(context));
}

WUPSConfigAPIStatus
WUPSConfigItemButtonCombo_CreateEx(const char *identifier,
                                   const char *displayName,
                                   const WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, const WUPSButtonCombo_ComboHandle comboHandle,
                                   const uint32_t detectHoldDurationInMs, const WUPSButtonCombo_Buttons detectAbortButtonCombo, const uint32_t detectAbortHoldButtonInMs,
                                   const ButtonComboValueChangedCallback callback,
                                   WUPSConfigItemHandle *outItemHandle) {
    if (outItemHandle == nullptr) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }
    auto *item = static_cast<ConfigItemButtonCombo *>(malloc(sizeof(ConfigItemButtonCombo)));
    if (item == nullptr) {
        OSReport("WUPSConfigItemButtonCombo_CreateEx: Failed to allocate memory for item data.\n");
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }

    if (identifier != nullptr) {
        item->identifier = strdup(identifier);
    } else {
        item->identifier = nullptr;
    }

    item->comboHandle                       = comboHandle;
    item->defaultButtonCombo                = static_cast<WUPSButtonCombo_Buttons>(defaultComboInWUPSButtonComboButtons);
    item->currentButtonCombo                = static_cast<WUPSButtonCombo_Buttons>(0);
    item->comboComboAtCreation              = defaultComboInWUPSButtonComboButtons;
    item->detectComboHoldDurationInMs       = detectHoldDurationInMs;
    item->detectAbortButton                 = detectAbortButtonCombo;
    item->detectAbortHoldButtonInMs         = detectAbortHoldButtonInMs;
    item->valueChangedCallback              = reinterpret_cast<void *>(callback);
    item->intern.conflictMsgShownForXFrames = 0;


    item->itemState = WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_NONE;

    WUPSButtonCombo_ButtonComboInfoEx comboOptions = {};
    if (const WUPSButtonCombo_Error comboErr = WUPSButtonComboAPI_GetButtonComboInfoEx(comboHandle, &comboOptions); comboErr == WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
        item->comboComboAtCreation = comboOptions.basicCombo.combo;
        item->currentButtonCombo   = comboOptions.basicCombo.combo;
    } else {
        OSReport("WUPSButtonComboAPI_GetButtonComboInfo failed %s\n", WUPSButtonComboAPI_GetStatusStr(comboErr));
        item->itemState = WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_INVALID_HANDLE;
    }

    constexpr WUPSConfigAPIItemCallbacksV2 callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemButtonCombo_getCurrentValueDisplay,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemButtonCombo_getCurrentValueSelectedDisplay,
            .onSelected                     = nullptr,
            .restoreDefault                 = &WUPSConfigItemButtonCombo_restoreDefault,
            .isMovementAllowed              = &WUPSConfigItemButtonCombo_isMovementAllowed,
            .onCloseCallback                = &WUPSConfigItemButtonCombo_onCloseCallback,
            .onInput                        = &WUPSConfigItemButtonCombo_onInput,
            .onInputEx                      = nullptr,
            .onDelete                       = &WUPSConfigItemButtonCombo_onDelete,
    };

    const WUPSConfigAPIItemOptionsV2 options = {
            .displayName = displayName,
            .context     = item,
            .callbacks   = callbacks,
    };

    if (const WUPSConfigAPIStatus err = WUPSConfigAPI_Item_Create(options, &item->itemHandle); err != WUPSCONFIG_API_RESULT_SUCCESS) {
        OSReport("WUPSConfigItemButtonCombo_tAddToCategoryEx: Failed to create config item.\n");
        WUPSConfigItemButtonCombo_Cleanup(item);
        return err;
    }

    *outItemHandle = item->itemHandle;
    return WUPSCONFIG_API_RESULT_SUCCESS;
}


WUPSConfigAPIStatus
WUPSConfigItemButtonCombo_AddToCategoryEx(const WUPSConfigCategoryHandle cat,
                                          const char *identifier, const char *displayName,
                                          const WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, const WUPSButtonCombo_ComboHandle comboHandle,
                                          const uint32_t detectHoldDurationInMs, const WUPSButtonCombo_Buttons detectAbortButtonCombo, const uint32_t detectAbortHoldButtonInMs,
                                          const ButtonComboValueChangedCallback callback) {
    WUPSConfigItemHandle itemHandle;
    WUPSConfigAPIStatus res;
    if ((res = WUPSConfigItemButtonCombo_CreateEx(identifier,
                                                  displayName,
                                                  defaultComboInWUPSButtonComboButtons, comboHandle,
                                                  detectHoldDurationInMs, detectAbortButtonCombo, detectAbortHoldButtonInMs,
                                                  callback, &itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return res;
    }

    if ((res = WUPSConfigAPI_Category_AddItem(cat, itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WUPSConfigAPI_Item_Destroy(itemHandle);
        return res;
    }
    return WUPSCONFIG_API_RESULT_SUCCESS;
}

WUPSConfigAPIStatus
WUPSConfigItemButtonCombo_AddToCategory(const WUPSConfigCategoryHandle cat,
                                        const char *identifier, const char *displayName,
                                        const WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, const WUPSButtonCombo_ComboHandle comboHandle,
                                        const ButtonComboValueChangedCallback callback) {
    return WUPSConfigItemButtonCombo_AddToCategoryEx(cat, identifier, displayName,
                                                     defaultComboInWUPSButtonComboButtons, comboHandle,
                                                     2000, WUPS_BUTTON_COMBO_BUTTON_B, 250,
                                                     callback);
}
