#pragma once

#include <wups/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemBoolean {
    WUPSConfigItemHandle handle;
    const char *identifier;
    bool defaultValue;
    bool valueAtCreation;
    bool value;
    char trueValue[32];
    char falseValue[32];
    void *valueChangedCallback;
} ConfigItemBoolean;

typedef void (*BooleanValueChangedCallback)(ConfigItemBoolean *, bool);

WUPSConfigAPIStatus
WUPSConfigItemBoolean_CreateEx(const char *identifier,
                               const char *displayName,
                               bool defaultValue,
                               bool currentValue,
                               BooleanValueChangedCallback callback,
                               const char *trueValue,
                               const char *falseValue,
                               WUPSConfigItemHandle *outHandle);


/**
 * @brief Adds a boolean configuration item to the specified category.
 *
 * This function adds a boolean configuration item to the given category. The item is displayed with a specified display name.
 * The default value and current value of the item are set to the provided values. A callback function is called whenever
 * the value of the item changes.
 *
 * @param cat The handle of the category to add the item to.
 * @param identifier Optional identifier for the item. Can be NULL.
 * @param displayName The display name of the item.
 * @param defaultValue The default value of the item.
 * @param currentValue The current value of the item.
 * @param callback A callback function that will be called when the config menu closes and the value of the item has been changed.
 * @return True if the item was added successfully, false otherwise.
 */
WUPSConfigAPIStatus
WUPSConfigItemBoolean_AddToCategory(WUPSConfigCategoryHandle cat,
                                    const char *identifier,
                                    const char *displayName,
                                    bool defaultValue,
                                    bool currentValue,
                                    BooleanValueChangedCallback callback);

/**
 * @brief Adds a boolean configuration item to the specified category.
 *
 * This function adds a boolean configuration item to the given category. The item is displayed with a specified display name.
 * The default value and current value of the item are set to the provided values. A callback function is called whenever
 * the value of the item changes.
 *
 * @param cat The handle of the category to add the item to.
 * @param identifier Optional identifier for the item. Can be NULL.
 * @param displayName The display name of the item.
 * @param defaultValue The default value of the item.
 * @param currentValue The current value of the item.
 * @param callback A callback function that will be called when the config menu closes and the value of the item has been changed.
 * @param trueValue The string representation of the true value.
 * @param falseValue The string representation of the false value.
 * @return True if the item was successfully added to the category, false otherwise.
 */
WUPSConfigAPIStatus
WUPSConfigItemBoolean_AddToCategoryEx(WUPSConfigCategoryHandle cat,
                                      const char *identifier,
                                      const char *displayName,
                                      bool defaultValue,
                                      bool currentValue,
                                      BooleanValueChangedCallback callback,
                                      const char *trueValue,
                                      const char *falseValue);

#ifdef __cplusplus
}
#endif

#if defined(__cplusplus) && __cplusplus >= 201703L

#include "WUPSConfigItem.h"
#include <optional>
#include <stdexcept>
#include <string>
#include <wups/config_api.h>

class WUPSConfigItemBoolean : public WUPSConfigItem {
public:
    static std::optional<WUPSConfigItemBoolean> CreateEx(std::optional<std::string> identifier,
                                                         std::string_view displayName,
                                                         bool defaultValue,
                                                         bool currentValue,
                                                         BooleanValueChangedCallback callback,
                                                         std::string_view trueValue,
                                                         std::string_view falseValue,
                                                         WUPSConfigAPIStatus &err) noexcept;

    static WUPSConfigItemBoolean CreateEx(std::optional<std::string> identifier,
                                          std::string_view displayName,
                                          bool defaultValue,
                                          bool currentValue,
                                          BooleanValueChangedCallback callback,
                                          std::string_view trueValue,
                                          std::string_view falseValue);

    static std::optional<WUPSConfigItemBoolean> Create(std::optional<std::string> identifier,
                                                       std::string_view displayName,
                                                       bool defaultValue,
                                                       bool currentValue,
                                                       BooleanValueChangedCallback callback,
                                                       WUPSConfigAPIStatus &err) noexcept;

    static WUPSConfigItemBoolean Create(std::optional<std::string> identifier,
                                        std::string_view displayName,
                                        bool defaultValue,
                                        bool currentValue,
                                        BooleanValueChangedCallback callback);

private:
    explicit WUPSConfigItemBoolean(WUPSConfigItemHandle itemHandle) : WUPSConfigItem(itemHandle) {
    }
};
#endif