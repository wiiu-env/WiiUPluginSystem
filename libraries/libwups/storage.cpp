#include <cstdlib>
#include <cstring>
#include <wups.h>

struct wups_internal_functions_t {
    OpenStorageFunction openfunction_ptr               = nullptr;
    CloseStorageFunction closefunction_ptr             = nullptr;
    DeleteItemFunction delete_item_function_ptr        = nullptr;
    CreateSubItemFunction create_sub_item_function_ptr = nullptr;
    GetSubItemFunction get_sub_item_function_ptr       = nullptr;
    StoreItemFunction store_item_function_ptr          = nullptr;
    GetItemFunction get_item_function_ptr              = nullptr;
    GetItemSizeFunction get_item_size_function_ptr     = nullptr;
    char plugin_id[256]{};
    wups_storage_root_item __storageroot_item = nullptr;
};

static wups_internal_functions_t __internal_functions __attribute__((section(".data"))) = {};

WUPSStorageError WUPS_InitStorage(wups_loader_init_storage_args_t args) {
    if (args.version != WUPS_STORAGE_CUR_API_VERSION) {
        __internal_functions = {};
        return WUPS_STORAGE_ERROR_INVALID_VERSION;
    }
    __internal_functions.openfunction_ptr             = args.open_storage_ptr;
    __internal_functions.closefunction_ptr            = args.close_storage_ptr;
    __internal_functions.delete_item_function_ptr     = args.delete_item_function_ptr;
    __internal_functions.create_sub_item_function_ptr = args.create_sub_item_function_ptr;
    __internal_functions.get_sub_item_function_ptr    = args.get_sub_item_function_ptr;
    __internal_functions.store_item_function_ptr      = args.store_item_function_ptr;
    __internal_functions.get_item_function_ptr        = args.get_item_function_ptr;
    __internal_functions.get_item_size_function_ptr   = args.get_item_size_function_ptr;

    strncpy(__internal_functions.plugin_id, args.plugin_id, sizeof(__internal_functions.plugin_id) - 1);
    return WUPS_STORAGE_ERROR_SUCCESS;
}

const char *WUPS_GetStorageStatusStr(WUPSStorageError status) {
    switch (status) {
        case WUPS_STORAGE_ERROR_SUCCESS:
            return "WUPS_STORAGE_ERROR_SUCCESS";
        case WUPS_STORAGE_ERROR_INVALID_ARGS:
            return "WUPS_STORAGE_ERROR_INVALID_ARGS";
        case WUPS_STORAGE_ERROR_INVALID_POINTER:
            return "WUPS_STORAGE_ERROR_INVALID_POINTER";
        case WUPS_STORAGE_ERROR_INVALID_VERSION:
            return "WUPS_STORAGE_ERROR_INVALID_VERSION";
        case WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE:
            return "WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE";
        case WUPS_STORAGE_ERROR_NOT_FOUND:
            return "WUPS_STORAGE_ERROR_NOT_FOUND";
        case WUPS_STORAGE_ERROR_MALLOC_FAILED:
            return "WUPS_STORAGE_ERROR_MALLOC_FAILED";
        case WUPS_STORAGE_ERROR_ALREADY_OPENED:
            return "WUPS_STORAGE_ERROR_ALREADY_OPENED";
        case WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL:
            return "WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL";
        case WUPS_STORAGE_ERROR_ALREADY_EXISTS:
            return "WUPS_STORAGE_ERROR_ALREADY_EXISTS";
        case WUPS_STORAGE_ERROR_UNKNOWN_ERROR:
            return "WUPS_STORAGE_ERROR_UNKNOWN_ERROR";
    }
    return "WUPS_STORAGE_ERROR_UNKNOWN";
}

