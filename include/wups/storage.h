#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef enum {
    WUPS_STORAGE_ERROR_SUCCESS                  = 0,
    WUPS_STORAGE_ERROR_INVALID_ARGS             = -0x01,
    WUPS_STORAGE_ERROR_MALLOC_FAILED            = -0x02,
    WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE     = -0x03,
    WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL         = -0x04,
    WUPS_STORAGE_ERROR_ALREADY_EXISTS           = -0x05,
    WUPS_STORAGE_ERROR_IO_ERROR                 = -0x06,
    WUPS_STORAGE_ERROR_NOT_FOUND                = -0x10,
    WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED = -0xF0,
    WUPS_STORAGE_ERROR_INTERNAL_INVALID_VERSION = -0xF1,
    WUPS_STORAGE_ERROR_UNKNOWN_ERROR            = -0x100,
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

typedef WUPSStorageError (*WUPSStorage_SaveFunction)(wups_storage_root_item root, bool force);
typedef WUPSStorageError (*WUPSStorage_ForceReloadFunction)(wups_storage_root_item root);
typedef WUPSStorageError (*WUPSStorage_WipeStorageFunction)(wups_storage_root_item root);
typedef WUPSStorageError (*WUPSStorage_DeleteItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key);
typedef WUPSStorageError (*WUPSStorage_CreateSubItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, wups_storage_item *outItem);
typedef WUPSStorageError (*WUPSStorage_GetSubItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, wups_storage_item *outItem);
typedef WUPSStorageError (*WUPSStorage_StoreItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, WUPSStorageItemType itemType, void *data, uint32_t length);
typedef WUPSStorageError (*WUPSStorage_GetItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, WUPSStorageItemType itemType, void *data, uint32_t maxSize, uint32_t *outSize);
typedef WUPSStorageError (*WUPSStorage_GetItemSizeFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, uint32_t *outSize);

typedef uint32_t WUPS_STORAGE_API_VERSION;
#define WUPS_STORAGE_CUR_API_VERSION 0x02

typedef struct wups_loader_init_storage_args_t_ {
    WUPS_STORAGE_API_VERSION version;
    wups_storage_root_item root_item;
    WUPSStorage_SaveFunction save_function_ptr;
    WUPSStorage_ForceReloadFunction force_reload_function_ptr;
    WUPSStorage_WipeStorageFunction wipe_storage_function_ptr;
    WUPSStorage_DeleteItemFunction delete_item_function_ptr;
    WUPSStorage_CreateSubItemFunction create_sub_item_function_ptr;
    WUPSStorage_GetSubItemFunction get_sub_item_function_ptr;
    WUPSStorage_StoreItemFunction store_item_function_ptr;
    WUPSStorage_GetItemFunction get_item_function_ptr;
    WUPSStorage_GetItemSizeFunction get_item_size_function_ptr;
} wups_loader_init_storage_args_t;

/* called by backend */
WUPSStorageError WUPSStorageAPI_InitInternal(wups_loader_init_storage_args_t args);

const char *WUPSStorageAPI_GetStatusStr(WUPSStorageError status);

WUPSStorageError WUPSStorageAPI_SaveStorage(bool forceSave);
WUPSStorageError WUPSStorageAPI_ForceReloadStorage();
WUPSStorageError WUPSStorageAPI_WipeStorage();

WUPSStorageError WUPSStorageAPI_DeleteItem(wups_storage_item parent, const char *key);

WUPSStorageError WUPSStorageAPI_CreateSubItem(wups_storage_item parent, const char *key, wups_storage_item *outItem);
WUPSStorageError WUPSStorageAPI_GetSubItem(wups_storage_item parent, const char *key, wups_storage_item *outItem);

WUPSStorageError WUPSStorageAPI_StoreItem(wups_storage_item parent, const char *key, WUPSStorageItemType type, void *data, uint32_t size);

inline WUPSStorageError WUPSStorageAPI_StoreString(wups_storage_item parent, const char *key, const char *value) {
    if (value == NULL) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_STRING, (void *) value, strlen(value));
}

inline WUPSStorageError WUPSStorageAPI_StoreBool(wups_storage_item parent, const char *key, bool value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_BOOL, (void *) &value, sizeof(bool));
}

inline WUPSStorageError WUPSStorageAPI_StoreInt(wups_storage_item parent, const char *key, int32_t value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_S32, (void *) &value, sizeof(int32_t));
}

inline WUPSStorageError WUPSStorageAPI_StoreS32(wups_storage_item parent, const char *key, int32_t value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_S32, (void *) &value, sizeof(int32_t));
}

