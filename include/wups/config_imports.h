#pragma once

#include "config.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t WUPSConfigItem_Create(WUPSConfigItemHandle *out, const char *configID, const char *displayName, WUPSConfigCallbacks_t callbacks, void *context);

int32_t WUPSConfigItem_Destroy(WUPSConfigItemHandle handle);

int32_t WUPSConfig_Destroy(WUPSConfigHandle handle);

int32_t WUPSConfigItem_SetDisplayName(WUPSConfigItemHandle handle, const char *displayName);

int32_t WUPSConfigItem_GetDisplayName(WUPSConfigItemHandle handle, char *out_buf, int32_t out_len);

int32_t WUPSConfigItem_SetConfigID(WUPSConfigItemHandle handle, const char *configID);

int32_t WUPSConfigItem_GetConfigID(WUPSConfigItemHandle handle, char *out_buf, int32_t out);

int32_t WUPSConfig_Create(WUPSConfigHandle *out, const char *name);

int32_t WUPSConfigCategory_Destroy(WUPSConfigCategoryHandle handle);

int32_t WUPSConfig_GetName(WUPSConfigHandle handle, char *out_buf, int32_t out_len);

int32_t WUPSConfig_AddCategoryByName(WUPSConfigHandle handle, const char *categoryName, WUPSConfigCategoryHandle *out);

int32_t WUPSConfig_AddCategory(WUPSConfigHandle handle, WUPSConfigCategoryHandle category);

/*
int32_t WUPSConfig_GetCategoryCount(WUPSConfigHandle handle, int32_t *category_count);

int32_t WUPSConfig_GetCategories(WUPSConfigHandle handle, WUPSConfigCategoryHandle *categories_out, int32_t categories_out_size);
 */

int32_t WUPSConfigCategory_Create(WUPSConfigCategoryHandle *out, const char *name);

int32_t WUPSConfigCategory_GetName(WUPSConfigCategoryHandle handle, char *out_buf, int32_t out_len);

int32_t WUPSConfigCategory_AddItem(WUPSConfigCategoryHandle handle, WUPSConfigItemHandle item_Handle);

#define WUPSConfig_AddCategoryByNameHandled(__config__, __categoryName__, __out__)     \
    do {                                                                               \
        if (WUPSConfig_AddCategoryByName(__config__, __categoryName__, __out__) < 0) { \
            WUPSConfig_Destroy(__config__);                                            \
            return 0;                                                                  \
        }                                                                              \
    } while (0)

#define WUPSConfig_CreateHandled(__config__, __configName__)     \
    do {                                                         \
        if (WUPSConfig_Create(__config__, __configName__) < 0) { \
            return 0;                                            \
        }                                                        \
    } while (0)

#ifdef __cplusplus
}
#endif