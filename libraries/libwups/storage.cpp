#include <cstdlib>
#include <cstring>
#include <wups.h>

#include "utils/base64.h"

static OpenStorageFunction openfunction_ptr __attribute__((section(".data")))   = nullptr;
static CloseStorageFunction closefunction_ptr __attribute__((section(".data"))) = nullptr;
static const char *plugin_id __attribute__((section(".data")))                  = nullptr;

static uint32_t storage_initialized __attribute__((section(".data"))) = false;
static uint32_t isOpened __attribute__((section(".data")));
static uint32_t isDirty __attribute__((section(".data")));
static wups_storage_item_t rootItem __attribute__((section(".data")));

void WUPS_InitStorage(wups_loader_init_storage_args_t args) {
    openfunction_ptr  = args.open_storage_ptr;
    closefunction_ptr = args.close_storage_ptr;
    plugin_id         = args.plugin_id;

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

static wups_storage_item_t *addItem(wups_storage_item_t *parent, const char *key, wups_storage_type_t type) {
    wups_storage_item_t *foundItem = nullptr;
    for (uint32_t i = 0; i < parent->data_size; i++) {
        wups_storage_item_t *item = &((wups_storage_item_t *) parent->data)[i];

        if (strcmp(item->key, key) == 0) {
            free(item->data);
            foundItem = item;
            break;
        }

        if (item->pending_delete) {
            free(item->data);
            free(item->key);

            item->key = (char *) malloc(strlen(key) + 1);
            strcpy(item->key, key);

            foundItem = item;
            break;
        }
    }

    if (!foundItem) {
        parent->data_size++;
        parent->data = (wups_storage_item_t *) realloc(parent->data, parent->data_size * sizeof(wups_storage_item_t));

        foundItem = &((wups_storage_item_t *) parent->data)[parent->data_size - 1];

        foundItem->key = (char *) malloc(strlen(key) + 1);
        strcpy(foundItem->key, key);
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

    wups_storage_item_t *item = addItem(parent, key, WUPS_STORAGE_TYPE_ITEM);

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

    wups_storage_item_t *item = addItem(parent, key, WUPS_STORAGE_TYPE_STRING);

    uint32_t size   = strlen(string) + 1;
    item->data      = malloc(size);
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

    wups_storage_item_t *item = addItem(parent, key, WUPS_STORAGE_TYPE_INT);

    item->data              = malloc(sizeof(int32_t));
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

    wups_storage_item_t *item = addItem(parent, key, WUPS_STORAGE_TYPE_STRING);

    item->data      = b64_encode((const uint8_t *) data, size);
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