inline WUPSStorageError WUPSStorageAPI_StoreS64(wups_storage_item parent, const char *key, int64_t value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_S64, (void *) &value, sizeof(int64_t));
}

inline WUPSStorageError WUPSStorageAPI_StoreU32(wups_storage_item parent, const char *key, uint32_t value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_U32, (void *) &value, sizeof(uint32_t));
}

inline WUPSStorageError WUPSStorageAPI_StoreU64(wups_storage_item parent, const char *key, uint64_t value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_U64, (void *) &value, sizeof(uint64_t));
}

inline WUPSStorageError WUPSStorageAPI_StoreFloat(wups_storage_item parent, const char *key, float value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_FLOAT, (void *) &value, sizeof(float));
}

inline WUPSStorageError WUPSStorageAPI_StoreDouble(wups_storage_item parent, const char *key, double value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_DOUBLE, (void *) &value, sizeof(double));
}

inline WUPSStorageError WUPSStorageAPI_StoreBinary(wups_storage_item parent, const char *key, const void *data, uint32_t size) {
    if (data == NULL) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_BINARY, (void *) data, size);
}

WUPSStorageError WUPSStorageAPI_GetItem(wups_storage_item parent, const char *key, WUPSStorageItemType type, void *data, uint32_t maxSize, uint32_t *outSize);

inline WUPSStorageError WUPSStorageAPI_GetString(wups_storage_item parent, const char *key, char *outString, uint32_t maxSize, uint32_t *outSize) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_STRING, outString, maxSize, outSize);
}

inline WUPSStorageError WUPSStorageAPI_GetBool(wups_storage_item parent, const char *key, bool *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_BOOL, outValue, sizeof(*outValue), NULL);
}

inline WUPSStorageError WUPSStorageAPI_GetInt(wups_storage_item parent, const char *key, int32_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_S32, outValue, sizeof(*outValue), NULL);
}

inline WUPSStorageError WUPSStorageAPI_GetS32(wups_storage_item parent, const char *key, int32_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_S32, outValue, sizeof(*outValue), NULL);
}

inline WUPSStorageError WUPSStorageAPI_GetS64(wups_storage_item parent, const char *key, int64_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_S64, outValue, sizeof(*outValue), NULL);
}

inline WUPSStorageError WUPSStorageAPI_GetU32(wups_storage_item parent, const char *key, uint32_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_U32, outValue, sizeof(*outValue), NULL);
}

inline WUPSStorageError WUPSStorageAPI_GetU64(wups_storage_item parent, const char *key, uint64_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_U64, outValue, sizeof(*outValue), NULL);
}

inline WUPSStorageError WUPSStorageAPI_GetFloat(wups_storage_item parent, const char *key, float *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_FLOAT, outValue, sizeof(*outValue), NULL);
}

inline WUPSStorageError WUPSStorageAPI_GetDouble(wups_storage_item parent, const char *key, double *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_DOUBLE, outValue, sizeof(*outValue), NULL);
}

inline WUPSStorageError WUPSStorageAPI_GetBinary(wups_storage_item parent, const char *key, void *outData, uint32_t maxSize, uint32_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_BINARY, outData, maxSize, outValue);
}

WUPSStorageError WUPSStorageAPI_GetItemSize(wups_storage_item parent, const char *key, uint32_t *outSize);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <cstring>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

class WUPSStorageSubItem;

namespace WUPSStorageAPI {
    WUPSStorageError DeleteItem(std::string_view key) noexcept;

    WUPSStorageError SaveStorage(bool forceSave = false);
    WUPSStorageError ForceReloadStorage();
    WUPSStorageError WipeStorage();

    std::string_view GetStatusStr(const WUPSStorageError &err) noexcept;

    WUPSStorageSubItem GetRootItem() noexcept;

    std::optional<WUPSStorageSubItem> CreateSubItem(std::string_view key, WUPSStorageError &err) noexcept;
    std::optional<WUPSStorageSubItem> GetSubItem(std::string_view key, WUPSStorageError &err) noexcept;
    std::optional<WUPSStorageSubItem> GetOrCreateSubItem(std::string_view key, WUPSStorageError &err) noexcept;

    WUPSStorageError GetItemSize(std::string_view key, uint32_t &outSize) noexcept;

    WUPSStorageSubItem CreateSubItem(std::string_view key);
    WUPSStorageSubItem GetSubItem(std::string_view key);
    WUPSStorageSubItem GetOrCreateSubItem(std::string_view key);

