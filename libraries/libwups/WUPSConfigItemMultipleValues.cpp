#include "wups/config/WUPSConfigItemMultipleValues.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wups.h>

int32_t WUPSConfigItemMultipleValues_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
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

bool WUPSConfigItemMultipleValues_callCallback(void *context) {
    auto *item = (ConfigItemMultipleValues *) context;
    if (item->callback != nullptr && item->values && item->valueIndex >= 0 && item->valueIndex < item->valueCount) {
        ((MultipleValuesChangedCallback) (item->callback))(item, item->values[item->valueIndex].value);
        return true;
    }
    return false;
}

void WUPSConfigItemMultipleValues_onButtonPressed(void *context, WUPSConfigButtons buttons) {
    auto *item = (ConfigItemMultipleValues *) context;
    if (buttons & WUPS_CONFIG_BUTTON_LEFT) {
        item->valueIndex--;
    } else if (buttons & WUPS_CONFIG_BUTTON_RIGHT) {
        item->valueIndex++;
    }
    if (item->valueIndex < 0) {
        item->valueIndex = 0;
    } else if (item->valueIndex >= item->valueCount) {
        item->valueIndex = item->valueCount - 1;
    }
}

bool WUPSConfigItemMultipleValues_isMovementAllowed(void *context) {
    return true;
}

int32_t WUPSConfigItemMultipleValues_getCurrentValueSelectedDisplay(void *context, char *out_buf, int32_t out_size) {
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

void WUPSConfigItemMultipleValues_restoreDefault(void *context) {
    auto *item       = (ConfigItemMultipleValues *) context;
    item->valueIndex = item->defaultValueIndex;
}

void WUPSConfigItemMultipleValues_onSelected(void *context, bool isSelected) {
}

static void WUPSConfigItemMultipleValues_Cleanup(ConfigItemMultipleValues *item) {
    if (!item) {
        return;
    }

    for (int i = 0; i < item->valueCount; ++i) {
        free(item->values[i].valueName);
    }

    free(item->configId);
    free(item->values);

    free(item);
}

void WUPSConfigItemMultipleValues_onDelete(void *context) {
    auto *item = (ConfigItemMultipleValues *) context;

    WUPSConfigItemMultipleValues_Cleanup(item);
}

extern "C" bool
WUPSConfigItemMultipleValues_AddToCategory(WUPSConfigCategoryHandle cat, const char *configId, const char *displayName,
                                           int32_t defaultValueIndex, ConfigItemMultipleValuesPair *possibleValues,
                                           int pairCount, MultipleValuesChangedCallback callback) {
    if (cat == 0 || displayName == nullptr || possibleValues == nullptr || pairCount < 0) {
        return false;
    }
    auto *item = (ConfigItemMultipleValues *) malloc(sizeof(ConfigItemMultipleValues));
    if (item == nullptr) {
        return false;
    }

    auto *values = (ConfigItemMultipleValuesPair *) malloc(sizeof(ConfigItemMultipleValuesPair) * pairCount);

    for (int i = 0; i < pairCount; ++i) {
        values[i].value = possibleValues[i].value;
        if (possibleValues[i].valueName == nullptr) {
            values[i].valueName = nullptr;
            continue;
        }
        values[i].valueName = strdup(possibleValues[i].valueName);
    }

    item->valueCount        = pairCount;
    item->values            = values;
    item->valueIndex        = defaultValueIndex;
    item->defaultValueIndex = defaultValueIndex;
    item->callback          = (void *) callback;

    if (configId != nullptr) {
        item->configId = strdup(configId);
    } else {
        item->configId = nullptr;
    }

    WUPSConfigCallbacks_t callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemMultipleValues_getCurrentValueDisplay,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemMultipleValues_getCurrentValueSelectedDisplay,
            .onSelected                     = &WUPSConfigItemMultipleValues_onSelected,
            .restoreDefault                 = &WUPSConfigItemMultipleValues_restoreDefault,
            .isMovementAllowed              = &WUPSConfigItemMultipleValues_isMovementAllowed,
            .callCallback                   = &WUPSConfigItemMultipleValues_callCallback,
            .onButtonPressed                = &WUPSConfigItemMultipleValues_onButtonPressed,
            .onDelete                       = &WUPSConfigItemMultipleValues_onDelete};

    if (WUPSConfigItem_Create(&item->handle, configId, displayName, callbacks, item) < 0) {
        WUPSConfigItemMultipleValues_Cleanup(item);
        return false;
    }

    if (WUPSConfigCategory_AddItem(cat, item->handle) < 0) {
        WUPSConfigItem_Destroy(item->handle);
        return false;
    }
    return true;
}