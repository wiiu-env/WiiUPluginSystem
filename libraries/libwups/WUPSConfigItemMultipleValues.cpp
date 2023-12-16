#include "wups/config/WUPSConfigItemMultipleValues.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wups/config.h>

static int32_t WUPSConfigItemMultipleValues_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemMultipleValues *) context;

    if (item->values && item->valueIndex >= 0 && item->valueIndex < item->valueCount) {
        if (item->values[item->valueIndex].valueName == nullptr) {
            return -2;
        }
        strncpy(out_buf, item->values[item->valueIndex].valueName, out_size);
        return 0;
    }
    return -1;
}

static void WUPSConfigItemMultipleValues_onCloseCallback(void *context) {
    auto *item = (ConfigItemMultipleValues *) context;
    if (item->valueIndexAtCreation != item->valueIndex && item->valueChangedCallback != nullptr) {
        if (item->values && item->valueIndex >= 0 && item->valueIndex < item->valueCount) {
            ((MultipleValuesChangedCallback) (item->valueChangedCallback))(item, item->values[item->valueIndex].value);
        }
    }
}

static void WUPSConfigItemMultipleValues_onInput(void *context, WUPSConfigSimplePadData input) {
    auto *item = (ConfigItemMultipleValues *) context;
    if (input.buttons_d & WUPS_CONFIG_BUTTON_LEFT) {
        item->valueIndex--;
    } else if (input.buttons_d & WUPS_CONFIG_BUTTON_RIGHT) {
        item->valueIndex++;
    }
    if (item->valueIndex < 0) {
        item->valueIndex = 0;
    } else if (item->valueIndex >= item->valueCount) {
        item->valueIndex = item->valueCount - 1;
    }
}

static int32_t WUPSConfigItemMultipleValues_getCurrentValueSelectedDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemMultipleValues *) context;
    if (item->values && item->valueIndex >= 0 && item->valueIndex < item->valueCount) {
        if (item->valueCount == 1) {
            snprintf(out_buf, out_size, "  %s  ", item->values[item->valueIndex].valueName);
        } else if (item->valueIndex == 0) {
            snprintf(out_buf, out_size, "  %s >", item->values[item->valueIndex].valueName);
        } else if (item->valueIndex + 1 == item->valueCount) {
            snprintf(out_buf, out_size, "< %s  ", item->values[item->valueIndex].valueName);
        } else {
            snprintf(out_buf, out_size, "< %s >", item->values[item->valueIndex].valueName);
        }
    } else {
        return -1;
    }
    return 0;
}

static void WUPSConfigItemMultipleValues_restoreDefault(void *context) {
    auto *item       = (ConfigItemMultipleValues *) context;
    item->valueIndex = item->defaultValueIndex;
}

static void WUPSConfigItemMultipleValues_Cleanup(ConfigItemMultipleValues *item) {
    if (!item) {
        return;
    }

    for (int i = 0; i < item->valueCount; ++i) {
        free((void *) item->values[i].valueName);
    }

    free((void *) item->identifier);

    free(item->values);
    free(item);
}

static void WUPSConfigItemMultipleValues_onDelete(void *context) {
    auto *item = (ConfigItemMultipleValues *) context;
    WUPSConfigItemMultipleValues_Cleanup(item);
}

extern "C" WUPSConfigAPIStatus
WUPSConfigItemMultipleValues_Create(const char *identifier, const char *displayName,
                                    int32_t defaultValueIndex, int currentValueIndex,
                                    ConfigItemMultipleValuesPair *possibleValues,
                                    int pairCount, MultipleValuesChangedCallback callback,
                                    WUPSConfigItemHandle *outHandle) {
    if (outHandle == nullptr || displayName == nullptr || possibleValues == nullptr || pairCount < 0 ||
        defaultValueIndex >= pairCount || currentValueIndex >= pairCount) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }
    *outHandle = {};
    auto *item = (ConfigItemMultipleValues *) malloc(sizeof(ConfigItemMultipleValues));
    if (item == nullptr) {
        return WUPSCONFIG_API_RESULT_OUT_OF_MEMORY;
    }
    *item = {};

    item->values = (ConfigItemMultipleValuesPair *) malloc(sizeof(ConfigItemMultipleValuesPair) * pairCount);
    if (!item->values) {
        WUPSConfigItemMultipleValues_Cleanup(item);
        return WUPSCONFIG_API_RESULT_OUT_OF_MEMORY;
    }

    if (identifier != nullptr) {
        item->identifier = strdup(identifier);
    } else {
        item->identifier = nullptr;
    }

    for (int i = 0; i < pairCount; ++i) {
        item->values[i].value = possibleValues[i].value;
        if (possibleValues[i].valueName == nullptr) {
            item->values[i].valueName = nullptr;
            continue;
        }
        item->values[i].valueName = strdup(possibleValues[i].valueName);
    }

    item->valueCount           = pairCount;
    item->defaultValueIndex    = defaultValueIndex;
    item->valueIndex           = currentValueIndex;
    item->valueIndexAtCreation = currentValueIndex;
    item->valueChangedCallback = (void *) callback;

    WUPSConfigAPIItemCallbacksV2 callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemMultipleValues_getCurrentValueDisplay,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemMultipleValues_getCurrentValueSelectedDisplay,
            .onSelected                     = nullptr,
            .restoreDefault                 = &WUPSConfigItemMultipleValues_restoreDefault,
            .isMovementAllowed              = nullptr,
            .onCloseCallback                = &WUPSConfigItemMultipleValues_onCloseCallback,
            .onInput                        = &WUPSConfigItemMultipleValues_onInput,
            .onInputEx                      = nullptr,
            .onDelete                       = &WUPSConfigItemMultipleValues_onDelete};

    WUPSConfigAPIItemOptionsV2 options = {
            .displayName = displayName,
            .context     = item,
            .callbacks   = callbacks,
    };

    WUPSConfigAPIStatus err;
    if ((err = WUPSConfigAPI_Item_Create(options, &item->handle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WUPSConfigItemMultipleValues_Cleanup(item);
        return err;
    }
    *outHandle = item->handle;
    return WUPSCONFIG_API_RESULT_SUCCESS;
}

extern "C" WUPSConfigAPIStatus
WUPSConfigItemMultipleValues_AddToCategory(WUPSConfigCategoryHandle cat, const char *identifier, const char *displayName,
                                           int32_t defaultValueIndex, int currentValueIndex,
                                           ConfigItemMultipleValuesPair *possibleValues, int pairCount,
                                           MultipleValuesChangedCallback callback) {
    WUPSConfigItemHandle itemHandle;
    WUPSConfigAPIStatus res;
    if ((res = WUPSConfigItemMultipleValues_Create(identifier,
                                                   displayName,
                                                   defaultValueIndex, currentValueIndex,
                                                   possibleValues, pairCount,
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
