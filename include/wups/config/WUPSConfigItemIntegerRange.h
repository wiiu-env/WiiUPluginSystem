#pragma once

#include <wups/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemIntegerRange {
    WUPSConfigItemHandle handle;
    const char *identifier;
    int defaultValue;
    int value;
    int valueAtCreation;
    int minValue;
    int maxValue;
    void *valueChangedCallback;
} ConfigItemIntegerRange;

typedef void (*IntegerRangeValueChangedCallback)(ConfigItemIntegerRange *, int32_t);

WUPSConfigAPIStatus
WUPSConfigItemIntegerRange_Create(const char *identifier,
                                  const char *displayName,
                                  int32_t defaultValue, int32_t currentValue,
                                  int32_t minValue, int32_t maxValue,
                                  IntegerRangeValueChangedCallback callback,
                                  WUPSConfigItemHandle *outHandle);

/**
 * \brief Adds an integer range configuration item to a category.
 *
 * This function creates a new ConfigItemIntegerRange item and adds it to the specified category.
 * The item represents an integer value within a specified range, and allows the user to modify the value.
 *
 * \param cat                    The category handle to which the item should be added.
 * \param identifier             Optional identifier for the item. Can be NULL.
 * \param displayName            The display name for the item.
 * \param defaultValue           The default value for the item.
 * \param currentValue           The current value for the item.
 * \param minValue               The minimum value allowed for the item.
 * \param maxValue               The maximum value allowed for the item.
 * \param callback               A callback function that will be called when the config menu closes and the value of the item has been changed.
 *
 * \return                       Returns true if the item was successfully added to the category, false otherwise.
 *
 * @note The defaultValue and currentValue must in the specified range.
 */
WUPSConfigAPIStatus
WUPSConfigItemIntegerRange_AddToCategory(WUPSConfigCategoryHandle cat,
                                         const char *identifier,
                                         const char *displayName,
                                         int32_t defaultValue, int32_t currentValue,
                                         int32_t minValue, int32_t maxValue,
                                         IntegerRangeValueChangedCallback callback);

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

class WUPSConfigItemIntegerRange : public WUPSConfigItem {

public:
    static std::optional<WUPSConfigItemIntegerRange> Create(
            std::optional<std::string> identifier,
            std::string_view displayName,
            int32_t defaultValue, int32_t currentValue,
            int32_t minValue, int32_t maxValue,
            IntegerRangeValueChangedCallback valuesChangedCallback,
            WUPSConfigAPIStatus &err) noexcept;

    static WUPSConfigItemIntegerRange Create(
            std::optional<std::string> identifier,
            std::string_view displayName,
            int32_t defaultValue, int32_t currentValue,
            int32_t minValue, int32_t maxValue,
            IntegerRangeValueChangedCallback valuesChangedCallback);

private:
    explicit WUPSConfigItemIntegerRange(WUPSConfigItemHandle itemHandle) : WUPSConfigItem(itemHandle) {
    }
};
#endif
