#include <cstdlib>
#include <cstring>
#include <wups.h>

#include "utils/base64.h"

static OpenStorageFunction openfunction_ptr __attribute__((section(".data")))   = nullptr;
static CloseStorageFunction closefunction_ptr __attribute__((section(".data"))) = nullptr;
static char plugin_id[256] __attribute__((section(".data")));

static uint32_t storage_initialized __attribute__((section(".data"))) = false;
static uint32_t isOpened __attribute__((section(".data")));
static uint32_t isDirty __attribute__((section(".data")));
static wups_storage_item_t rootItem __attribute__((section(".data")));

void WUPS_InitStorage(wups_loader_init_storage_args_t args) {
    openfunction_ptr  = args.open_storage_ptr;
    closefunction_ptr = args.close_storage_ptr;
    strncpy(plugin_id, args.plugin_id, sizeof(plugin_id) - 1);

    storage_initialized = true;
    isOpened            = false;
    isDirty             = false;

    rootItem.key            = nullptr;
    rootItem.data           = nullptr;
    rootItem.data_size      = 0;
    rootItem.pending_delete = false;
    rootItem.type           = WUPS_STORAGE_TYPE_ITEM;
}

const char *WUPS_GetStorageStatusStr(WUPSStorageError status) {
    switch (status) {
        case WUPS_STORAGE_ERROR_SUCCESS:
            return "WUPS_STORAGE_ERROR_SUCCESS";
        case WUPS_STORAGE_ERROR_NOT_OPENED:
            return "WUPS_STORAGE_ERROR_NOT_OPENED";
        case WUPS_STORAGE_ERROR_ALREADY_OPENED:
            return "WUPS_STORAGE_ERROR_ALREADY_OPENED";
        case WUPS_STORAGE_ERROR_INVALID_ARGS:
            return "WUPS_STORAGE_ERROR_INVALID_ARGS";
        case WUPS_STORAGE_ERROR_NOT_FOUND:
            return "WUPS_STORAGE_ERROR_NOT_FOUND";
        case WUPS_STORAGE_ERROR_NOT_INITIALIZED:
            return "WUPS_STORAGE_ERROR_NOT_INITIALIZED";
        case WUPS_STORAGE_ERROR_INVALID_BACKEND_PARAMS:
            return "WUPS_STORAGE_ERROR_INVALID_BACKEND_PARAMS";
        case WUPS_STORAGE_ERROR_INVALID_JSON:
            return "WUPS_STORAGE_ERROR_INVALID_JSON";
        case WUPS_STORAGE_ERROR_IO:
            return "WUPS_STORAGE_ERROR_IO";
        case WUPS_STORAGE_ERROR_B64_DECODE_FAILED:
            return "WUPS_STORAGE_ERROR_B64_DECODE_FAILED";
        case WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL:
            return "WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL";
        case WUPS_STORAGE_ERROR_MALLOC_FAILED:
            return "WUPS_STORAGE_ERROR_MALLOC_FAILED";
        case WUPS_STORAGE_ERROR_SUB_ITEM_REALLOC:
            return "WUPS_STORAGE_ERROR_SUB_ITEM_REALLOC";
    }
    return "WUPS_STORAGE_ERROR_UNKNOWN";
}

WUPSStorageError WUPS_OpenStorage(void) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (isOpened) {
        return WUPS_STORAGE_ERROR_ALREADY_OPENED;
    }

    WUPSStorageError result = openfunction_ptr(plugin_id, &rootItem);

    if (result == WUPS_STORAGE_ERROR_SUCCESS || result == WUPS_STORAGE_ERROR_INVALID_JSON) {
        isOpened = true;
        isDirty  = false;
    }

    return result;
}

static void closeItem(wups_storage_item_t *item) {
    if (!item) {
        return;
    }

    if (item->type == WUPS_STORAGE_TYPE_ITEM) {
        auto *items = (wups_storage_item_t *) item->data;
        for (uint32_t i = 0; i < item->data_size; i++) {
            closeItem(&items[i]);
        }
    }
    free(item->data);
    free(item->key);
}