WUPSStorageError WUPS_OpenStorage(void) {
    if (__internal_functions.openfunction_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_POINTER;
    }
    auto res = __internal_functions.openfunction_ptr(__internal_functions.plugin_id, &__internal_functions.__storageroot_item);
    if (res != WUPS_STORAGE_ERROR_SUCCESS) {
        __internal_functions.__storageroot_item = nullptr;
    }
    return res;
}

WUPSStorageError WUPS_CloseStorage(void) {
    if (__internal_functions.closefunction_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_POINTER;
    }
    auto res = __internal_functions.closefunction_ptr(__internal_functions.plugin_id);
    if (res == WUPS_STORAGE_ERROR_SUCCESS) {
        __internal_functions.__storageroot_item = nullptr;
    }
    return res;
}

WUPSStorageError WUPS_DeleteItem(wups_storage_item parent, const char *key) {
    if (__internal_functions.delete_item_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_POINTER;
    }
    return __internal_functions.delete_item_function_ptr(__internal_functions.__storageroot_item, parent, key);
}

WUPSStorageError WUPS_CreateSubItem(wups_storage_item parent, const char *key, wups_storage_item *outItem) {
    if (__internal_functions.create_sub_item_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_POINTER;
    }
    return __internal_functions.create_sub_item_function_ptr(__internal_functions.__storageroot_item, parent, key, outItem);
}

WUPSStorageError WUPS_GetSubItem(wups_storage_item parent, const char *key, wups_storage_item *outItem) {
    if (__internal_functions.get_sub_item_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_POINTER;
    }
    return __internal_functions.get_sub_item_function_ptr(__internal_functions.__storageroot_item, parent, key, outItem);
}

static inline WUPSStorageError WUPS_StoreItem(wups_storage_item parent, const char *key, WUPSStorageItemType type, void *data, uint32_t size) {
    if (__internal_functions.store_item_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_POINTER;
    }
    return __internal_functions.store_item_function_ptr(__internal_functions.__storageroot_item, parent, key, type, data, size);
}

