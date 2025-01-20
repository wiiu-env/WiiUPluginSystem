#pragma once

#ifdef __cplusplus

#include "WUPSConfigItem.h"
#include <optional>
#include <string>

class WUPSConfigCategory {
public:
    explicit WUPSConfigCategory(WUPSConfigCategoryHandle handle) noexcept;
    virtual ~WUPSConfigCategory();

    WUPSConfigCategory(const WUPSConfigCategory &) = delete;

    WUPSConfigCategory &operator=(const WUPSConfigCategory &) = delete;

    WUPSConfigCategory(WUPSConfigCategory &&src) noexcept;

    WUPSConfigCategory &operator=(WUPSConfigCategory &&src) noexcept;

    void releaseHandle() noexcept;

    static std::optional<WUPSConfigCategory> Create(std::string_view name, WUPSConfigAPIStatus &error) noexcept;

    static WUPSConfigCategory Create(std::string_view name);

    bool add(WUPSConfigCategory &&cat, WUPSConfigAPIStatus &error) noexcept;

    void add(WUPSConfigCategory &&cat);

    bool add(WUPSConfigItem &&item, WUPSConfigAPIStatus &error) noexcept;

    void add(WUPSConfigItem &&item);

    [[nodiscard]] const WUPSConfigCategoryHandle &getHandle() const;

    void release();

private:
    WUPSConfigCategoryHandle mHandle = {};
};
#endif