WUPSStorageError WUPS_CloseStorage(void) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (!isOpened) {
        return WUPS_STORAGE_ERROR_NOT_OPENED;
    }

    WUPSStorageError result = WUPS_STORAGE_ERROR_SUCCESS;
    if (isDirty) {
        result = closefunction_ptr(plugin_id, &rootItem);
    }

    if (result == WUPS_STORAGE_ERROR_SUCCESS) {
        isOpened = false;
        isDirty  = false;

        closeItem(&rootItem);
        rootItem.data_size = 0;
        rootItem.data      = nullptr;
        rootItem.key       = nullptr;
    }

    return result;
}

WUPSStorageError WUPS_DeleteItem(wups_storage_item_t *parent, const char *key) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (!isOpened) {
        return WUPS_STORAGE_ERROR_NOT_OPENED;
    }

    if (!key) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    if (!parent) {
        parent = &rootItem;
    } else if (parent->type != WUPS_STORAGE_TYPE_ITEM) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    isDirty = true;

    for (uint32_t i = 0; i < parent->data_size; i++) {
        wups_storage_item_t *item = &((wups_storage_item_t *) parent->data)[i];

        if (item->pending_delete || item->type == WUPS_STORAGE_TYPE_INVALID) {
            continue;
        }

        if (strcmp(item->key, key) == 0) {
            item->pending_delete = true;
            return WUPS_STORAGE_ERROR_SUCCESS;
        }
    }

    return WUPS_STORAGE_ERROR_NOT_FOUND;
}

// int32_t WUPS_GetSize(const char* key) {
//     if (!storage_initialized) {
//         return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
//     }

//     if (!isOpened) {
//         return WUPS_STORAGE_ERROR_NOT_OPENED;
//     }

//     for (uint32_t i = 0; i < amount_of_items; i++) {
//         wups_loader_storage_item_t* item = &items[i];

//         if (item->pending_delete || item->type == WUPS_STORAGE_TYPE_INVALID) {
//             continue;
//         }

//         if (strcmp(item->key, key) == 0) {
//             return item->data_size;
//         }
//     }

//     return WUPS_STORAGE_ERROR_NOT_FOUND;
// }

static wups_storage_item_t *addItem(wups_storage_item_t *parent, const char *key, wups_storage_type_t type, WUPSStorageError *error) {
    wups_storage_item_t *foundItem = nullptr;
    // First check for existing item with the same name.
    for (uint32_t i = 0; i < parent->data_size; i++) {
        wups_storage_item_t *item = &((wups_storage_item_t *) parent->data)[i];

        if (item->key && strcmp(item->key, key) == 0) {
            free(item->data);
            foundItem = item;
            break;
        }
    }

    if (!foundItem) {
        // Then check if there are any deleted item we can override.
        for (uint32_t i = 0; i < parent->data_size; i++) {
            wups_storage_item_t *item = &((wups_storage_item_t *) parent->data)[i];

            if (item->pending_delete) {
                free(item->data);
                free(item->key);
                item->data = nullptr;
                item->key  = nullptr;

                item->key = strdup(key);
                if (item->key == nullptr) {
                    return nullptr;
                }

                foundItem = item;
                break;
            }
        }
    }

    if (!foundItem) {
        // Only allow allocation for WUPS_STORAGE_TYPE_ITEM if it's the first item.
        // The realloc would cause the outPtr of previous WUPS_CreateSubItem call to be invalid.
        if (parent->data_size > 0 && type == WUPS_STORAGE_TYPE_ITEM) {
            *error = WUPS_STORAGE_ERROR_SUB_ITEM_REALLOC;
            return nullptr;
        }

        // Increase in step of 8 to avoid realloc calls (at the costs of some memory).
        constexpr uint32_t INCREASE_SIZE_BY = 8;

        auto *newPtr = (wups_storage_item_t *) realloc(parent->data, (parent->data_size + INCREASE_SIZE_BY) * sizeof(wups_storage_item_t));
        if (newPtr == nullptr) {
            *error = WUPS_STORAGE_ERROR_MALLOC_FAILED;
            return nullptr;
        }
        parent->data = newPtr;

        for (uint32_t j = parent->data_size; j < INCREASE_SIZE_BY; j++) {
            auto curItem = &((wups_storage_item_t *) parent->data)[j];
            memset(curItem, 0, sizeof(wups_storage_item_t));
            curItem->pending_delete = true;
        }

        foundItem = &((wups_storage_item_t *) parent->data)[parent->data_size];

        parent->data_size += INCREASE_SIZE_BY;

        foundItem->key = strdup(key);
        if (foundItem->key == nullptr) {
            foundItem->pending_delete = true;
            *error                    = WUPS_STORAGE_ERROR_MALLOC_FAILED;
            return nullptr;
        }
    }

    foundItem->type           = type;
    foundItem->pending_delete = false;
    foundItem->data           = nullptr;
    foundItem->data_size      = 0;
    return foundItem;
}