WUPSStorageError WUPS_StoreString(wups_storage_item parent, const char *key, const char *value) {
    if (value == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPS_StoreItem(parent, key, WUPS_STORAGE_ITEM_STRING, (void *) value, strlen(value));
}

WUPSStorageError WUPS_StoreBool(wups_storage_item parent, const char *key, bool value) {
    return WUPS_StoreItem(parent, key, WUPS_STORAGE_ITEM_BOOL, (void *) &value, sizeof(bool));
}

WUPSStorageError WUPS_StoreInt(wups_storage_item parent, const char *key, int32_t value) {
    return WUPS_StoreItem(parent, key, WUPS_STORAGE_ITEM_S32, (void *) &value, sizeof(int32_t));
}

WUPSStorageError WUPS_StoreS32(wups_storage_item parent, const char *key, int32_t value) {
    return WUPS_StoreItem(parent, key, WUPS_STORAGE_ITEM_S32, (void *) &value, sizeof(int32_t));
}

WUPSStorageError WUPS_StoreS64(wups_storage_item parent, const char *key, int64_t value) {
    return WUPS_StoreItem(parent, key, WUPS_STORAGE_ITEM_S64, (void *) &value, sizeof(int64_t));
}

WUPSStorageError WUPS_StoreU32(wups_storage_item parent, const char *key, uint32_t value) {
    return WUPS_StoreItem(parent, key, WUPS_STORAGE_ITEM_U32, (void *) &value, sizeof(uint32_t));
}

WUPSStorageError WUPS_StoreU64(wups_storage_item parent, const char *key, uint64_t value) {
    return WUPS_StoreItem(parent, key, WUPS_STORAGE_ITEM_U64, (void *) &value, sizeof(uint64_t));
}

WUPSStorageError WUPS_StoreFloat(wups_storage_item parent, const char *key, float value) {
    return WUPS_StoreItem(parent, key, WUPS_STORAGE_ITEM_FLOAT, (void *) &value, sizeof(float));
}

WUPSStorageError WUPS_StoreDouble(wups_storage_item parent, const char *key, double value) {
    return WUPS_StoreItem(parent, key, WUPS_STORAGE_ITEM_DOUBLE, (void *) &value, sizeof(double));
}

WUPSStorageError WUPS_StoreBinary(wups_storage_item parent, const char *key, const void *data, uint32_t size) {
    if (data == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPS_StoreItem(parent, key, WUPS_STORAGE_ITEM_BINARY, (void *) data, size);
}

static inline WUPSStorageError WUPS_GetItem(wups_storage_item parent, const char *key, WUPSStorageItemType type, void *data, uint32_t maxSize, uint32_t *outSize) {
    if (__internal_functions.get_item_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_POINTER;
    }
    return __internal_functions.get_item_function_ptr(__internal_functions.__storageroot_item, parent, key, type, data, maxSize, outSize);
}

WUPSStorageError WUPS_GetString(wups_storage_item parent, const char *key, char *outString, uint32_t maxSize, uint32_t *outSize) {
    if (outString == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    return WUPS_GetItem(parent, key, WUPS_STORAGE_ITEM_STRING, outString, maxSize, outSize);
}

WUPSStorageError WUPS_GetBool(wups_storage_item parent, const char *key, bool *outValue) {
    if (outValue == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPS_GetItem(parent, key, WUPS_STORAGE_ITEM_BOOL, outValue, sizeof(*outValue), nullptr);
}

WUPSStorageError WUPS_GetInt(wups_storage_item parent, const char *key, int32_t *outValue) {
    if (outValue == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPS_GetItem(parent, key, WUPS_STORAGE_ITEM_S32, outValue, sizeof(*outValue), nullptr);
}

WUPSStorageError WUPS_GetS32(wups_storage_item parent, const char *key, int32_t *outValue) {
    if (outValue == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPS_GetItem(parent, key, WUPS_STORAGE_ITEM_S32, outValue, sizeof(*outValue), nullptr);
}

WUPSStorageError WUPS_GetS64(wups_storage_item parent, const char *key, int64_t *outValue) {
    if (outValue == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPS_GetItem(parent, key, WUPS_STORAGE_ITEM_S64, outValue, sizeof(*outValue), nullptr);
}

WUPSStorageError WUPS_GetU32(wups_storage_item parent, const char *key, uint32_t *outValue) {
    if (outValue == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPS_GetItem(parent, key, WUPS_STORAGE_ITEM_U32, outValue, sizeof(*outValue), nullptr);
}

WUPSStorageError WUPS_GetU64(wups_storage_item parent, const char *key, uint64_t *outValue) {
    if (outValue == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPS_GetItem(parent, key, WUPS_STORAGE_ITEM_U64, outValue, sizeof(*outValue), nullptr);
}

WUPSStorageError WUPS_GetFloat(wups_storage_item parent, const char *key, float *outValue) {
    if (outValue == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPS_GetItem(parent, key, WUPS_STORAGE_ITEM_FLOAT, outValue, sizeof(*outValue), nullptr);
}

WUPSStorageError WUPS_GetDouble(wups_storage_item parent, const char *key, double *outValue) {
    if (outValue == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPS_GetItem(parent, key, WUPS_STORAGE_ITEM_DOUBLE, outValue, sizeof(*outValue), nullptr);
}

WUPSStorageError WUPS_GetBinary(wups_storage_item parent, const char *key, void *outData, uint32_t maxSize, uint32_t *outValue) {
    if (outData == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    return WUPS_GetItem(parent, key, WUPS_STORAGE_ITEM_BINARY, outData, maxSize, outValue);
}

WUPSStorageError WUPS_GetItemSize(wups_storage_item parent, const char *key, uint32_t *outSize) {
    if (outSize == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }

    if (__internal_functions.get_item_size_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_POINTER;
    }
    return __internal_functions.get_item_size_function_ptr(__internal_functions.__storageroot_item, parent, key, outSize);
}
