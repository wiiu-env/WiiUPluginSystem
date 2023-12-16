#include <wups/config/WUPSConfigItem.h>
#include <wups/config_api.h>

WUPSConfigItem::~WUPSConfigItem() {
    if (mHandle.handle != nullptr) {
        WUPSConfigAPI_Item_Destroy(mHandle);
    }
}