WUPSStorageError WUPS_CreateSubItem(wups_storage_item_t *parent, const char *key, wups_storage_item_t **outItem) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (!isOpened) {
        return WUPS_STORAGE_ERROR_NOT_OPENED;
    }

    if (!key || !outItem) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    if (!parent) {
        parent = &rootItem;
    } else if (parent->type != WUPS_STORAGE_TYPE_ITEM) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    isDirty = true;

    WUPSStorageError error;
    wups_storage_item_t *item = addItem(parent, key, WUPS_STORAGE_TYPE_ITEM, &error);
    if (item == nullptr) {
        return error;
    }

    *outItem = item;
    return WUPS_STORAGE_ERROR_SUCCESS;
}

WUPSStorageError WUPS_GetSubItem(wups_storage_item_t *parent, const char *key, wups_storage_item_t **outItem) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (!isOpened) {
        return WUPS_STORAGE_ERROR_NOT_OPENED;
    }

    if (!key || outItem == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    if (!parent) {
        parent = &rootItem;
    } else if (parent->type != WUPS_STORAGE_TYPE_ITEM) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    for (uint32_t i = 0; i < parent->data_size; i++) {
        wups_storage_item_t *item = &((wups_storage_item_t *) parent->data)[i];

        if (item->pending_delete || item->type != WUPS_STORAGE_TYPE_ITEM) {
            continue;
        }

        if (strcmp(item->key, key) == 0) {
            *outItem = item;
            return WUPS_STORAGE_ERROR_SUCCESS;
        }
    }

    return WUPS_STORAGE_ERROR_NOT_FOUND;
}

WUPSStorageError WUPS_StoreString(wups_storage_item_t *parent, const char *key, const char *string) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (!isOpened) {
        return WUPS_STORAGE_ERROR_NOT_OPENED;
    }

    if (!key || !string) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    if (!parent) {
        parent = &rootItem;
    } else if (parent->type != WUPS_STORAGE_TYPE_ITEM) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    isDirty = true;

    WUPSStorageError error;
    wups_storage_item_t *item = addItem(parent, key, WUPS_STORAGE_TYPE_STRING, &error);
    if (item == nullptr) {
        return error;
    }

    uint32_t size = strlen(string) + 1;
    item->data    = malloc(size);
    if (item->data == nullptr) {
        item->key            = nullptr;
        item->pending_delete = true;
        return WUPS_STORAGE_ERROR_MALLOC_FAILED;
    }
    item->data_size = size;
    strcpy((char *) item->data, string);

    return WUPS_STORAGE_ERROR_SUCCESS;
}

WUPSStorageError WUPS_StoreBool(wups_storage_item_t *parent, const char *key, bool value) {
    return WUPS_StoreInt(parent, key, (int32_t) value);
}

WUPSStorageError WUPS_StoreInt(wups_storage_item_t *parent, const char *key, int32_t value) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (!isOpened) {
        return WUPS_STORAGE_ERROR_NOT_OPENED;
    }

    if (!key) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    if (!parent) {
        parent = &rootItem;
    } else if (parent->type != WUPS_STORAGE_TYPE_ITEM) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    isDirty = true;

    WUPSStorageError error;
    wups_storage_item_t *item = addItem(parent, key, WUPS_STORAGE_TYPE_INT, &error);
    if (item == nullptr) {
        return error;
    }

    item->data = malloc(sizeof(int32_t));
    if (item->data == nullptr) {
        item->key            = nullptr;
        item->pending_delete = true;
        return WUPS_STORAGE_ERROR_MALLOC_FAILED;
    }
    item->data_size         = sizeof(int32_t);
    *(int32_t *) item->data = value;

    return WUPS_STORAGE_ERROR_SUCCESS;
}

