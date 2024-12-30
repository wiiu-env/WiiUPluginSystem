#pragma once

#ifdef __cplusplus

#include "wups/config.h"

class WUPSConfigItem {
protected:
    explicit WUPSConfigItem(const WUPSConfigItemHandle itemHandle) : mHandle(itemHandle) {
    }

public:
    virtual ~WUPSConfigItem();

    WUPSConfigItem(const WUPSConfigItem &) = delete;

    WUPSConfigItem(WUPSConfigItem &&src) noexcept;

    WUPSConfigItem &operator=(WUPSConfigItem &&src) noexcept;

    void releaseHandle() noexcept;

    [[nodiscard]] const WUPSConfigItemHandle &getHandle() const;

    void release();

private:
    WUPSConfigItemHandle mHandle = {};
};

#endif
