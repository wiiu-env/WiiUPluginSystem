#include "wups/config/WUPSConfigItemStub.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wups.h>

int32_t WUPSConfigItemStub_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    memset(out_buf, 0, out_size);
    return 0;
}

bool WUPSConfigItemStub_callCallback(void *context) {
    return false;
}

void WUPSConfigItemStub_onButtonPressed(void *context, WUPSConfigButtons buttons) {
}

bool WUPSConfigItemStub_isMovementAllowed(void *context) {
    return true;
}

int32_t WUPSConfigItemStub_getCurrentValueSelectedDisplay(void *context, char *out_buf, int32_t out_size) {
    memset(out_buf, 0, out_size);
    return 0;
}

void WUPSConfigItemStub_restoreDefault(void *context) {
}

void WUPSConfigItemStub_onSelected(void *context, bool isSelected) {
}

static void WUPSConfigItemStub_Cleanup(ConfigItemStub *item) {
    free(item);
}

void WUPSConfigItemStub_onDelete(void *context) {
    WUPSConfigItemStub_Cleanup((ConfigItemStub *) context);
}


extern "C" bool
WUPSConfigItemStub_AddToCategoryEx(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName) {
    if (cat == 0) {
        return false;
    }

    auto *item = (ConfigItemStub *) malloc(sizeof(ConfigItemStub));
    if (item == nullptr) {
        return false;
    }

    WUPSConfigCallbacks_t callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemStub_getCurrentValueDisplay,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemStub_getCurrentValueSelectedDisplay,
            .onSelected                     = &WUPSConfigItemStub_onSelected,
            .restoreDefault                 = &WUPSConfigItemStub_restoreDefault,
            .isMovementAllowed              = &WUPSConfigItemStub_isMovementAllowed,
            .callCallback                   = &WUPSConfigItemStub_callCallback,
            .onButtonPressed                = &WUPSConfigItemStub_onButtonPressed,
            .onDelete                       = &WUPSConfigItemStub_onDelete};

    if (WUPSConfigItem_Create(&item->handle, configID, displayName, callbacks, item) < 0) {
        WUPSConfigItemStub_Cleanup(item);
        return false;
    }

    if (WUPSConfigCategory_AddItem(cat, item->handle) < 0) {
        WUPSConfigItem_Destroy(item->handle);
        return false;
    }
    return true;
}

extern "C" bool WUPSConfigItemStub_AddToCategory(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName) {
    return WUPSConfigItemStub_AddToCategoryEx(cat, configID, displayName);
}
