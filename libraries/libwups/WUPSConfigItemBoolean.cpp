#include <wups.h>
#include <cstdio>
#include <cstdlib>
#include "wups/config/WUPSConfigItemBoolean.h"

void WUPSConfigItemBoolean_onDelete(void *context);

int32_t WUPSConfigItemBoolean_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemBoolean *) context;
    if (item->value) {
        return snprintf(out_buf, out_size, "  %s", item->trueValue);
    } else {
        return snprintf(out_buf, out_size, "  %s", item->falseValue);
    }
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
        return snprintf(out_buf, out_size, "  %s >", item->trueValue);
    } else {
        return snprintf(out_buf, out_size, "< %s", item->falseValue);
    }
}

void WUPSConfigItemBoolean_restoreDefault(void *context) {
    auto *item = (ConfigItemBoolean *) context;
    item->value = item->defaultValue;
}

extern "C" bool WUPSConfigItemBoolean_AddToCategoryEx(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName, bool defaultValue, BooleanValueChangedCallback callback, const char *trueValue, const char *falseValue) {
    if (cat == 0) {
        return false;
    }
    auto *item = (ConfigItemBoolean *) malloc(sizeof(ConfigItemBoolean));
    if (item == nullptr) {
        return false;
    }

    item->defaultValue = defaultValue;
    item->value = defaultValue;
    item->callback = (void *) callback;
    snprintf(item->trueValue, sizeof(item->trueValue), "%s", trueValue);
    snprintf(item->falseValue, sizeof(item->falseValue), "%s", falseValue);

    WUPSConfigCallbacks_t callbacks{};

    callbacks.getCurrentValueDisplay = &WUPSConfigItemBoolean_getCurrentValueDisplay;
    callbacks.callCallback = &WUPSConfigItemBoolean_callCallback;
    callbacks.getCurrentValueSelectedDisplay = &WUPSConfigItemBoolean_getCurrentValueSelectedDisplay;
    callbacks.isMovementAllowed = &WUPSConfigItemBoolean_isMovementAllowed;
    callbacks.onButtonPressed = &WUPSConfigItemBoolean_onButtonPressed;
    callbacks.restoreDefault = &WUPSConfigItemBoolean_restoreDefault;
    callbacks.onDelete = &WUPSConfigItemBoolean_onDelete;

    if (WUPSConfigItem_Create(&item->handle, configID, displayName, callbacks, item) < 0) {
        free(item);
        return false;
    }

    if (WUPSConfigCategory_AddItem(cat, item->handle) < 0) {
        return false;
    }
    return true;
}

void WUPSConfigItemBoolean_onDelete(void *context) {
    auto *item = (ConfigItemBoolean *) context;
    free(item);
}

extern "C" bool WUPSConfigItemBoolean_AddToCategory(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName, bool defaultValue, BooleanValueChangedCallback callback) {
    return WUPSConfigItemBoolean_AddToCategoryEx(cat, configID, displayName, defaultValue, callback, "true", "false");
}
