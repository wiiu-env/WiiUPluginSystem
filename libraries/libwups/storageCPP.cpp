#include <optional>
#include <stdexcept>
#include <string>
#include <wups/storage.h>

WUPSStorageError WUPSStorageAPI::DeleteItem(std::string_view key) noexcept {
    WUPSStorageSubItem item(nullptr);
    return item.DeleteItem(key);
}

WUPSStorageError WUPSStorageAPI::SaveStorage(bool forceSave) {
    return WUPSStorageAPI_SaveStorage(forceSave);
}

WUPSStorageError WUPSStorageAPI::ForceReloadStorage() {
    return WUPSStorageAPI_ForceReloadStorage();
}

WUPSStorageError WUPSStorageAPI::WipeStorage() {
    return WUPSStorageAPI_WipeStorage();
}

std::optional<WUPSStorageSubItem> WUPSStorageAPI::CreateSubItem(std::string_view key, WUPSStorageError &err) noexcept {
    WUPSStorageSubItem item(nullptr);
    return item.CreateSubItem(key, err);
}

std::optional<WUPSStorageSubItem> WUPSStorageAPI::GetSubItem(std::string_view key, WUPSStorageError &err) noexcept {
    WUPSStorageSubItem item(nullptr);
    return item.GetSubItem(key, err);
}

std::optional<WUPSStorageSubItem> WUPSStorageAPI::GetOrCreateSubItem(std::string_view key, WUPSStorageError &err) noexcept {
    WUPSStorageSubItem item(nullptr);
    return item.GetOrCreateSubItem(key, err);
}

WUPSStorageSubItem WUPSStorageAPI::CreateSubItem(std::string_view key) {
    WUPSStorageSubItem item(nullptr);
    return item.CreateSubItem(key);
}

WUPSStorageSubItem WUPSStorageAPI::GetSubItem(std::string_view key) {
    WUPSStorageSubItem item(nullptr);
    return item.GetSubItem(key);
}

WUPSStorageSubItem WUPSStorageAPI::GetOrCreateSubItem(std::string_view key) {
    WUPSStorageSubItem item(nullptr);
    return item.GetOrCreateSubItem(key);
}

std::string_view WUPSStorageAPI::GetStatusStr(const WUPSStorageError &err) noexcept {
    return WUPSStorageAPI_GetStatusStr(err);
}

WUPSStorageSubItem WUPSStorageAPI::GetRootItem() noexcept {
    return WUPSStorageSubItem(nullptr);
}

WUPSStorageError WUPSStorageAPI::GetItemSize(std::string_view key, uint32_t &outSize) noexcept {
    WUPSStorageSubItem item(nullptr);
    return item.GetItemSize(key, outSize);
}


WUPSStorageError WUPSStorageSubItem::DeleteItem(std::string_view key) noexcept {
    return WUPSStorageAPI_DeleteItem(mHandle, key.data());
}

WUPSStorageError WUPSStorageSubItem::GetItemSize(std::string_view key, uint32_t &outSize) noexcept {
    return WUPSStorageAPI_GetItemSize(mHandle, key.data(), &outSize);
}

std::optional<WUPSStorageSubItem> WUPSStorageSubItem::CreateSubItem(std::string_view key, WUPSStorageError &err) noexcept {
    wups_storage_item outItem = {};
    err                       = WUPSStorageAPI_CreateSubItem(mHandle, key.data(), &outItem);
    if (err != WUPS_STORAGE_ERROR_SUCCESS) {
        return std::nullopt;
    }
    return WUPSStorageSubItem(outItem);
}

std::optional<WUPSStorageSubItem> WUPSStorageSubItem::GetSubItem(std::string_view key, WUPSStorageError &err) const noexcept {
    wups_storage_item outItem = {};
    err                       = WUPSStorageAPI_GetSubItem(mHandle, key.data(), &outItem);
    if (err != WUPS_STORAGE_ERROR_SUCCESS) {
        return std::nullopt;
    }
    return WUPSStorageSubItem(outItem);
}


std::optional<WUPSStorageSubItem> WUPSStorageSubItem::GetOrCreateSubItem(std::string_view key, WUPSStorageError &err) noexcept {
    wups_storage_item outItem = {};
    err                       = WUPSStorageAPI_GetSubItem(mHandle, key.data(), &outItem);
    if (err == WUPS_STORAGE_ERROR_NOT_FOUND) {
        return CreateSubItem(key);
    }
    return WUPSStorageSubItem(outItem);
}

WUPSStorageSubItem WUPSStorageSubItem::CreateSubItem(std::string_view key) {
    WUPSStorageError err;
    auto res = CreateSubItem(key, err);
    if (!res) {
        throw std::runtime_error(std::string("WUPSStorageSubItem::CreateSubItem(\"").append(key).append("\")").append(WUPSStorageAPI_GetStatusStr(err)));
    }
    return *res;
}

WUPSStorageSubItem WUPSStorageSubItem::GetSubItem(std::string_view key) const {
    WUPSStorageError err;
    auto res = GetSubItem(key, err);
    if (!res) {
        throw std::runtime_error(std::string("WUPSStorageSubItem::GetSubItem(\"").append(key).append("\")").append(WUPSStorageAPI_GetStatusStr(err)));
    }
    return *res;
}

WUPSStorageSubItem WUPSStorageSubItem::GetOrCreateSubItem(std::string_view key) {
    WUPSStorageError err;
    auto res = GetOrCreateSubItem(key, err);
    if (!res) {
        throw std::runtime_error(std::string("WUPSStorageSubItem::GetOrCreateSubItem(\"").append(key).append("\")").append(WUPSStorageAPI_GetStatusStr(err)));
    }
    return *res;
}

bool WUPSStorageSubItem::operator==(const WUPSStorageSubItem &rhs) const {
    return mHandle == rhs.mHandle;
}

bool WUPSStorageSubItem::operator!=(const WUPSStorageSubItem &rhs) const {
    return !(rhs == *this);
}
