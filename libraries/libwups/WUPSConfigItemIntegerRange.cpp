#include <wups.h>
#include <cstdio>
#include <cstdlib>
#include "LinkedList.h"
#include "wups/config/WUPSConfigItemIntegerRange.h"

int32_t WUPSConfigItemIntegerRange_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemIntegerRange *) context;
    return snprintf(out_buf, out_size, "%d", item->value);
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
        return snprintf(out_buf, out_size, "  %d >", item->value);
    } else if (item->value == item->maxValue) {
        return snprintf(out_buf, out_size, "<  %d", item->value);
    }
    return snprintf(out_buf, out_size, "< %d >", item->value);
}

void WUPSConfigItemIntegerRange_restoreDefault(void *context) {
    auto *item = (ConfigItemIntegerRange *) context;
    item->value = item->defaultValue;
}

LinkedList<ConfigItemIntegerRange *> wups_configItemIntegerRangeItems;

extern "C" WUPSConfigItemHandle WUPSConfigItemIntegerRange_Create(const char *configID, const char *displayName, int32_t defaultValue, int32_t minValue, int32_t maxValue, IntegerRangeValueChangedCallback callback) {
    auto *item = (ConfigItemIntegerRange *) malloc(sizeof(ConfigItemIntegerRange *));
    if (item == nullptr) {
        return 0;
    }

    item->defaultValue = defaultValue;
    item->value = defaultValue;
    item->minValue = minValue;
    item->maxValue = maxValue;
    item->callback = (void *) callback;

    WUPSConfigCallbacks_t callbacks{};

    callbacks.getCurrentValueDisplay = &WUPSConfigItemIntegerRange_getCurrentValueDisplay;
    callbacks.callCallback = &WUPSConfigItemIntegerRange_callCallback;
    callbacks.getCurrentValueSelectedDisplay = &WUPSConfigItemIntegerRange_getCurrentValueSelectedDisplay;
    callbacks.isMovementAllowed = &WUPSConfigItemIntegerRange_isMovementAllowed;
    callbacks.onButtonPressed = &WUPSConfigItemIntegerRange_onButtonPressed;
    callbacks.restoreDefault = &WUPSConfigItemIntegerRange_restoreDefault;

    if (WUPSConfigItem_Create(&item->handle, configID, displayName, callbacks, item) < 0) {
        free(item);
        return 0;
    };

    wups_configItemIntegerRangeItems.add(item);

    return item->handle;
}

extern "C" void WUPSConfigItemIntegerRange_CleanUp() {
    int size = wups_configItemIntegerRangeItems.size();
    for (int i = 0; i < size; i++) {
        delete wups_configItemIntegerRangeItems[i];
    }
    wups_configItemIntegerRangeItems.clear();
}