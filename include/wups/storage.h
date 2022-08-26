#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef enum wups_storage_type_t_ {
    WUPS_STORAGE_TYPE_INVALID,
    WUPS_STORAGE_TYPE_STRING,
    WUPS_STORAGE_TYPE_INT,
    WUPS_STORAGE_TYPE_ITEM,
} wups_storage_type_t;

typedef enum {
    WUPS_STORAGE_ERROR_SUCCESS                = 0,
    WUPS_STORAGE_ERROR_NOT_OPENED             = -1,
    WUPS_STORAGE_ERROR_ALREADY_OPENED         = -2,
    WUPS_STORAGE_ERROR_INVALID_ARGS           = -3,
    WUPS_STORAGE_ERROR_NOT_FOUND              = -4,
    WUPS_STORAGE_ERROR_NOT_INITIALIZED        = -5,
    WUPS_STORAGE_ERROR_INVALID_BACKEND_PARAMS = -6,
    WUPS_STORAGE_ERROR_INVALID_JSON           = -7,
    WUPS_STORAGE_ERROR_IO                     = -8,
    WUPS_STORAGE_ERROR_B64_DECODE_FAILED      = -9,
    WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL       = -10,
} WUPSStorageError;

typedef struct wups_storage_item_t_ {
    char *key;
    void *data;
    uint32_t data_size;
    uint32_t pending_delete;
    wups_storage_type_t type;
} wups_storage_item_t;

typedef WUPSStorageError (*OpenStorageFunction)(const char *plugin_id, wups_storage_item_t *items);
typedef WUPSStorageError (*CloseStorageFunction)(const char *plugin_id, wups_storage_item_t *items);

typedef struct wups_loader_init_storage_args_t_ {
    OpenStorageFunction open_storage_ptr;
    CloseStorageFunction close_storage_ptr;
    const char *plugin_id;
} wups_loader_init_storage_args_t;

/* called by backend */
void WUPS_InitStorage(wups_loader_init_storage_args_t args);

const char *WUPS_GetStorageStatusStr(WUPSStorageError status);

/* opens storage for reading and writing */
WUPSStorageError WUPS_OpenStorage(void);

/* closes storage and saves changes */
WUPSStorageError WUPS_CloseStorage(void);

/* deletes key from storage */
WUPSStorageError WUPS_DeleteItem(wups_storage_item_t *parent, const char *key);

/* returns the size of key on success, or an error code */
// TODO do we need this? what about binary data?
// int32_t WUPS_GetSize(const char* key);

WUPSStorageError WUPS_CreateSubItem(wups_storage_item_t *parent, const char *key, wups_storage_item_t **outItem);
WUPSStorageError WUPS_GetSubItem(wups_storage_item_t *parent, const char *key, wups_storage_item_t **outItem);

WUPSStorageError WUPS_StoreString(wups_storage_item_t *parent, const char *key, const char *string);
WUPSStorageError WUPS_StoreBool(wups_storage_item_t *parent, const char *key, bool value);
WUPSStorageError WUPS_StoreInt(wups_storage_item_t *parent, const char *key, int32_t value);
WUPSStorageError WUPS_StoreBinary(wups_storage_item_t *parent, const char *key, const void *data, uint32_t size);

WUPSStorageError WUPS_GetString(wups_storage_item_t *parent, const char *key, char *outString, uint32_t maxSize);
WUPSStorageError WUPS_GetBool(wups_storage_item_t *parent, const char *key, bool *outBool);
WUPSStorageError WUPS_GetInt(wups_storage_item_t *parent, const char *key, int32_t *outInt);
WUPSStorageError WUPS_GetBinary(wups_storage_item_t *parent, const char *key, void *outData, uint32_t maxSize);

#ifdef __cplusplus
}
#endif