    template<typename T>
    inline WUPSStorageError GetEx(wups_storage_item parent, const char *key, T &outValue) noexcept;

    template<>
    inline WUPSStorageError GetEx<int32_t>(wups_storage_item parent, const char *key, int32_t &outValue) noexcept {
        return WUPSStorageAPI_GetS32(parent, key, &outValue);
    }

    template<>
    inline WUPSStorageError GetEx<int64_t>(wups_storage_item parent, const char *key, int64_t &outValue) noexcept {
        return WUPSStorageAPI_GetS64(parent, key, &outValue);
    }

    template<>
    inline WUPSStorageError GetEx<uint32_t>(wups_storage_item parent, const char *key, uint32_t &outValue) noexcept {
        return WUPSStorageAPI_GetU32(parent, key, &outValue);
    }

    template<>
    inline WUPSStorageError GetEx<uint64_t>(wups_storage_item parent, const char *key, uint64_t &outValue) noexcept {
        return WUPSStorageAPI_GetU64(parent, key, &outValue);
    }

    template<>
    inline WUPSStorageError GetEx<bool>(wups_storage_item parent, const char *key, bool &outValue) noexcept {
        return WUPSStorageAPI_GetBool(parent, key, &outValue);
    }

    template<>
    inline WUPSStorageError GetEx<float>(wups_storage_item parent, const char *key, float &outValue) noexcept {
        return WUPSStorageAPI_GetFloat(parent, key, &outValue);
    }

    template<>
    inline WUPSStorageError GetEx<double>(wups_storage_item parent, const char *key, double &outValue) noexcept {
        return WUPSStorageAPI_GetDouble(parent, key, &outValue);
    }

    template<typename T>
    inline WUPSStorageError GetEx(wups_storage_item parent, const char *key, T &outValue) noexcept {
        static_assert(sizeof(T) == sizeof(uint32_t) && std::is_enum<T>::value, "T must be an enum of size sizeof(uint32_t)");
        return WUPSStorageAPI_GetU32(parent, key, (uint32_t *) &outValue);
    }

    template<>
    inline WUPSStorageError GetEx<std::vector<uint8_t>>(wups_storage_item parent, const char *key, std::vector<uint8_t> &outValue) noexcept {
        uint32_t outSize = 0;

        if (outValue.empty()) {
            uint32_t resizeToSize = 0;
            auto r                = WUPSStorageAPI_GetItemSize(parent, key, &resizeToSize);
            if (r == WUPS_STORAGE_ERROR_SUCCESS) {
                outValue.resize(resizeToSize);
            } else {
                return r;
            }
        }

        auto res = WUPSStorageAPI_GetBinary(parent, key, outValue.data(), outValue.size(), &outSize);
        if (res == WUPS_STORAGE_ERROR_SUCCESS) {
            outValue.resize(outSize);
        } else {
            outValue.resize(0);
        }
        return res;
    }

    template<>
    inline WUPSStorageError GetEx<std::string>(wups_storage_item parent, const char *key, std::string &outValue) noexcept {
        uint32_t outSize = 0;
        if (outValue.empty()) {
            uint32_t resizeToSize = 0;
            auto r                = WUPSStorageAPI_GetItemSize(parent, key, &resizeToSize);
            if (r == WUPS_STORAGE_ERROR_SUCCESS) {
                outValue.resize(resizeToSize);
            } else {
                return r;
            }
        }

        auto res = WUPSStorageAPI_GetString(parent, key, outValue.data(), outValue.size(), &outSize);
        if (res == WUPS_STORAGE_ERROR_SUCCESS) {
            // outSize does count the null terminator as well, std::string's size() doesn't include a null terminator.
            outValue.resize(strlen(outValue.c_str()));
        } else {
            outValue.resize(0);
        }
        return res;
    }

    template<typename T>
    inline WUPSStorageError Get(const char *key, T &outValue) noexcept {
        return GetEx(nullptr, key, outValue);
    }

    template<typename T>
    inline WUPSStorageError StoreEx(wups_storage_item parent, const char *key, const T &value) noexcept;

    template<>
    inline WUPSStorageError StoreEx<int32_t>(wups_storage_item parent, const char *key, const int32_t &value) noexcept {
        return WUPSStorageAPI_StoreS32(parent, key, value);
    }

    template<>
    inline WUPSStorageError StoreEx<int64_t>(wups_storage_item parent, const char *key, const int64_t &value) noexcept {
        return WUPSStorageAPI_StoreS64(parent, key, value);
    }

