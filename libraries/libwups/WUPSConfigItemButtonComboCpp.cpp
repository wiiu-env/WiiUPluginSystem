#include <stdexcept>
#include <wups/config/WUPSConfigItemButtonCombo.h>
#include <wups/config_api.h>

std::optional<WUPSConfigItemButtonCombo> WUPSConfigItemButtonCombo::CreateEx(const std::optional<std::string> &identifier,
                                                                             const std::string_view displayName,
                                                                             const WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, const WUPSButtonCombo_ComboHandle comboHandle,
                                                                             const uint32_t detectHoldDurationInMs, const WUPSButtonCombo_Buttons detectAbortButtonCombo, const uint32_t detectAbortHoldButtonInMs,
                                                                             const ButtonComboValueChangedCallback callback,
                                                                             WUPSConfigAPIStatus &err) noexcept {
    WUPSConfigItemHandle itemHandle;
    if ((err = WUPSConfigItemButtonCombo_CreateEx(identifier ? identifier->data() : nullptr,
                                                  displayName.data(),
                                                  defaultComboInWUPSButtonComboButtons, comboHandle,
                                                  detectHoldDurationInMs, detectAbortButtonCombo, detectAbortHoldButtonInMs,
                                                  callback,
                                                  &itemHandle)) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return std::nullopt;
    }
    return WUPSConfigItemButtonCombo(itemHandle);
}

WUPSConfigItemButtonCombo WUPSConfigItemButtonCombo::CreateEx(const std::optional<std::string> &identifier,
                                                              const std::string_view displayName,
                                                              const WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, const WUPSButtonCombo_ComboHandle comboHandle,
                                                              const uint32_t detectHoldDurationInMs, const WUPSButtonCombo_Buttons detectAbortButtonCombo, const uint32_t detectAbortHoldButtonInMs,
                                                              const ButtonComboValueChangedCallback callback) {
    WUPSConfigAPIStatus err;
    auto result = CreateEx(identifier, displayName, defaultComboInWUPSButtonComboButtons, comboHandle, detectHoldDurationInMs, detectAbortButtonCombo, detectAbortHoldButtonInMs, callback, err);
    if (!result) {
        throw std::runtime_error(std::string("Failed to create WUPSConfigItemButtonCombo: ").append(WUPSConfigAPI_GetStatusStr(err)));
    }
    return std::move(*result);
}

std::optional<WUPSConfigItemButtonCombo> WUPSConfigItemButtonCombo::Create(const std::optional<std::string> &identifier,
                                                                           const std::string_view displayName,
                                                                           const WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, const WUPSButtonCombo_ComboHandle comboHandle,
                                                                           const ButtonComboValueChangedCallback callback,
                                                                           WUPSConfigAPIStatus &err) noexcept {
    return CreateEx(identifier, displayName, defaultComboInWUPSButtonComboButtons, comboHandle, 2000, WUPS_BUTTON_COMBO_BUTTON_B, 250, callback, err);
}

WUPSConfigItemButtonCombo WUPSConfigItemButtonCombo::Create(const std::optional<std::string> &identifier,
                                                            const std::string_view displayName,
                                                            const WUPSButtonCombo_Buttons defaultComboInWUPSButtonComboButtons, const WUPSButtonCombo_ComboHandle comboHandle,
                                                            const ButtonComboValueChangedCallback callback) {
    WUPSConfigAPIStatus err = WUPSCONFIG_API_RESULT_UNKNOWN_ERROR;
    auto res                = Create(identifier, displayName, defaultComboInWUPSButtonComboButtons, comboHandle, callback, err);
    if (!res) {
        throw std::runtime_error(std::string("Failed to create WUPSConfigItemButtonCombo: ").append(WUPSConfigAPI_GetStatusStr(err)));
    }
    return std::move(*res);
}
