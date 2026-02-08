#include <coreinit/debug.h>
#include <coreinit/thread.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <wups.h>
#include <wups/config/WUPSConfigItemIPAddress.h>

namespace {
    void modifyIpOctet(ConfigItemIPAddress *item, int octetIndex, const int delta) {
        const int shift = (3 - octetIndex) * 8;
        uint8_t octet   = (item->currentValue >> shift) & 0xFF;
        octet += delta;
        const uint32_t mask = ~(0xFF << shift);
        item->currentValue  = (item->currentValue & mask) | (static_cast<uint32_t>(octet) << shift);
    }

    int32_t getCurrentValueDisplayGeneric(void *context, bool isSelected, char *out_buf, int32_t out_size) {
        auto *item = static_cast<ConfigItemIPAddress *>(context);

        const uint8_t o1 = (item->currentValue >> 24) & 0xFF;
        const uint8_t o2 = (item->currentValue >> 16) & 0xFF;
        const uint8_t o3 = (item->currentValue >> 8) & 0xFF;
        const uint8_t o4 = (item->currentValue) & 0xFF;

        switch (item->itemState) {
            case WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD1:
                snprintf(out_buf, out_size, "[%u].%u.%u.%u", o1, o2, o3, o4);
                break;

            case WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD2:
                snprintf(out_buf, out_size, "%u.[%u].%u.%u", o1, o2, o3, o4);
                break;

            case WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD3:
                snprintf(out_buf, out_size, "%u.%u.[%u].%u", o1, o2, o3, o4);
                break;

            case WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD4:
                snprintf(out_buf, out_size, "%u.%u.%u.[%u]", o1, o2, o3, o4);
                break;

            default:
                if (isSelected) {
                    snprintf(out_buf, out_size, "(Press A to edit) %u.%u.%u.%u", o1, o2, o3, o4);
                } else {
                    snprintf(out_buf, out_size, "%u.%u.%u.%u", o1, o2, o3, o4);
                }
                break;
        }

        return 0;
    }

} // namespace

static int32_t WUPSConfigItemIPAddress_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    return getCurrentValueDisplayGeneric(context, false, out_buf, out_size);
}

static void WUPSConfigItemIPAddress_onCloseCallback(void *context) {
    auto *item = static_cast<ConfigItemIPAddress *>(context);
    if (item->currentValue != item->valueAtCreation && item->valueChangedCallback != nullptr) {
        reinterpret_cast<IPAddressValueChangedCallback>(item->valueChangedCallback)(item, item->currentValue);
    }
}

#define IP_VALUE_EDIT_INITIAL_DELAY 5
static void WUPSConfigItemIPAddress_onInput(void *context, WUPSConfigSimplePadData input) {
    auto *item = static_cast<ConfigItemIPAddress *>(context);

    // Handle edit request
    if (item->itemState == WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_NONE) {
        if (input.buttons_d & WUPS_CONFIG_BUTTON_A) {
            item->valueBeforeEdit = item->currentValue;
            item->itemState       = WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD1;
            item->frameTimer      = 0;
        }
        return;
    }

    // When editing press A to accept, press B to abort
    if ((input.buttons_d & WUPS_CONFIG_BUTTON_A) || (input.buttons_d & WUPS_CONFIG_BUTTON_B)) {
        if (input.buttons_d & WUPS_CONFIG_BUTTON_B) {
            item->currentValue = item->valueBeforeEdit;
        }
        item->itemState = WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_NONE;
        return;
    }

    int fieldIndex = 0;
    switch (item->itemState) {
        case WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD1: {
            fieldIndex = 0;
            break;
        }
        case WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD2: {
            fieldIndex = 1;
            break;
        }
        case WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD3: {
            fieldIndex = 2;
            break;
        }
        case WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD4: {
            fieldIndex = 3;
            break;
        }
        default:
            return;
    }

    // Handle navigation between field
    if (input.buttons_d & WUPS_CONFIG_BUTTON_LEFT) {
        if (fieldIndex > 0) {
            item->itemState = static_cast<ConfigItemIPAddress_State>(item->itemState - 1);
        } else
            item->itemState = WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD4;
        return;
    }
    if (input.buttons_d & WUPS_CONFIG_BUTTON_RIGHT) {
        if (fieldIndex < 3) {
            item->itemState = static_cast<ConfigItemIPAddress_State>(item->itemState + 1);
        } else {
            item->itemState = WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD1;
        }
        return;
    }

    // Handle modify ocet
    bool isUp   = input.buttons_h & WUPS_CONFIG_BUTTON_UP;
    bool isDown = input.buttons_h & WUPS_CONFIG_BUTTON_DOWN;

    if (isUp || isDown) {
        bool applyChange = false;

        if ((isUp && (input.buttons_d & WUPS_CONFIG_BUTTON_UP)) ||
            (isDown && (input.buttons_d & WUPS_CONFIG_BUTTON_DOWN))) {
            applyChange      = true;
            item->frameTimer = IP_VALUE_EDIT_INITIAL_DELAY;
        } else {
            if (item->frameTimer > 0) {
                item->frameTimer--;
            } else {
                applyChange = true;
            }
        }

        if (applyChange) {
            modifyIpOctet(item, fieldIndex, isUp ? 1 : -1);
        }
    } else {
        item->frameTimer = 0;
    }
}

