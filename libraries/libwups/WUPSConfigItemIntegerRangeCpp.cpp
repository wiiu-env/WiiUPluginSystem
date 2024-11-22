#include "wups/config/WUPSConfigItemIntegerRange.h"

std::optional<WUPSConfigItemIntegerRange> WUPSConfigItemIntegerRange::Create(
        std::optional<std::string_view> identifier,
        std::string_view displayName,
        int32_t defaultValue, int32_t currentValue,
        int32_t minValue, int32_t maxValue,
        IntegerRangeValueChangedCallback valuesChangedCallback,
        WUPSConfigAPIStatus &err) noexcept {
    WUPSConfigItemHandle itemHandle;
    if ((err = WUPSConfigItemIntegerRange_Create(identifier ? identifier->data() : nullptr,
                                                 displayName.data(),
                                                 defaultValue, currentValue,
                                                 minValue, maxValue,
                                                 valuesChangedCallback,
                                                 &itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return std::nullopt;
    }
    return WUPSConfigItemIntegerRange(itemHandle);
}

WUPSConfigItemIntegerRange WUPSConfigItemIntegerRange::Create(
        std::optional<std::string_view> identifier,
        std::string_view displayName,
        int32_t defaultValue, int32_t currentValue,
        int32_t minValue, int32_t maxValue,
        IntegerRangeValueChangedCallback valuesChangedCallback) {
    WUPSConfigAPIStatus err = WUPSCONFIG_API_RESULT_UNKNOWN_ERROR;
    auto result             = Create(identifier, displayName, defaultValue, currentValue, minValue, maxValue, valuesChangedCallback, err);
    if (!result) {
        throw std::runtime_error(std::string("Failed to create WUPSConfigItemIntegerRange: ").append(WUPSConfigAPI_GetStatusStr(err)));
    }
    return std::move(*result);
}