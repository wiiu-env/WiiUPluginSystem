#include <cstring>
#include <wups/storage.h>

struct wups_internal_functions_t {
    WUPSStorage_SaveFunction save_function_ptr                     = nullptr;
    WUPSStorage_ForceReloadFunction force_reload_function_ptr      = nullptr;
    WUPSStorage_WipeStorageFunction wipe_storage_function_ptr      = nullptr;
    WUPSStorage_DeleteItemFunction delete_item_function_ptr        = nullptr;
    WUPSStorage_CreateSubItemFunction create_sub_item_function_ptr = nullptr;
    WUPSStorage_GetSubItemFunction get_sub_item_function_ptr       = nullptr;
    WUPSStorage_StoreItemFunction store_item_function_ptr          = nullptr;
    WUPSStorage_GetItemFunction get_item_function_ptr              = nullptr;
    WUPSStorage_GetItemSizeFunction get_item_size_function_ptr     = nullptr;
    wups_storage_root_item __storageroot_item                      = nullptr;
};

static wups_internal_functions_t __internal_functions __attribute__((section(".data"))) = {};

WUPSStorageError WUPSStorageAPI_InitInternal(wups_loader_init_storage_args_t args) {
    if (args.version > WUPS_STORAGE_CUR_API_VERSION) {
        __internal_functions = {};
        return WUPS_STORAGE_ERROR_INTERNAL_INVALID_VERSION;
    }
    __internal_functions.__storageroot_item           = args.root_item;
    __internal_functions.save_function_ptr            = args.save_function_ptr;
    __internal_functions.force_reload_function_ptr    = args.force_reload_function_ptr;
    __internal_functions.wipe_storage_function_ptr    = args.wipe_storage_function_ptr;
    __internal_functions.delete_item_function_ptr     = args.delete_item_function_ptr;
    __internal_functions.create_sub_item_function_ptr = args.create_sub_item_function_ptr;
    __internal_functions.get_sub_item_function_ptr    = args.get_sub_item_function_ptr;
    __internal_functions.store_item_function_ptr      = args.store_item_function_ptr;
    __internal_functions.get_item_function_ptr        = args.get_item_function_ptr;
    __internal_functions.get_item_size_function_ptr   = args.get_item_size_function_ptr;

    return WUPS_STORAGE_ERROR_SUCCESS;
}

const char *WUPSStorageAPI_GetStatusStr(WUPSStorageError status) {
    switch (status) {
        case WUPS_STORAGE_ERROR_SUCCESS:
            return "WUPS_STORAGE_ERROR_SUCCESS";
        case WUPS_STORAGE_ERROR_INVALID_ARGS:
            return "WUPS_STORAGE_ERROR_INVALID_ARGS";
        case WUPS_STORAGE_ERROR_MALLOC_FAILED:
            return "WUPS_STORAGE_ERROR_MALLOC_FAILED";
        case WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE:
            return "WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE";
        case WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL:
            return "WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL";
        case WUPS_STORAGE_ERROR_ALREADY_EXISTS:
            return "WUPS_STORAGE_ERROR_ALREADY_EXISTS";
        case WUPS_STORAGE_ERROR_IO_ERROR:
            return "WUPS_STORAGE_ERROR_IO_ERROR";
        case WUPS_STORAGE_ERROR_NOT_FOUND:
            return "WUPS_STORAGE_ERROR_NOT_FOUND";
        case WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED:
            return "WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED";
        case WUPS_STORAGE_ERROR_INTERNAL_INVALID_VERSION:
            return "WUPS_STORAGE_ERROR_INTERNAL_INVALID_VERSION";
        case WUPS_STORAGE_ERROR_UNKNOWN_ERROR:
            return "WUPS_STORAGE_ERROR_UNKNOWN_ERROR";
    }
    return "WUPS_STORAGE_ERROR_UNKNOWN";
}

WUPSStorageError WUPSStorageAPI_SaveStorage(bool force) {
    if (__internal_functions.save_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.__storageroot_item == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.save_function_ptr(__internal_functions.__storageroot_item, force);
}

WUPSStorageError WUPSStorageAPI_ForceReloadStorage() {
    if (__internal_functions.force_reload_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.__storageroot_item == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.force_reload_function_ptr(__internal_functions.__storageroot_item);
}

WUPSStorageError WUPSStorageAPI_WipeStorage() {
    if (__internal_functions.wipe_storage_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.__storageroot_item == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.wipe_storage_function_ptr(__internal_functions.__storageroot_item);
}

WUPSStorageError WUPSStorageAPI_DeleteItem(wups_storage_item parent, const char *key) {
    if (__internal_functions.delete_item_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.__storageroot_item == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.delete_item_function_ptr(__internal_functions.__storageroot_item, parent, key);
}

WUPSStorageError WUPSStorageAPI_CreateSubItem(wups_storage_item parent, const char *key, wups_storage_item *outItem) {
    if (__internal_functions.create_sub_item_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.__storageroot_item == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.create_sub_item_function_ptr(__internal_functions.__storageroot_item, parent, key, outItem);
}

WUPSStorageError WUPSStorageAPI_GetSubItem(wups_storage_item parent, const char *key, wups_storage_item *outItem) {
    if (__internal_functions.get_sub_item_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.__storageroot_item == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.get_sub_item_function_ptr(__internal_functions.__storageroot_item, parent, key, outItem);
}

WUPSStorageError WUPSStorageAPI_StoreItem(wups_storage_item parent, const char *key, WUPSStorageItemType type, void *data, uint32_t size) {
    if (__internal_functions.store_item_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.__storageroot_item == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.store_item_function_ptr(__internal_functions.__storageroot_item, parent, key, type, data, size);
}

WUPSStorageError WUPSStorageAPI_GetItem(wups_storage_item parent, const char *key, WUPSStorageItemType type, void *data, uint32_t maxSize, uint32_t *outSize) {
    if (data == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    if (__internal_functions.get_item_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.__storageroot_item == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.get_item_function_ptr(__internal_functions.__storageroot_item, parent, key, type, data, maxSize, outSize);
}


WUPSStorageError WUPSStorageAPI_GetItemSize(wups_storage_item parent, const char *key, uint32_t *outSize) {
    if (outSize == nullptr) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    if (__internal_functions.get_item_size_function_ptr == nullptr) {
        return WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.get_item_size_function_ptr(__internal_functions.__storageroot_item, parent, key, outSize);
}
