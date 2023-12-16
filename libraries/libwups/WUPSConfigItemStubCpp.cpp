#include "wups/config/WUPSConfigItemStub.h"

std::optional<WUPSConfigItemStub> WUPSConfigItemStub::Create(std::string_view displayName,
                                                             WUPSConfigAPIStatus &err) noexcept {
    WUPSConfigItemHandle itemHandle;
    if ((err = WUPSConfigItemStub_Create(displayName.data(),
                                         &itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return std::nullopt;
    }
    return WUPSConfigItemStub(itemHandle);
}

WUPSConfigItemStub WUPSConfigItemStub::Create(std::string_view displayName) {
    WUPSConfigAPIStatus err = WUPSCONFIG_API_RESULT_UNKNOWN_ERROR;
    auto result             = Create(displayName, err);
    if (!result) {
        throw std::runtime_error(std::string("Failed to create WUPSConfigItemStub: ").append(WUPSConfigAPI_GetStatusStr(err)));
    }
    return std::move(*result);
}
