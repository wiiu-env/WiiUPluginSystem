#pragma once

#ifdef __cplusplus

#include "wups/config.h"
#include <coreinit/debug.h>

class WUPSConfigItem {
protected:
    explicit WUPSConfigItem(WUPSConfigItemHandle itemHandle) : mHandle(itemHandle) {
    }

public:
    virtual ~WUPSConfigItem();

    WUPSConfigItem(const WUPSConfigItem &) = delete;

    WUPSConfigItem(WUPSConfigItem &&src) noexcept : mHandle(src.mHandle) {
        src.mHandle = {};
    }

    WUPSConfigItem &operator=(WUPSConfigItem &&src) noexcept {
        if (this != &src) {
            this->mHandle = src.mHandle;
            src.mHandle   = {};
        }
        return *this;
    }

    [[nodiscard]] const WUPSConfigItemHandle &getHandle() const {
        return mHandle;
    }

    void release() {
        mHandle = {};
    }

private:
    WUPSConfigItemHandle mHandle = {};
};

#endif
