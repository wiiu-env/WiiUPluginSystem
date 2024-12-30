#include <wups/config/WUPSConfigCategory.h>
#include <wups/config_api.h>

#include <stdexcept>

WUPSConfigCategory::WUPSConfigCategory(const WUPSConfigCategoryHandle handle) noexcept : mHandle(handle) {
}

WUPSConfigCategory::~WUPSConfigCategory() {
    releaseHandle();
}

WUPSConfigCategory::WUPSConfigCategory(WUPSConfigCategory &&src) noexcept {
    releaseHandle();
    mHandle     = src.mHandle;
    src.mHandle = {};
}

WUPSConfigCategory &WUPSConfigCategory::operator=(WUPSConfigCategory &&src) noexcept {
    if (this != &src) {
        releaseHandle();
        this->mHandle = src.mHandle;
        src.mHandle   = {};
    }
    return *this;
}

void WUPSConfigCategory::releaseHandle() noexcept {
    if (mHandle.handle != nullptr) {
        WUPSConfigAPI_Category_Destroy(mHandle);
        mHandle.handle = nullptr;
    }
}

std::optional<WUPSConfigCategory> WUPSConfigCategory::Create(std::string_view name, WUPSConfigAPIStatus &error) noexcept {
    WUPSConfigCategoryHandle catHandle;
    if ((error = WUPSConfigAPI_Category_Create({.name = name.data()}, &catHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return std::nullopt;
    }
    return WUPSConfigCategory(catHandle);
}

WUPSConfigCategory WUPSConfigCategory::Create(const std::string_view name) {
    WUPSConfigAPIStatus error;
    auto res = Create(name, error);
    if (!res) {
        throw std::runtime_error{std::string("Failed to create category: ").append(name)};
    }
    return std::move(*res);
}

bool WUPSConfigCategory::add(WUPSConfigCategory &&cat, WUPSConfigAPIStatus &error) noexcept {
    if (mHandle.handle == nullptr || cat.getHandle().handle == nullptr) {
        return false;
    }
    if ((error = WUPSConfigAPI_Category_AddCategory(mHandle, cat.getHandle())) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return false;
    }
    cat.release();
    return true;
}

void WUPSConfigCategory::add(WUPSConfigCategory &&cat) {
    WUPSConfigAPIStatus err;
    if (!add(std::move(cat), err)) {
        throw std::runtime_error{"Failed to add category to category"};
    }
}

bool WUPSConfigCategory::add(WUPSConfigItem &&item, WUPSConfigAPIStatus &error) noexcept {
    if (mHandle.handle == nullptr || item.getHandle().handle == nullptr) {
        error = WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
        return false;
    }
    if ((error = WUPSConfigAPI_Category_AddItem(mHandle, item.getHandle())) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return false;
    }
    item.release();
    return true;
}

void WUPSConfigCategory::add(WUPSConfigItem &&item) {
    WUPSConfigAPIStatus err;
    if (!add(std::move(item), err)) {
        throw std::runtime_error{"Failed to add item to category"};
    }
}

const WUPSConfigCategoryHandle &WUPSConfigCategory::getHandle() const {
    return mHandle;
}

void WUPSConfigCategory::release() {
    mHandle = {};
}