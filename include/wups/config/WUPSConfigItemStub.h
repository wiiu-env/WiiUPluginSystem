#pragma once

#include <wups/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemStub {
    WUPSConfigItemHandle handle;
} ConfigItemStub;

WUPSConfigAPIStatus WUPSConfigItemStub_Create(const char *displayName,
                                              WUPSConfigItemHandle *outHandle);

/**
 * @brief Adds a stub item to a category to display information
 *
 * @param cat The handle of the category to which the item should be added.
 * @param displayName The display name of the item.
 * @return true if the item was added successfully, false otherwise.
 */
WUPSConfigAPIStatus
WUPSConfigItemStub_AddToCategory(WUPSConfigCategoryHandle cat, const char *displayName);

#ifdef __cplusplus
}
#endif

#if defined(__cplusplus) && __cplusplus >= 201703L

#include "WUPSConfigItem.h"
#include <optional>
#include <stdexcept>
#include <string>
#include <wups/config_api.h>

class WUPSConfigItemStub : public WUPSConfigItem {
public:
    static std::optional<WUPSConfigItemStub> Create(std::string_view displayName,
                                                    WUPSConfigAPIStatus &err) noexcept;

    static WUPSConfigItemStub Create(std::string_view displayName);

private:
    explicit WUPSConfigItemStub(WUPSConfigItemHandle itemHandle) : WUPSConfigItem(itemHandle) {
    }
};
#endif
