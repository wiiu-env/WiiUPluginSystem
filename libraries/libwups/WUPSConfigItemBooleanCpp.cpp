#include "wups/config/WUPSConfigItemBoolean.h"

std::optional<WUPSConfigItemBoolean> WUPSConfigItemBoolean::CreateEx(std::optional<std::string> identifier, std::string_view displayName, bool defaultValue, bool currentValue, BooleanValueChangedCallback callback, std::string_view trueValue, std::string_view falseValue, WUPSConfigAPIStatus &err) noexcept {
    WUPSConfigItemHandle itemHandle;
    if ((err = WUPSConfigItemBoolean_CreateEx(identifier ? identifier->data() : nullptr,
                                              displayName.data(),
                                              defaultValue, currentValue,
                                              callback,
                                              trueValue.data(),
                                              falseValue.data(),
                                              &itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return std::nullopt;
    }
    return WUPSConfigItemBoolean(itemHandle);
}
WUPSConfigItemBoolean WUPSConfigItemBoolean::CreateEx(std::optional<std::string> identifier, std::string_view displayName, bool defaultValue, bool currentValue, BooleanValueChangedCallback callback, std::string_view trueValue, std::string_view falseValue) {
    WUPSConfigAPIStatus err;
    auto result = CreateEx(std::move(identifier), displayName, defaultValue, currentValue, callback, trueValue, falseValue, err);
    if (!result) {
        throw std::runtime_error(std::string("Failed to create WUPSConfigItemBoolean: ").append(WUPSConfigAPI_GetStatusStr(err)));
    }
    return std::move(*result);
}

std::optional<WUPSConfigItemBoolean> WUPSConfigItemBoolean::Create(std::optional<std::string> identifier, std::string_view displayName, bool defaultValue, bool currentValue, BooleanValueChangedCallback callback, WUPSConfigAPIStatus &err) noexcept {
    return CreateEx(std::move(identifier), displayName, defaultValue, currentValue, callback, "true", "false", err);
}

WUPSConfigItemBoolean WUPSConfigItemBoolean::Create(std::optional<std::string> identifier, std::string_view displayName, bool defaultValue, bool currentValue, BooleanValueChangedCallback callback) {
    WUPSConfigAPIStatus err = WUPSCONFIG_API_RESULT_UNKNOWN_ERROR;
    auto res                = Create(std::move(identifier), displayName, defaultValue, currentValue, callback, err);
    if (!res) {
        throw std::runtime_error(std::string("Failed to create WUPSConfigItemBoolean: ").append(WUPSConfigAPI_GetStatusStr(err)));
    }
    return std::move(*res);
}
