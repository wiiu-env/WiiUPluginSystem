#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    WUPS_STORAGE_ERROR_SUCCESS              = 0,
    WUPS_STORAGE_ERROR_INVALID_ARGS         = 1,
    WUPS_STORAGE_ERROR_INVALID_POINTER      = 2,
    WUPS_STORAGE_ERROR_INVALID_VERSION      = 3,
    WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE = 4,
    WUPS_STORAGE_ERROR_NOT_FOUND            = 5,
    WUPS_STORAGE_ERROR_MALLOC_FAILED        = 6,
    WUPS_STORAGE_ERROR_ALREADY_OPENED       = 7,
    WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL     = 8,
    WUPS_STORAGE_ERROR_ALREADY_EXISTS       = 9,
    WUPS_STORAGE_ERROR_UNKNOWN_ERROR        = 10
} WUPSStorageError;

typedef enum {
    WUPS_STORAGE_ITEM_S32    = 0,
    WUPS_STORAGE_ITEM_S64    = 1,
    WUPS_STORAGE_ITEM_U32    = 2,
    WUPS_STORAGE_ITEM_U64    = 3,
    WUPS_STORAGE_ITEM_STRING = 4,
    WUPS_STORAGE_ITEM_BINARY = 5,
    WUPS_STORAGE_ITEM_BOOL   = 6,
    WUPS_STORAGE_ITEM_FLOAT  = 7,
    WUPS_STORAGE_ITEM_DOUBLE = 8,
} WUPSStorageItemTypes;

typedef uint32_t WUPSStorageItemType;

typedef void *wups_storage_root_item;
typedef void *wups_storage_item;

typedef WUPSStorageError (*OpenStorageFunction)(const char *plugin_id, wups_storage_root_item *root);
typedef WUPSStorageError (*CloseStorageFunction)(const char *plugin_id);
typedef WUPSStorageError (*DeleteItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key);
typedef WUPSStorageError (*CreateSubItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, wups_storage_item *outItem);
typedef WUPSStorageError (*GetSubItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, wups_storage_item *outItem);
typedef WUPSStorageError (*StoreItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, WUPSStorageItemType itemType, void *data, uint32_t length);
typedef WUPSStorageError (*GetItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, WUPSStorageItemType itemType, void *data, uint32_t maxSize, uint32_t *outSize);
typedef WUPSStorageError (*GetItemSizeFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, uint32_t *outSize);

typedef uint32_t WUPS_STORAGE_API_VERSION;
#define WUPS_STORAGE_CUR_API_VERSION 0x02

typedef struct wups_loader_init_storage_args_t_ {
    WUPS_STORAGE_API_VERSION version;
    OpenStorageFunction open_storage_ptr;
    CloseStorageFunction close_storage_ptr;
    DeleteItemFunction delete_item_function_ptr;
    CreateSubItemFunction create_sub_item_function_ptr;
    GetSubItemFunction get_sub_item_function_ptr;
    StoreItemFunction store_item_function_ptr;
    GetItemFunction get_item_function_ptr;
    GetItemSizeFunction get_item_size_function_ptr;
    const char *plugin_id;
} wups_loader_init_storage_args_t;

/* called by backend */
WUPSStorageError WUPS_InitStorage(wups_loader_init_storage_args_t args);

const char *WUPS_GetStorageStatusStr(WUPSStorageError status);

/* opens storage for reading and writing */
WUPSStorageError WUPS_OpenStorage(void);

/* closes storage and saves changes */
WUPSStorageError WUPS_CloseStorage(void);

/* deletes key from storage */
WUPSStorageError WUPS_DeleteItem(wups_storage_item parent, const char *key);

WUPSStorageError WUPS_CreateSubItem(wups_storage_item parent, const char *key, wups_storage_item *outItem);
WUPSStorageError WUPS_GetSubItem(wups_storage_item parent, const char *key, wups_storage_item *outItem);

WUPSStorageError WUPS_StoreString(wups_storage_item parent, const char *key, const char *string);
WUPSStorageError WUPS_StoreBool(wups_storage_item parent, const char *key, bool value);
WUPSStorageError WUPS_StoreInt(wups_storage_item parent, const char *key, int32_t value);
WUPSStorageError WUPS_StoreS32(wups_storage_item parent, const char *key, int32_t value);
WUPSStorageError WUPS_StoreS64(wups_storage_item parent, const char *key, int64_t value);
WUPSStorageError WUPS_StoreU32(wups_storage_item parent, const char *key, uint32_t value);
WUPSStorageError WUPS_StoreU64(wups_storage_item parent, const char *key, uint64_t value);
WUPSStorageError WUPS_StoreFloat(wups_storage_item parent, const char *key, float value);
WUPSStorageError WUPS_StoreDouble(wups_storage_item parent, const char *key, double value);
WUPSStorageError WUPS_StoreBinary(wups_storage_item parent, const char *key, const void *data, uint32_t size);

WUPSStorageError WUPS_GetString(wups_storage_item parent, const char *key, char *outString, uint32_t maxSize, uint32_t *outSize);
WUPSStorageError WUPS_GetBool(wups_storage_item parent, const char *key, bool *outValue);
WUPSStorageError WUPS_GetInt(wups_storage_item parent, const char *key, int32_t *outValue);
WUPSStorageError WUPS_GetS32(wups_storage_item parent, const char *key, int32_t *outValue);
WUPSStorageError WUPS_GetS64(wups_storage_item parent, const char *key, int64_t *outValue);
WUPSStorageError WUPS_GetU32(wups_storage_item parent, const char *key, uint32_t *outValue);
WUPSStorageError WUPS_GetU64(wups_storage_item parent, const char *key, uint64_t *outValue);
WUPSStorageError WUPS_GetFloat(wups_storage_item parent, const char *key, float *outValue);
WUPSStorageError WUPS_GetDouble(wups_storage_item parent, const char *key, double *outValue);
WUPSStorageError WUPS_GetBinary(wups_storage_item parent, const char *key, void *outData, uint32_t maxSize, uint32_t *outSize);

WUPSStorageError WUPS_GetItemSize(wups_storage_item parent, const char *key, uint32_t *outSize);

#ifdef __cplusplus
}
#endif
