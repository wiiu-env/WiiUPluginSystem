#include "wups/config/WUPSConfigItemBoolean.h"
#include "wups/config_api.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wups/config.h>

static void WUPSConfigItemBoolean_onCloseCallback(void *context) {
    auto *item = (ConfigItemBoolean *) context;
    if (item->valueAtCreation != item->value && item->valueChangedCallback != nullptr) {
        ((BooleanValueChangedCallback) (item->valueChangedCallback))(item, item->value);
    }
}

static inline void toggleValue(ConfigItemBoolean *item) {
    item->value = !item->value;
}

static void WUPSConfigItemBoolean_onInput(void *context, WUPSConfigSimplePadData input) {
    auto *item = (ConfigItemBoolean *) context;
    if ((input.buttons_d & WUPS_CONFIG_BUTTON_A) == WUPS_CONFIG_BUTTON_A) {
        toggleValue(item);
    } else if (input.buttons_d & WUPS_CONFIG_BUTTON_LEFT && !item->value) {
        toggleValue(item);
    } else if ((input.buttons_d & WUPS_CONFIG_BUTTON_RIGHT) && item->value) {
        toggleValue(item);
    }
}
static int32_t WUPSConfigItemBoolean_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemBoolean *) context;
    snprintf(out_buf, out_size, "  %s", item->value ? item->trueValue : item->falseValue);
    return 0;
}

static int32_t WUPSConfigItemBoolean_getCurrentValueSelectedDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemBoolean *) context;
    if (item->value) {
        snprintf(out_buf, out_size, "  %s >", item->trueValue);
    } else {
        snprintf(out_buf, out_size, "< %s  ", item->falseValue);
    }
    return 0;
}

static void WUPSConfigItemBoolean_restoreDefault(void *context) {
    auto *item  = (ConfigItemBoolean *) context;
    item->value = item->defaultValue;
}

static void WUPSConfigItemBoolean_Cleanup(ConfigItemBoolean *item) {
    if (!item) {
        return;
    }
    free((void *) item->identifier);
    free(item);
}

static void WUPSConfigItemBoolean_onDelete(void *context) {
    WUPSConfigItemBoolean_Cleanup((ConfigItemBoolean *) context);
}

extern "C" WUPSConfigAPIStatus
WUPSConfigItemBoolean_CreateEx(const char *identifier,
                               const char *displayName,
                               bool defaultValue,
                               bool currentValue,
                               BooleanValueChangedCallback callback,
                               const char *trueValue,
                               const char *falseValue,
                               WUPSConfigItemHandle *outHandle) {
    if (outHandle == nullptr) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }
    auto *item = (ConfigItemBoolean *) malloc(sizeof(ConfigItemBoolean));
    if (item == nullptr) {
        return WUPSCONFIG_API_RESULT_OUT_OF_MEMORY;
    }

    if (identifier != nullptr) {
        item->identifier = strdup(identifier);
    } else {
        item->identifier = nullptr;
    }

    item->defaultValue         = defaultValue;
    item->value                = currentValue;
    item->valueAtCreation      = currentValue;
    item->valueChangedCallback = (void *) callback;
    snprintf(item->trueValue, sizeof(item->trueValue), "%s", trueValue);
    snprintf(item->falseValue, sizeof(item->falseValue), "%s", falseValue);

    WUPSConfigAPIItemCallbacksV2 callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemBoolean_getCurrentValueDisplay,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemBoolean_getCurrentValueSelectedDisplay,
            .onSelected                     = nullptr,
            .restoreDefault                 = &WUPSConfigItemBoolean_restoreDefault,
            .isMovementAllowed              = nullptr,
            .onCloseCallback                = &WUPSConfigItemBoolean_onCloseCallback,
            .onInput                        = &WUPSConfigItemBoolean_onInput,
            .onInputEx                      = nullptr,
            .onDelete                       = &WUPSConfigItemBoolean_onDelete};

    WUPSConfigAPIItemOptionsV2 options = {
            .displayName = displayName,
            .context     = item,
            .callbacks   = callbacks,
    };

    WUPSConfigAPIStatus err;
    if ((err = WUPSConfigAPI_Item_Create(options, &item->handle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WUPSConfigItemBoolean_Cleanup(item);
        return err;
    }

    *outHandle = item->handle;
    return WUPSCONFIG_API_RESULT_SUCCESS;
}

extern "C" WUPSConfigAPIStatus
WUPSConfigItemBoolean_AddToCategoryEx(WUPSConfigCategoryHandle cat,
                                      const char *identifier,
                                      const char *displayName,
                                      bool defaultValue,
                                      bool currentValue,
                                      BooleanValueChangedCallback callback,
                                      const char *trueValue,
                                      const char *falseValue) {
    WUPSConfigItemHandle itemHandle;
    WUPSConfigAPIStatus res;
    if ((res = WUPSConfigItemBoolean_CreateEx(identifier,
                                              displayName,
                                              defaultValue, currentValue,
                                              callback,
                                              trueValue, falseValue,
                                              &itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return res;
    }

    if ((res = WUPSConfigAPI_Category_AddItem(cat, itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {

        WUPSConfigAPI_Item_Destroy(itemHandle);
        return res;
    }
    return WUPSCONFIG_API_RESULT_SUCCESS;
}

extern "C" WUPSConfigAPIStatus
WUPSConfigItemBoolean_AddToCategory(WUPSConfigCategoryHandle cat,
                                    const char *identifier,
                                    const char *displayName,
                                    bool defaultValue,
                                    bool currentValue,
                                    BooleanValueChangedCallback callback) {
    return WUPSConfigItemBoolean_AddToCategoryEx(cat, identifier, displayName, defaultValue, currentValue, callback, "true", "false");
}
