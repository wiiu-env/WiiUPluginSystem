#pragma once

#include "config.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef WUPSConfigAPICallbackStatus (*WUPSConfigAPI_MenuOpenedCallback)(WUPSConfigCategoryHandle root);
typedef void (*WUPSConfigAPI_MenuClosedCallback)();

WUPSConfigAPIStatus WUPSConfigAPI_InitEx(uint32_t pluginIdentifier, WUPSConfigAPIOptions, WUPSConfigAPI_MenuOpenedCallback, WUPSConfigAPI_MenuClosedCallback);

WUPSConfigAPIStatus WUPSConfigAPI_Init(WUPSConfigAPIOptionsV1 optionsV1, WUPSConfigAPI_MenuOpenedCallback openedCallback, WUPSConfigAPI_MenuClosedCallback closedCallback);

WUPSConfigAPIStatus WUPSConfigAPI_GetVersion(WUPSConfigAPIVersion *outVariable);

WUPSConfigAPIStatus WUPSConfigAPI_Category_CreateEx(WUPSConfigAPICreateCategoryOptions options, WUPSConfigCategoryHandle *out);

static inline WUPSConfigAPIStatus WUPSConfigAPI_Category_Create(WUPSConfigAPICreateCategoryOptionsV1 options, WUPSConfigCategoryHandle *out) {
    WUPSConfigAPICreateCategoryOptions optionsWrapper = {
            .version = WUPS_API_CATEGORY_OPTION_VERSION_V1,
            .data    = {.v1 = options},
    };
    return WUPSConfigAPI_Category_CreateEx(optionsWrapper, out);
}

WUPSConfigAPIStatus WUPSConfigAPI_Category_Destroy(WUPSConfigCategoryHandle handle);

WUPSConfigAPIStatus WUPSConfigAPI_Category_AddCategory(WUPSConfigCategoryHandle parentHandle, WUPSConfigCategoryHandle categoryHandle);

WUPSConfigAPIStatus WUPSConfigAPI_Category_AddItem(WUPSConfigCategoryHandle parentHandle, WUPSConfigItemHandle itemHandle);

WUPSConfigAPIStatus WUPSConfigAPI_Item_CreateEx(WUPSConfigAPICreateItemOptions options, WUPSConfigItemHandle *out);

static inline WUPSConfigAPIStatus WUPSConfigAPI_Item_Create(WUPSConfigAPIItemOptionsV1 options, WUPSConfigItemHandle *out) {
    WUPSConfigAPICreateItemOptions itemOptions = {
            .version = WUPS_API_ITEM_OPTION_VERSION_V1,
            .data    = {.v1 = options},
    };
    return WUPSConfigAPI_Item_CreateEx(itemOptions, out);
}

WUPSConfigAPIStatus WUPSConfigAPI_Item_Destroy(WUPSConfigItemHandle handle);

const char *WUPSConfigAPI_GetStatusStr(WUPSConfigAPIStatus status);

#ifdef __cplusplus
}
#endif