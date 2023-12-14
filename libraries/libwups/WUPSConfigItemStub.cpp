#include "wups/config/WUPSConfigItemStub.h"
#include "wups/config_api.h"
#include <cstdlib>
#include <cstring>
#include <wups/config.h>

static int32_t WUPSConfigItemStub_getEmptyTextValue(void *context, char *out_buf, int32_t out_size) {
    memset(out_buf, 0, out_size);
    return 0;
}

static void WUPSConfigItemStub_Cleanup(void *item) {
    free(item);
}

extern "C" WUPSConfigAPIStatus
WUPSConfigItemStub_Create(const char *displayName, WUPSConfigItemHandle *outHandle) {
    if (displayName == nullptr || outHandle == nullptr) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }

    auto *item = (ConfigItemStub *) malloc(sizeof(ConfigItemStub));
    if (item == nullptr) {
        return WUPSCONFIG_API_RESULT_OUT_OF_MEMORY;
    }

    WUPSConfigAPIItemCallbacksV2 callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemStub_getEmptyTextValue,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemStub_getEmptyTextValue,
            .onSelected                     = nullptr,
            .restoreDefault                 = nullptr,
            .isMovementAllowed              = nullptr,
            .onCloseCallback                = nullptr,
            .onInput                        = nullptr,
            .onInputEx                      = nullptr,
            .onDelete                       = &WUPSConfigItemStub_Cleanup,
    };

    WUPSConfigAPIItemOptionsV2 options = {
            .displayName = displayName,
            .context     = item,
            .callbacks   = callbacks,
    };

    WUPSConfigAPIStatus err;
    if ((err = WUPSConfigAPI_Item_Create(options, &item->handle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WUPSConfigItemStub_Cleanup(item);
        return err;
    }

    *outHandle = item->handle;

    return WUPSCONFIG_API_RESULT_SUCCESS;
}

extern "C" WUPSConfigAPIStatus
WUPSConfigItemStub_AddToCategory(WUPSConfigCategoryHandle cat, const char *displayName) {
    WUPSConfigItemHandle itemHandle;
    WUPSConfigAPIStatus res;
    if ((res = WUPSConfigItemStub_Create(displayName,
                                         &itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return res;
    }
    if ((res = WUPSConfigAPI_Category_AddItem(cat, itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WUPSConfigAPI_Item_Destroy(itemHandle);
        return res;
    }
    return WUPSCONFIG_API_RESULT_SUCCESS;
}