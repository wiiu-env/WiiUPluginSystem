#pragma once

#include <stdint.h>
#include <wups/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ConfigItemIPAddress_State {
    WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_NONE,
    // Make sure they are sequential
    WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD1,
    WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD2,
    WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD3,
    WUPS_CONFIG_ITEM_IP_ADDRESS_STATE_EDIT_INPUT_FIELD4
} ConfigItemIPAddress_State;

typedef struct ConfigItemIPAddress {
    char *identifier;
    WUPSConfigItemHandle itemHandle;
    ConfigItemIPAddress_State itemState;
    uint32_t defaultValue;
    uint32_t currentValue;
    uint32_t valueAtCreation;
    uint32_t valueBeforeEdit;
    uint32_t frameTimer;
    void *valueChangedCallback;
} ConfigItemIPAddress;

typedef void (*IPAddressValueChangedCallback)(ConfigItemIPAddress *item, uint32_t ipAddress);

WUPSConfigAPIStatus
WUPSConfigItemIPAddress_Create(const char *identifier,
                               const char *displayName,
                               uint32_t defaultValue,
                               uint32_t currentValue,
                               IPAddressValueChangedCallback callback,
                               WUPSConfigItemHandle *outItemHandle);


WUPSConfigAPIStatus
WUPSConfigItemIPAddress_AddToCategory(WUPSConfigCategoryHandle cat,
                                      const char *identifier, const char *displayName,
                                      uint32_t defaultValue,
                                      uint32_t currentValue,
                                      IPAddressValueChangedCallback callback);
#ifdef __cplusplus
}
#endif

#if defined(__cplusplus) && __cplusplus > 201703L

#include <optional>
#include <string>
#include <wups/config/WUPSConfigItem.h>

class WUPSConfigItemIPAddress : public WUPSConfigItem {
public:
    static std::optional<WUPSConfigItemIPAddress> Create(const std::optional<std::string> &identifier,
                                                         std::string_view displayName,
                                                         uint32_t defaultValue,
                                                         uint32_t currentValue,
                                                         IPAddressValueChangedCallback callback,
                                                         WUPSConfigAPIStatus &err) noexcept;

    static WUPSConfigItemIPAddress Create(const std::optional<std::string> &identifier,
                                          std::string_view displayName,
                                          uint32_t defaultValue,
                                          uint32_t currentValue,
                                          IPAddressValueChangedCallback callback);

private:
    explicit WUPSConfigItemIPAddress(const WUPSConfigItemHandle itemHandle) : WUPSConfigItem(itemHandle) {
    }
};
#endif
