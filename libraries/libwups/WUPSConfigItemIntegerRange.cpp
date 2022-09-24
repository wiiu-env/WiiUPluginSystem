#include "wups/config/WUPSConfigItemIntegerRange.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wups.h>

int32_t WUPSConfigItemIntegerRange_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemIntegerRange *) context;
    snprintf(out_buf, out_size, "%d", item->value);
    return 0;
}

bool WUPSConfigItemIntegerRange_callCallback(void *context) {
    auto *item = (ConfigItemIntegerRange *) context;
    if (item->callback != nullptr) {
        ((IntegerRangeValueChangedCallback) item->callback)(item, item->value);
        return true;
    }
    return false;
}

void WUPSConfigItemIntegerRange_onButtonPressed(void *context, WUPSConfigButtons buttons) {
    auto *item = (ConfigItemIntegerRange *) context;
    if (buttons & WUPS_CONFIG_BUTTON_LEFT) {
        item->value--;
    } else if ((buttons & WUPS_CONFIG_BUTTON_RIGHT)) {
        item->value++;
    } else if ((buttons & WUPS_CONFIG_BUTTON_L)) {
        item->value = item->value - 50;
    } else if ((buttons & WUPS_CONFIG_BUTTON_R)) {
        item->value = item->value + 50;
    }

    if (item->value < item->minValue) {
        item->value = item->minValue;
    } else if (item->value > item->maxValue) {
        item->value = item->maxValue;
    }
}

bool WUPSConfigItemIntegerRange_isMovementAllowed(void *context) {
    return true;
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

void WUPSConfigItemIntegerRange_onDelete(void *context) {
    auto *item = (ConfigItemIntegerRange *) context;
    free(item->configId);
    free(item);
}

void WUPSConfigItemIntegerRange_onSelected(void *context, bool isSelected) {
}

extern "C" bool WUPSConfigItemIntegerRange_AddToCategory(WUPSConfigCategoryHandle cat, const char *configId, const char *displayName, int32_t defaultValue, int32_t minValue, int32_t maxValue,
                                                         IntegerRangeValueChangedCallback callback) {
    if (cat == 0) {
        return false;
    }
    auto *item = (ConfigItemIntegerRange *) malloc(sizeof(ConfigItemIntegerRange));
    if (item == nullptr) {
        return false;
    }

    if (configId != nullptr) {
        item->configId = strdup(configId);
    } else {
        item->configId = nullptr;
    }

    item->defaultValue = defaultValue;
    item->value        = defaultValue;
    item->minValue     = minValue;
    item->maxValue     = maxValue;
    item->callback     = (void *) callback;

    WUPSConfigCallbacks_t callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemIntegerRange_getCurrentValueDisplay,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemIntegerRange_getCurrentValueSelectedDisplay,
            .onSelected                     = &WUPSConfigItemIntegerRange_onSelected,
            .restoreDefault                 = &WUPSConfigItemIntegerRange_restoreDefault,
            .isMovementAllowed              = &WUPSConfigItemIntegerRange_isMovementAllowed,
            .callCallback                   = &WUPSConfigItemIntegerRange_callCallback,
            .onButtonPressed                = &WUPSConfigItemIntegerRange_onButtonPressed,
            .onDelete                       = &WUPSConfigItemIntegerRange_onDelete};

    if (WUPSConfigItem_Create(&(item->handle), configId, displayName, callbacks, item) < 0) {
        free(item);
        return false;
    };

    if (WUPSConfigCategory_AddItem(cat, item->handle) < 0) {
        WUPSConfigItem_Destroy(item->handle);
        return false;
    }
    return true;
}