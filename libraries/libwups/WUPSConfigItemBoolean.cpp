#include "wups/config/WUPSConfigItemBoolean.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wups.h>

int32_t WUPSConfigItemBoolean_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemBoolean *) context;
    snprintf(out_buf, out_size, "  %s", item->value ? item->trueValue : item->falseValue);
    return 0;
}

void toggleValue(ConfigItemBoolean *item) {
    item->value = !item->value;
}

bool WUPSConfigItemBoolean_callCallback(void *context) {
    auto *item = (ConfigItemBoolean *) context;
    if (item->callback != nullptr) {
        ((BooleanValueChangedCallback) (item->callback))(item, item->value);
        return true;
    }
    return false;
}

void WUPSConfigItemBoolean_onButtonPressed(void *context, WUPSConfigButtons buttons) {
    auto *item = (ConfigItemBoolean *) context;
    if ((buttons & WUPS_CONFIG_BUTTON_A) == WUPS_CONFIG_BUTTON_A) {
        toggleValue(item);
    } else if (buttons & WUPS_CONFIG_BUTTON_LEFT && !item->value) {
        toggleValue(item);
    } else if ((buttons & WUPS_CONFIG_BUTTON_RIGHT) && item->value) {
        toggleValue(item);
    }
}

bool WUPSConfigItemBoolean_isMovementAllowed(void *context) {
    return true;
}

int32_t WUPSConfigItemBoolean_getCurrentValueSelectedDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemBoolean *) context;
    if (item->value) {
        snprintf(out_buf, out_size, "  %s >", item->trueValue);
    } else {
        snprintf(out_buf, out_size, "< %s  ", item->falseValue);
    }
    return 0;
}

void WUPSConfigItemBoolean_restoreDefault(void *context) {
    auto *item  = (ConfigItemBoolean *) context;
    item->value = item->defaultValue;
}

void WUPSConfigItemBoolean_onSelected(void *context, bool isSelected) {
}

static void WUPSConfigItemBoolean_Cleanup(ConfigItemBoolean *item) {
    if (!item) {
        return;
    }
    free(item->configId);
    free(item);
}

void WUPSConfigItemBoolean_onDelete(void *context) {
    WUPSConfigItemBoolean_Cleanup((ConfigItemBoolean *) context);
}

extern "C" bool
WUPSConfigItemBoolean_AddToCategoryEx(WUPSConfigCategoryHandle cat, const char *configId, const char *displayName, bool defaultValue, BooleanValueChangedCallback callback, const char *trueValue,
                                      const char *falseValue) {
    if (cat == 0) {
        return false;
    }
    auto *item = (ConfigItemBoolean *) malloc(sizeof(ConfigItemBoolean));
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
    item->callback     = (void *) callback;
    snprintf(item->trueValue, sizeof(item->trueValue), "%s", trueValue);
    snprintf(item->falseValue, sizeof(item->falseValue), "%s", falseValue);

    WUPSConfigCallbacks_t callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemBoolean_getCurrentValueDisplay,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemBoolean_getCurrentValueSelectedDisplay,
            .onSelected                     = &WUPSConfigItemBoolean_onSelected,
            .restoreDefault                 = &WUPSConfigItemBoolean_restoreDefault,
            .isMovementAllowed              = &WUPSConfigItemBoolean_isMovementAllowed,
            .callCallback                   = &WUPSConfigItemBoolean_callCallback,
            .onButtonPressed                = &WUPSConfigItemBoolean_onButtonPressed,
            .onDelete                       = &WUPSConfigItemBoolean_onDelete};

    if (WUPSConfigItem_Create(&item->handle, configId, displayName, callbacks, item) < 0) {
        WUPSConfigItemBoolean_Cleanup(item);
        return false;
    }

    if (WUPSConfigCategory_AddItem(cat, item->handle) < 0) {
        WUPSConfigItem_Destroy(item->handle);
        return false;
    }
    return true;
}

extern "C" bool WUPSConfigItemBoolean_AddToCategory(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName, bool defaultValue, BooleanValueChangedCallback callback) {
    return WUPSConfigItemBoolean_AddToCategoryEx(cat, configID, displayName, defaultValue, callback, "true", "false");
}
