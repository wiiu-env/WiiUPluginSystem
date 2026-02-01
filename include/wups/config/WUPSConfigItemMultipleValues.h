#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <wups/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemMultipleValuesPair {
    uint32_t value;
    const char *valueName;
} ConfigItemMultipleValuesPair;

typedef struct ConfigItemMultipleValues {
    WUPSConfigItemHandle handle;
    const char *identifier;
    int32_t defaultValueIndex;
    int32_t valueIndex;
    int32_t valueIndexAtCreation;
    ConfigItemMultipleValuesPair *values;
    int valueCount;
    void *valueChangedCallback;
} ConfigItemMultipleValues;

typedef void (*MultipleValuesChangedCallback)(ConfigItemMultipleValues *, uint32_t);


WUPSConfigAPIStatus
WUPSConfigItemMultipleValues_Create(const char *identifier, const char *displayName,
                                    int defaultValueIndex, int currentValueIndex,
                                    ConfigItemMultipleValuesPair *possibleValues, int pairCount,
                                    MultipleValuesChangedCallback callback,
                                    WUPSConfigItemHandle *outHandle);

/**
 * @brief Add a multiple values configuration item to a category.
 *
 * This function adds a multiple values configuration item to a specified category.
 * The item will be displayed in the configuration menu with the provided display name.
 *
 * @param cat The handle of the category where the item should be added.
 * @param identifier The identifier of the item. It is used to uniquely identify the item.
 * @param displayName The display name of the item. It will be shown in the configuration menu.
 * @param defaultValueIndex The index of the default value in the array of possible values.
 * @param currentValueIndex The index of the current value in the array of possible values.
 * @param possibleValues An array of possible values for the item.
 * @param pairCount The number of pairs (value and value name) in the possibleValues array.
 * @param callback A callback function that will be called when the config menu closes and the value of the item has been changed.
 *
 * @return true if the item was successfully added to the category, false otherwise.
 *
 * @note The defaultValueIndex and currentValueIndex must be valid for the given pairCount.
 */
WUPSConfigAPIStatus
WUPSConfigItemMultipleValues_AddToCategory(WUPSConfigCategoryHandle cat, const char *identifier, const char *displayName,
                                           int defaultValueIndex, int currentValueIndex,
                                           ConfigItemMultipleValuesPair *possibleValues, int pairCount,
                                           MultipleValuesChangedCallback callback);

#ifdef __cplusplus
}
#endif


#if defined(__cplusplus) && __cplusplus >= 201703L

#include "WUPSConfigItem.h"
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <wups/config_api.h>

class WUPSConfigItemMultipleValues : public WUPSConfigItem {

public:
    struct ValuePair {
        uint32_t value;
        std::string_view name;
    };

    static std::optional<WUPSConfigItemMultipleValues> CreateFromIndex(std::optional<const std::string> identifier,
                                                                       std::string_view displayName,
                                                                       int defaultValueIndex, int currentValueIndex,
                                                                       const std::span<const ValuePair> &possibleValues,
                                                                       MultipleValuesChangedCallback valuesChangedCallback,
                                                                       WUPSConfigAPIStatus &err) noexcept;

    static WUPSConfigItemMultipleValues CreateFromIndex(std::optional<const std::string> identifier,
                                                        std::string_view displayName,
                                                        int defaultValueIndex, int currentValueIndex,
                                                        const std::span<const ValuePair> &possibleValues,
                                                        MultipleValuesChangedCallback valuesChangedCallback);

    static std::optional<WUPSConfigItemMultipleValues> CreateFromValue(
            std::optional<const std::string> identifier,
            std::string_view displayName,
            uint32_t defaultValue, uint32_t currentValue,
            const std::span<const ValuePair> &possibleValues,
            MultipleValuesChangedCallback valuesChangedCallback,
            WUPSConfigAPIStatus &err) noexcept;

    static WUPSConfigItemMultipleValues CreateFromValue(
            std::optional<const std::string> identifier,
            std::string_view displayName,
            int32_t defaultValue, int32_t currentValue,
            const std::span<const ValuePair> &possibleValues,
            MultipleValuesChangedCallback valuesChangedCallback);

private:
    explicit WUPSConfigItemMultipleValues(WUPSConfigItemHandle itemHandle) : WUPSConfigItem(itemHandle) {
    }
};
#endif