WUPSStorageError WUPS_StoreBinary(wups_storage_item_t *parent, const char *key, const void *data, uint32_t size) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (!isOpened) {
        return WUPS_STORAGE_ERROR_NOT_OPENED;
    }

    if (!key || !data || size == 0) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    if (!parent) {
        parent = &rootItem;
    } else if (parent->type != WUPS_STORAGE_TYPE_ITEM) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    isDirty = true;

    WUPSStorageError error;
    wups_storage_item_t *item = addItem(parent, key, WUPS_STORAGE_TYPE_STRING, &error);
    if (item == nullptr) {
        return error;
    }

    item->data = b64_encode((const uint8_t *) data, size);
    if (item->data == nullptr) {
        item->key            = nullptr;
        item->pending_delete = true;
        return WUPS_STORAGE_ERROR_MALLOC_FAILED;
    }
    item->data_size = strlen((char *) data) + 1;

    return WUPS_STORAGE_ERROR_SUCCESS;
}

WUPSStorageError WUPS_GetString(wups_storage_item_t *parent, const char *key, char *outString, uint32_t maxSize) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (!isOpened) {
        return WUPS_STORAGE_ERROR_NOT_OPENED;
    }

    if (!key || !outString || maxSize == 0) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    if (!parent) {
        parent = &rootItem;
    } else if (parent->type != WUPS_STORAGE_TYPE_ITEM) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    for (uint32_t i = 0; i < parent->data_size; i++) {
        wups_storage_item_t *item = &((wups_storage_item_t *) parent->data)[i];

        if (item->pending_delete || item->type != WUPS_STORAGE_TYPE_STRING) {
            continue;
        }

        if (strcmp(item->key, key) == 0) {
            strncpy(outString, (char *) item->data, maxSize);
            return WUPS_STORAGE_ERROR_SUCCESS;
        }
    }
    return WUPS_STORAGE_ERROR_NOT_FOUND;
}

WUPSStorageError WUPS_GetBool(wups_storage_item_t *parent, const char *key, bool *outBool) {
    int32_t out;
    WUPSStorageError result = WUPS_GetInt(parent, key, &out);
    if (result != WUPS_STORAGE_ERROR_SUCCESS) {
        return result;
    }

    *outBool = out != 0;

    return WUPS_STORAGE_ERROR_SUCCESS;
}

WUPSStorageError WUPS_GetInt(wups_storage_item_t *parent, const char *key, int32_t *outInt) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (!isOpened) {
        return WUPS_STORAGE_ERROR_NOT_OPENED;
    }

    if (!key || !outInt) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    if (!parent) {
        parent = &rootItem;
    } else if (parent->type != WUPS_STORAGE_TYPE_ITEM) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    for (uint32_t i = 0; i < parent->data_size; i++) {
        wups_storage_item_t *item = &((wups_storage_item_t *) parent->data)[i];

        if (item->pending_delete || item->type != WUPS_STORAGE_TYPE_INT) {
            continue;
        }

        if (strcmp(item->key, key) == 0) {
            *outInt = *(int32_t *) item->data;
            return WUPS_STORAGE_ERROR_SUCCESS;
        }
    }

    return WUPS_STORAGE_ERROR_NOT_FOUND;
}

WUPSStorageError WUPS_GetBinary(wups_storage_item_t *parent, const char *key, void *outData, uint32_t maxSize) {
    if (!storage_initialized) {
        return WUPS_STORAGE_ERROR_NOT_INITIALIZED;
    }

    if (!isOpened) {
        return WUPS_STORAGE_ERROR_NOT_OPENED;
    }

    if (!key || !outData || maxSize == 0) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    if (!parent) {
        parent = &rootItem;
    } else if (parent->type != WUPS_STORAGE_TYPE_ITEM) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    for (uint32_t i = 0; i < parent->data_size; i++) {
        wups_storage_item_t *item = &((wups_storage_item_t *) parent->data)[i];

        if (item->pending_delete || item->type != WUPS_STORAGE_TYPE_STRING) {
            continue;
        }

        if (strcmp(item->key, key) == 0) {
            if (b64_decoded_size((char *) item->data) > maxSize) {
                return WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL;
            }

            if (b64_decode((char *) item->data, (uint8_t *) outData, item->data_size)) {
                return WUPS_STORAGE_ERROR_SUCCESS;
            } else {
                return WUPS_STORAGE_ERROR_B64_DECODE_FAILED;
            }
        }
    }

    return WUPS_STORAGE_ERROR_NOT_FOUND;
}