static int32_t
WUPSConfigItemIPAddress_getCurrentValueSelectedDisplay(void *context, char *out_buf, int32_t out_size) {
    return getCurrentValueDisplayGeneric(context, true, out_buf, out_size);
}

static void WUPSConfigItemIPAddress_restoreDefault(void *context) {
    auto *item = static_cast<ConfigItemIPAddress *>(context);

    item->currentValue = item->defaultValue;
}

static bool WUPSConfigItemIPAddress_isMovementAllowed(void *context) {
    const auto *item = static_cast<ConfigItemIPAddress *>(context);
    return item->itemState == WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_NONE;
}

static void WUPSConfigItemIPAddress_Cleanup(ConfigItemIPAddress *item) {
    if (!item) {
        return;
    }
    if (item->identifier) {
        free(item->identifier);
    }
    free(item);
}

static void WUPSConfigItemIPAddress_onDelete(void *context) {
    WUPSConfigItemIPAddress_Cleanup(static_cast<ConfigItemIPAddress *>(context));
}

WUPSConfigAPIStatus
WUPSConfigItemIPAddress_Create(const char *identifier,
                               const char *displayName,
                               const uint32_t defaultValue,
                               const uint32_t currentValue,
                               IPAddressValueChangedCallback callback,
                               WUPSConfigItemHandle *outItemHandle) {
    if (outItemHandle == nullptr) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }
    auto *item = static_cast<ConfigItemIPAddress *>(malloc(sizeof(ConfigItemIPAddress)));
    if (item == nullptr) {
        OSReport("WUPSConfigItemIPAddress_CreateEx: Failed to allocate memory for item data.\n");
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }

    if (identifier != nullptr) {
        item->identifier = strdup(identifier);
    } else {
        item->identifier = nullptr;
    }

    item->currentValue         = currentValue;
    item->defaultValue         = defaultValue;
    item->valueAtCreation      = currentValue;
    item->valueChangedCallback = reinterpret_cast<void *>(callback);

    item->itemState = WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_NONE;

    constexpr WUPSConfigAPIItemCallbacksV2 callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemIPAddress_getCurrentValueDisplay,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemIPAddress_getCurrentValueSelectedDisplay,
            .onSelected                     = nullptr,
            .restoreDefault                 = &WUPSConfigItemIPAddress_restoreDefault,
            .isMovementAllowed              = &WUPSConfigItemIPAddress_isMovementAllowed,
            .onCloseCallback                = &WUPSConfigItemIPAddress_onCloseCallback,
            .onInput                        = &WUPSConfigItemIPAddress_onInput,
            .onInputEx                      = nullptr,
            .onDelete                       = &WUPSConfigItemIPAddress_onDelete,
    };

    const WUPSConfigAPIItemOptionsV2 options = {
            .displayName = displayName,
            .context     = item,
            .callbacks   = callbacks,
    };

    if (const WUPSConfigAPIStatus err = WUPSConfigAPI_Item_Create(options, &item->itemHandle); err != WUPSCONFIG_API_RESULT_SUCCESS) {
        OSReport("WUPSConfigItemIPAddress_Create: Failed to create config item.\n");
        WUPSConfigItemIPAddress_Cleanup(item);
        return err;
    }

    *outItemHandle = item->itemHandle;
    return WUPSCONFIG_API_RESULT_SUCCESS;
}


WUPSConfigAPIStatus
WUPSConfigItemIPAddress_AddToCategory(const WUPSConfigCategoryHandle cat,
                                      const char *identifier, const char *displayName,
                                      const uint32_t defaultValue,
                                      const uint32_t currentValue,
                                      const IPAddressValueChangedCallback callback) {
    WUPSConfigItemHandle itemHandle;
    WUPSConfigAPIStatus res;
    if ((res = WUPSConfigItemIPAddress_Create(identifier,
                                              displayName,
                                              defaultValue,
                                              currentValue,
                                              callback, &itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return res;
    }

    if ((res = WUPSConfigAPI_Category_AddItem(cat, itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WUPSConfigAPI_Item_Destroy(itemHandle);
        return res;
    }
    return WUPSCONFIG_API_RESULT_SUCCESS;
}
