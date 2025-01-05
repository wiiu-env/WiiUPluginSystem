#pragma once

#include <stdint.h>
#include <wups/button_combo/defines.h>
#include <wups/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ConfigItemButtonCombo_State {
    WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_NONE,
    WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_PREPARE_FOR_HOLD,
    WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_WAIT_FOR_HOLD,
    WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_INVALID_HANDLE,
    WUPS_CONFIG_ITEM_BUTTON_COMBO_STATE_CONFLICT_WAIT,
} ConfigItemButtonCombo_State;

typedef struct ConfigItemButtonCombo {
    char *identifier;
    WUPSConfigItemHandle itemHandle;
    ConfigItemButtonCombo_State itemState;
    WUPSButtonCombo_ComboHandle comboHandle;
    WUPSButtonCombo_Buttons defaultButtonCombo;
    WUPSButtonCombo_Buttons currentButtonCombo;
    WUPSButtonCombo_Buttons comboComboAtCreation;
    uint32_t detectComboHoldDurationInMs;
    WUPSButtonCombo_Buttons detectAbortButton;
    uint32_t detectAbortHoldButtonInMs;
    void *valueChangedCallback;
    struct {
        uint32_t conflictMsgShownForXFrames;
    } intern;
} ConfigItemButtonCombo;

typedef void (*ButtonComboValueChangedCallback)(ConfigItemButtonCombo *item, uint32_t buttonComboInWUPSButtonComboButtons);


WUPSConfigAPIStatus
WUPSConfigItemButtonCombo_CreateEx(const char *identifier,
                                   const char *displayName,
                                   WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, WUPSButtonCombo_ComboHandle comboHandle,
                                   uint32_t detectHoldDurationInMs, WUPSButtonCombo_Buttons detectAbortButtonCombo, uint32_t detectAbortHoldButtonInMs,
                                   ButtonComboValueChangedCallback callback,
                                   WUPSConfigItemHandle *outItemHandle);

WUPSConfigAPIStatus
WUPSConfigItemButtonCombo_AddToCategoryEx(WUPSConfigCategoryHandle cat,
                                          const char *identifier, const char *displayName,
                                          WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, WUPSButtonCombo_ComboHandle comboHandle,
                                          uint32_t detectHoldDurationInMs, WUPSButtonCombo_Buttons detectAbortButtonCombo, uint32_t detectAbortHoldButtonInMs,
                                          ButtonComboValueChangedCallback callback);

WUPSConfigAPIStatus
WUPSConfigItemButtonCombo_AddToCategory(WUPSConfigCategoryHandle cat,
                                        const char *identifier, const char *displayName,
                                        WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, WUPSButtonCombo_ComboHandle comboHandle,
                                        ButtonComboValueChangedCallback callback);
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

#include <optional>
#include <string>
#include <wups/config/WUPSConfigItem.h>

class WUPSConfigItemButtonCombo : public WUPSConfigItem {
public:
    static std::optional<WUPSConfigItemButtonCombo> CreateEx(const std::optional<std::string> &identifier,
                                                             std::string_view displayName,
                                                             WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, WUPSButtonCombo_ComboHandle comboHandle,
                                                             uint32_t detectHoldDurationInMs, WUPSButtonCombo_Buttons detectAbortButtonCombo, uint32_t detectAbortHoldButtonInMs,
                                                             ButtonComboValueChangedCallback callback,
                                                             WUPSConfigAPIStatus &err) noexcept;

    static WUPSConfigItemButtonCombo CreateEx(const std::optional<std::string> &identifier,
                                              std::string_view displayName,
                                              WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, WUPSButtonCombo_ComboHandle comboHandle,
                                              uint32_t detectHoldDurationInMs, WUPSButtonCombo_Buttons detectAbortButtonCombo, uint32_t detectAbortHoldButtonInMs,
                                              ButtonComboValueChangedCallback callback);

    static std::optional<WUPSConfigItemButtonCombo> Create(const std::optional<std::string> &identifier,
                                                           std::string_view displayName,
                                                           WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, WUPSButtonCombo_ComboHandle comboHandle,
                                                           ButtonComboValueChangedCallback callback,
                                                           WUPSConfigAPIStatus &err) noexcept;

    static WUPSConfigItemButtonCombo Create(const std::optional<std::string> &identifier,
                                            std::string_view displayName,
                                            WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, WUPSButtonCombo_ComboHandle comboHandle,
                                            ButtonComboValueChangedCallback callback);

private:
    explicit WUPSConfigItemButtonCombo(const WUPSConfigItemHandle itemHandle) : WUPSConfigItem(itemHandle) {
    }
};
#endif