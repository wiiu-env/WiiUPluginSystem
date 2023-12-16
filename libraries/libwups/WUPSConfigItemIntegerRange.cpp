#include "wups/config/WUPSConfigItemIntegerRange.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wups/config.h>

void WUPSConfigItemIntegerRange_onCloseCallback(void *context) {
    auto *item = (ConfigItemIntegerRange *) context;
    if (item->valueAtCreation != item->value && item->valueChangedCallback != nullptr) {
        ((IntegerRangeValueChangedCallback) item->valueChangedCallback)(item, item->value);
    }
}

void WUPSConfigItemIntegerRange_onInput(void *context, WUPSConfigSimplePadData input) {
    auto *item = (ConfigItemIntegerRange *) context;

    if (input.buttons_d & WUPS_CONFIG_BUTTON_LEFT) {
        item->value--;
    } else if ((input.buttons_d & WUPS_CONFIG_BUTTON_RIGHT)) {
        item->value++;
    } else if ((input.buttons_d & WUPS_CONFIG_BUTTON_L)) {
        item->value = item->value - 50;
    } else if ((input.buttons_d & WUPS_CONFIG_BUTTON_R)) {
        item->value = item->value + 50;
    }

    if (item->value < item->minValue) {
        item->value = item->minValue;
    } else if (item->value > item->maxValue) {
        item->value = item->maxValue;
    }
}

int32_t WUPSConfigItemIntegerRange_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemIntegerRange *) context;
    snprintf(out_buf, out_size, "  %d", item->value);
    return 0;
}

int32_t WUPSConfigItemIntegerRange_getCurrentValueSelectedDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemIntegerRange *) context;
    if (item->value == item->minValue) {
        snprintf(out_buf, out_size, "  %d >", item->value);
    } else if (item->value == item->maxValue) {
        snprintf(out_buf, out_size, "<  %d  ", item->value);
    } else {
        snprintf(out_buf, out_size, "< %d >", item->value);
    }
    return 0;
}

void WUPSConfigItemIntegerRange_restoreDefault(void *context) {
    auto *item  = (ConfigItemIntegerRange *) context;
    item->value = item->defaultValue;
}

static void WUPSConfigItemIntegerRange_Cleanup(ConfigItemIntegerRange *item) {
    if (!item) {
        return;
    }
    free((void *) item->identifier);
    free(item);
}

void WUPSConfigItemIntegerRange_onDelete(void *context) {
    WUPSConfigItemIntegerRange_Cleanup((ConfigItemIntegerRange *) context);
}

extern "C" WUPSConfigAPIStatus
WUPSConfigItemIntegerRange_Create(const char *identifier,
                                  const char *displayName,
                                  int32_t defaultValue, int32_t currentValue,
                                  int32_t minValue, int32_t maxValue,
                                  IntegerRangeValueChangedCallback callback,
                                  WUPSConfigItemHandle *outHandle) {
    if (outHandle == nullptr) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }
    if (maxValue < minValue || defaultValue < minValue || defaultValue > maxValue || currentValue < minValue || currentValue > maxValue) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }
    *outHandle = {};
    auto *item = (ConfigItemIntegerRange *) malloc(sizeof(ConfigItemIntegerRange));
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
    item->minValue             = minValue;
    item->maxValue             = maxValue;
    item->valueChangedCallback = (void *) callback;

    WUPSConfigAPIItemCallbacksV2 callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemIntegerRange_getCurrentValueDisplay,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemIntegerRange_getCurrentValueSelectedDisplay,
            .onSelected                     = nullptr,
            .restoreDefault                 = &WUPSConfigItemIntegerRange_restoreDefault,
            .isMovementAllowed              = nullptr,
            .onCloseCallback                = &WUPSConfigItemIntegerRange_onCloseCallback,
            .onInput                        = &WUPSConfigItemIntegerRange_onInput,
            .onInputEx                      = nullptr,
            .onDelete                       = &WUPSConfigItemIntegerRange_onDelete,
    };

    WUPSConfigAPIItemOptionsV2 options = {
            .displayName = displayName,
            .context     = item,
            .callbacks   = callbacks,
    };

    WUPSConfigAPIStatus err;
    if ((err = WUPSConfigAPI_Item_Create(options, &(item->handle))) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WUPSConfigItemIntegerRange_Cleanup(item);
        return err;
    }
    *outHandle = item->handle;

    return WUPSCONFIG_API_RESULT_SUCCESS;
}

extern "C" WUPSConfigAPIStatus
WUPSConfigItemIntegerRange_AddToCategory(WUPSConfigCategoryHandle cat,
                                         const char *identifier,
                                         const char *displayName,
                                         int32_t defaultValue, int32_t currentValue,
                                         int32_t minValue, int32_t maxValue,
                                         IntegerRangeValueChangedCallback callback) {
    WUPSConfigItemHandle itemHandle;
    WUPSConfigAPIStatus res;
    if ((res = WUPSConfigItemIntegerRange_Create(identifier,
                                                 displayName,
                                                 defaultValue, currentValue,
                                                 minValue, maxValue,
                                                 callback,
                                                 &itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return res;
    }

    if ((res = WUPSConfigAPI_Category_AddItem(cat, itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WUPSConfigAPI_Item_Destroy(itemHandle);
        return res;
    }
    return WUPSCONFIG_API_RESULT_SUCCESS;
}
