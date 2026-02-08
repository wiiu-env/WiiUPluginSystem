#include <stdexcept>
#include <wups/config/WUPSConfigItemIPAddress.h>
#include <wups/config_api.h>

std::optional<WUPSConfigItemIPAddress> WUPSConfigItemIPAddress::Create(const std::optional<std::string> &identifier,
                                                                       const std::string_view displayName,
                                                                       const uint32_t defaultValue,
                                                                       const uint32_t currentValue,
                                                                       const IPAddressValueChangedCallback callback,
                                                                       WUPSConfigAPIStatus &err) noexcept {
    WUPSConfigItemHandle itemHandle;
    if ((err = WUPSConfigItemIPAddress_Create(identifier ? identifier->data() : nullptr,
                                              displayName.data(),
                                              defaultValue,
                                              currentValue,
                                              callback,
                                              &itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return std::nullopt;
    }
    return WUPSConfigItemIPAddress(itemHandle);
}

WUPSConfigItemIPAddress WUPSConfigItemIPAddress::Create(const std::optional<std::string> &identifier,
                                                        const std::string_view displayName,
                                                        const uint32_t defaultValue,
                                                        const uint32_t currentValue,
                                                        const IPAddressValueChangedCallback callback) {
    WUPSConfigAPIStatus err = WUPSCONFIG_API_RESULT_UNKNOWN_ERROR;
    auto res                = Create(identifier, displayName, defaultValue, currentValue, callback, err);
    if (!res) {
        throw std::runtime_error(std::string("Failed to create WUPSConfigItemIPAddress: ").append(WUPSConfigAPI_GetStatusStr(err)));
    }
    return std::move(*res);
}
