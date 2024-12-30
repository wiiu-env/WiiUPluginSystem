#include <wups/config/WUPSConfigItem.h>
#include <wups/config_api.h>

WUPSConfigItem::~WUPSConfigItem() {
    releaseHandle();
}

WUPSConfigItem::WUPSConfigItem(WUPSConfigItem &&src) noexcept {
    releaseHandle();
    this->mHandle = src.mHandle;
    src.mHandle   = {};
}

WUPSConfigItem &WUPSConfigItem::operator=(WUPSConfigItem &&src) noexcept {
    if (this != &src) {
        releaseHandle();
        this->mHandle = src.mHandle;
        src.mHandle   = {};
    }
    return *this;
}

void WUPSConfigItem::releaseHandle() noexcept {
    if (mHandle.handle != nullptr) {
        WUPSConfigAPI_Item_Destroy(mHandle);
        mHandle.handle = nullptr;
    }
}

const WUPSConfigItemHandle &WUPSConfigItem::getHandle() const {
    return mHandle;
}

void WUPSConfigItem::release() {
    mHandle = {};
}