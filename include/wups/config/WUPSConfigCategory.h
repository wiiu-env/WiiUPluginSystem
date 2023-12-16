#pragma once

#ifdef __cplusplus

#include "WUPSConfigItem.h"
#include <optional>
#include <stdexcept>
#include <string>

class WUPSConfigCategory {
public:
    explicit WUPSConfigCategory(WUPSConfigCategoryHandle handle) noexcept;
    virtual ~WUPSConfigCategory();

    WUPSConfigCategory(const WUPSConfigCategory &) = delete;

    WUPSConfigCategory(WUPSConfigCategory &&src) noexcept : mHandle(src.mHandle) {
        src.mHandle = {};
    }

    WUPSConfigCategory &operator=(WUPSConfigCategory &&src) noexcept {
        if (this != &src) {
            src.mHandle = {};
        }
        return *this;
    }

    static std::optional<WUPSConfigCategory> Create(std::string_view name, WUPSConfigAPIStatus &error) noexcept;

    static WUPSConfigCategory Create(std::string_view name);

    bool add(WUPSConfigCategory &&cat, WUPSConfigAPIStatus &error) noexcept;

    void add(WUPSConfigCategory &&cat);

    bool add(WUPSConfigItem &&item, WUPSConfigAPIStatus &error) noexcept;

    void add(WUPSConfigItem &&item);

    [[nodiscard]] const WUPSConfigCategoryHandle &getHandle() const {
        return mHandle;
    }

    void release() {
        mHandle = {};
    }

private:
    WUPSConfigCategoryHandle mHandle = {};
};
#endif