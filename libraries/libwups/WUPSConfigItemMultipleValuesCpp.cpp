#include "wups/config/WUPSConfigItemMultipleValues.h"


std::optional<WUPSConfigItemMultipleValues> WUPSConfigItemMultipleValues::CreateFromIndex(
        std::optional<const std::string> identifier,
        std::string_view displayName,
        int defaultValueIndex, int currentValueIndex,
        const std::span<const ValuePair> &possibleValues,
        MultipleValuesChangedCallback valuesChangedCallback,
        WUPSConfigAPIStatus &err) noexcept {
    auto *values = (ConfigItemMultipleValuesPair *) malloc(possibleValues.size_bytes());
    if (!values) {
        err = WUPSCONFIG_API_RESULT_OUT_OF_MEMORY;
        return std::nullopt;
    }
    int i = 0;
    for (const auto &cur : possibleValues) {
        values[i].value     = cur.value;
        values[i].valueName = cur.name.data();
        i++;
    }
    WUPSConfigItemHandle itemHandle;
    err = WUPSConfigItemMultipleValues_Create(
            identifier ? identifier->c_str() : nullptr,
            displayName.data(),
            defaultValueIndex, currentValueIndex,
            values, (int32_t) possibleValues.size(),
            valuesChangedCallback,
            &itemHandle);
    free(values);
    if (err != WUPSCONFIG_API_RESULT_SUCCESS) {
        return std::nullopt;
    }
    return WUPSConfigItemMultipleValues(itemHandle);
}

WUPSConfigItemMultipleValues WUPSConfigItemMultipleValues::CreateFromIndex(
        std::optional<const std::string> identifier,
        std::string_view displayName,
        int defaultValueIndex, int currentValueIndex,
        const std::span<const ValuePair> &possibleValues,
        MultipleValuesChangedCallback valuesChangedCallback) {
    WUPSConfigAPIStatus err = WUPSCONFIG_API_RESULT_UNKNOWN_ERROR;
    auto result             = CreateFromIndex(std::move(identifier), displayName, defaultValueIndex, currentValueIndex, possibleValues, valuesChangedCallback, err);
    if (!result) {
        throw std::runtime_error(std::string("Failed to create WUPSConfigItemMultipleValues: ").append(WUPSConfigAPI_GetStatusStr(err)));
    }
    return std::move(*result);
}

std::optional<WUPSConfigItemMultipleValues> WUPSConfigItemMultipleValues::CreateFromValue(
        std::optional<const std::string> identifier,
        std::string_view displayName,
        uint32_t defaultValue, uint32_t currentValue,
        const std::span<const ValuePair> &possibleValues,
        MultipleValuesChangedCallback valuesChangedCallback,
        WUPSConfigAPIStatus &err) noexcept {
    int defaultIndex      = -1;
    int currentValueIndex = -1;
    int i                 = 0;
    for (const auto &cur : possibleValues) {
        if (defaultIndex != -1 && currentValueIndex != -1) {
            break;
        }
        if (cur.value == currentValue) {
            currentValueIndex = i;
        }
        if (cur.value == defaultValue) {
            defaultIndex = i;
        }
        i++;
    }
    if (defaultIndex == -1 || currentValueIndex == -1) {
        err = WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
        return std::nullopt;
    }

    return WUPSConfigItemMultipleValues::CreateFromIndex(std::move(identifier),
                                                         displayName,
                                                         defaultIndex, currentValueIndex,
                                                         possibleValues,
                                                         valuesChangedCallback,
                                                         err);
}

WUPSConfigItemMultipleValues WUPSConfigItemMultipleValues::CreateFromValue(
        std::optional<const std::string> identifier, std::string_view displayName,
        int32_t defaultValue, int32_t currentValue,
        const std::span<const ValuePair> &possibleValues,
        MultipleValuesChangedCallback valuesChangedCallback) {
    WUPSConfigAPIStatus err = WUPSCONFIG_API_RESULT_UNKNOWN_ERROR;
    auto result             = CreateFromValue(std::move(identifier),
                                              displayName,
                                              defaultValue, currentValue,
                                              possibleValues,
                                              valuesChangedCallback,
                                              err);
    if (!result) {
        throw std::runtime_error(std::string("Failed to create WUPSConfigItemMultipleValues (\"").append(displayName).append("\":").append(WUPSConfigAPI_GetStatusStr(err)));
    }
    return std::move(*result);
}