    template<>
    inline WUPSStorageError StoreEx<uint32_t>(wups_storage_item parent, const char *key, const uint32_t &value) noexcept {
        return WUPSStorageAPI_StoreU32(parent, key, value);
    }

    template<>
    inline WUPSStorageError StoreEx<uint64_t>(wups_storage_item parent, const char *key, const uint64_t &value) noexcept {
        return WUPSStorageAPI_StoreU64(parent, key, value);
    }

    template<>
    inline WUPSStorageError StoreEx<bool>(wups_storage_item parent, const char *key, const bool &value) noexcept {
        return WUPSStorageAPI_StoreBool(parent, key, value);
    }

    template<>
    inline WUPSStorageError StoreEx<float>(wups_storage_item parent, const char *key, const float &value) noexcept {
        return WUPSStorageAPI_StoreFloat(parent, key, value);
    }

    template<>
    inline WUPSStorageError StoreEx<double>(wups_storage_item parent, const char *key, const double &value) noexcept {
        return WUPSStorageAPI_StoreDouble(parent, key, value);
    }

    template<>
    inline WUPSStorageError StoreEx<std::vector<uint8_t>>(wups_storage_item parent, const char *key, const std::vector<uint8_t> &value) noexcept {
        return WUPSStorageAPI_StoreBinary(parent, key, value.data(), value.size());
    }

    template<>
    inline WUPSStorageError StoreEx<std::string>(wups_storage_item parent, const char *key, const std::string &value) noexcept {
        return WUPSStorageAPI_StoreString(parent, key, value.c_str());
    }

    template<typename T>
    inline WUPSStorageError StoreEx(wups_storage_item parent, const char *key, const T &value) noexcept {
        static_assert(sizeof(T) == sizeof(uint32_t) && std::is_enum<T>::value, "T must be an enum of size sizeof(uint32_t)");
        return WUPSStorageAPI_StoreU32(parent, key, (uint32_t) value);
    }

    template<typename T>
    inline WUPSStorageError Store(const char *key, const T &value) noexcept {
        return StoreEx(nullptr, key, value);
    }

    template<typename T>
    inline WUPSStorageError GetOrStoreDefaultEx(wups_storage_item parent, const char *key, T &outValue, const T &defaultValue) noexcept {
        WUPSStorageError err = GetEx(parent, key, outValue);
        if (err == WUPS_STORAGE_ERROR_NOT_FOUND) {
            err = StoreEx(parent, key, defaultValue);
        }
        return err;
    }

    template<typename T>
    inline WUPSStorageError GetOrStoreDefault(const char *key, T &outValue, const T &defaultValue) noexcept {
        return GetOrStoreDefaultEx<T>(nullptr, key, outValue, defaultValue);
    }
} // namespace WUPSStorageAPI

class WUPSStorageSubItem {
public:
    explicit WUPSStorageSubItem(wups_storage_item handle) : mHandle(handle) {
    }
    virtual ~WUPSStorageSubItem() = default;

    bool operator==(const WUPSStorageSubItem &rhs) const;
    bool operator!=(const WUPSStorageSubItem &rhs) const;

    template<typename T>
    inline WUPSStorageError Store(const char *key, const T &value) noexcept {
        return WUPSStorageAPI::StoreEx(mHandle, key, value);
    }

    template<typename T>
    inline WUPSStorageError Get(const char *key, T &value) const noexcept {
        return WUPSStorageAPI::GetEx(mHandle, key, value);
    }

    template<typename T>
    inline WUPSStorageError GetOrStoreDefault(std::string_view key, T &outValue, const T &defaultValue) const noexcept {
        return WUPSStorageAPI::GetOrStoreDefaultEx(mHandle, key.data(), outValue, defaultValue);
    }

    WUPSStorageError DeleteItem(std::string_view view) noexcept;

    WUPSStorageError GetItemSize(std::string_view view, uint32_t &outSize) noexcept;

    std::optional<WUPSStorageSubItem> CreateSubItem(std::string_view key, WUPSStorageError &err) noexcept;

    std::optional<WUPSStorageSubItem> GetSubItem(std::string_view key, WUPSStorageError &err) const noexcept;

    std::optional<WUPSStorageSubItem> GetOrCreateSubItem(std::string_view key, WUPSStorageError &err) noexcept;

    [[nodiscard]] WUPSStorageSubItem CreateSubItem(std::string_view key);

    [[nodiscard]] WUPSStorageSubItem GetSubItem(std::string_view key) const;

    [[nodiscard]] WUPSStorageSubItem GetOrCreateSubItem(std::string_view key);

private:
    wups_storage_item mHandle = {};
};

#endif
