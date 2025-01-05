#include <stdexcept>
#include <wups/button_combo/api.h>
#include <wups/button_combo/defines.h>

namespace WUPSButtonComboAPI {
    std::string_view GetStatusStr(const WUPSButtonCombo_Error status) {
        return WUPSButtonComboAPI_GetStatusStr(status);
    }

    std::string_view GetControllerTypeStr(const WUPSButtonCombo_ControllerTypes controller) {
        return WUPSButtonComboAPI_GetControllerTypeStr(controller);
    }

    std::optional<ButtonCombo> CreateComboPressDownEx(const std::string_view label,
                                                      const WUPSButtonCombo_ControllerTypes controllerMask,
                                                      const WUPSButtonCombo_Buttons combo,
                                                      const WUPSButtonCombo_ComboCallback callback,
                                                      void *context,
                                                      const bool observer,
                                                      WUPSButtonCombo_ComboStatus &outStatus,
                                                      WUPSButtonCombo_Error &outError) noexcept {
        WUPSButtonCombo_ComboOptions options                 = {};
        options.metaOptions.label                            = label.data();
        options.callbackOptions                              = {.callback = callback, .context = context};
        options.buttonComboOptions.type                      = observer ? WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN_OBSERVER : WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN;
        options.buttonComboOptions.basicCombo.combo          = combo;
        options.buttonComboOptions.basicCombo.controllerMask = controllerMask;

        return ButtonCombo::Create(options, outStatus, outError);
    }

    std::optional<ButtonCombo> CreateComboPressDown(const std::string_view label,
                                                    const WUPSButtonCombo_Buttons combo,
                                                    const WUPSButtonCombo_ComboCallback callback,
                                                    void *context,
                                                    WUPSButtonCombo_ComboStatus &outStatus,
                                                    WUPSButtonCombo_Error &outError) noexcept {
        return CreateComboPressDownEx(label, WUPS_BUTTON_COMBO_CONTROLLER_ALL, combo, callback, context, false, outStatus, outError);
    }

    std::optional<ButtonCombo> CreateComboPressDownObserver(const std::string_view label,
                                                            const WUPSButtonCombo_Buttons combo,
                                                            const WUPSButtonCombo_ComboCallback callback,
                                                            void *context,
                                                            WUPSButtonCombo_ComboStatus &outStatus,
                                                            WUPSButtonCombo_Error &outError) noexcept {
        return CreateComboPressDownEx(label, WUPS_BUTTON_COMBO_CONTROLLER_ALL, combo, callback, context, true, outStatus, outError);
    }


    std::optional<ButtonCombo> CreateComboHoldEx(const std::string_view label,
                                                 const WUPSButtonCombo_ControllerTypes controllerMask,
                                                 const WUPSButtonCombo_Buttons combo,
                                                 const uint32_t holdDurationInMs,
                                                 const WUPSButtonCombo_ComboCallback callback,
                                                 void *context,
                                                 const bool observer,
                                                 WUPSButtonCombo_ComboStatus &outStatus,
                                                 WUPSButtonCombo_Error &outError) noexcept {
        WUPSButtonCombo_ComboOptions options                 = {};
        options.metaOptions.label                            = label.data();
        options.callbackOptions                              = {.callback = callback, .context = context};
        options.buttonComboOptions.type                      = observer ? WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD_OBSERVER : WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD;
        options.buttonComboOptions.basicCombo.combo          = combo;
        options.buttonComboOptions.basicCombo.controllerMask = controllerMask;
        options.buttonComboOptions.optionalHoldForXMs        = holdDurationInMs;

        return ButtonCombo::Create(options, outStatus, outError);
    }

    std::optional<ButtonCombo> CreateComboHold(const std::string_view label,
                                               const WUPSButtonCombo_Buttons combo,
                                               const uint32_t holdDurationInMs,
                                               const WUPSButtonCombo_ComboCallback callback,
                                               void *context,
                                               WUPSButtonCombo_ComboStatus &outStatus,
                                               WUPSButtonCombo_Error &outError) noexcept {
        return CreateComboHoldEx(label, WUPS_BUTTON_COMBO_CONTROLLER_ALL, combo, holdDurationInMs, callback, context, false, outStatus, outError);
    }

    std::optional<ButtonCombo> CreateComboHoldObserver(const std::string_view label,
                                                       const WUPSButtonCombo_Buttons combo,
                                                       const uint32_t holdDurationInMs,
                                                       const WUPSButtonCombo_ComboCallback callback,
                                                       void *context,
                                                       WUPSButtonCombo_ComboStatus &outStatus,
                                                       WUPSButtonCombo_Error &outError) noexcept {
        return CreateComboHoldEx(label, WUPS_BUTTON_COMBO_CONTROLLER_ALL, combo, holdDurationInMs, callback, context, true, outStatus, outError);
    }

    ButtonCombo CreateComboPressDownEx(const std::string_view label,
                                       const WUPSButtonCombo_ControllerTypes controllerMask,
                                       const WUPSButtonCombo_Buttons combo,
                                       const WUPSButtonCombo_ComboCallback callback,
                                       void *context,
                                       const bool observer,
                                       WUPSButtonCombo_ComboStatus &outStatus) {
        WUPSButtonCombo_Error error;
        auto res = CreateComboPressDownEx(label, controllerMask, combo, callback, context, observer, outStatus, error);
        if (!res) {
            throw std::runtime_error{std::string("Failed to create press down ex button combo: ").append(WUPSButtonComboAPI_GetStatusStr(error))};
        }
        return std::move(*res);
    }

    ButtonCombo CreateComboPressDown(const std::string_view label,
                                     const WUPSButtonCombo_Buttons combo,
                                     const WUPSButtonCombo_ComboCallback callback,
                                     void *context,
                                     WUPSButtonCombo_ComboStatus &outStatus) {
        return CreateComboPressDownEx(label, WUPS_BUTTON_COMBO_CONTROLLER_ALL, combo, callback, context, false, outStatus);
    }

    ButtonCombo CreateComboPressDownObserver(const std::string_view label,
                                             const WUPSButtonCombo_Buttons combo,
                                             const WUPSButtonCombo_ComboCallback callback,
                                             void *context,
                                             WUPSButtonCombo_ComboStatus &outStatus) {
        return CreateComboPressDownEx(label, WUPS_BUTTON_COMBO_CONTROLLER_ALL, combo, callback, context, true, outStatus);
    }

    ButtonCombo CreateComboHoldEx(const std::string_view label,
                                  const WUPSButtonCombo_ControllerTypes controllerMask,
                                  const WUPSButtonCombo_Buttons combo,
                                  const uint32_t holdDurationInMs,
                                  const WUPSButtonCombo_ComboCallback callback,
                                  void *context,
                                  const bool observer,
                                  WUPSButtonCombo_ComboStatus &outStatus) {
        WUPSButtonCombo_Error error;
        auto res = CreateComboHoldEx(label, controllerMask, combo, holdDurationInMs, callback, context, observer, outStatus, error);
        if (!res) {
            throw std::runtime_error{std::string("Failed to create press down ex button combo: ").append(WUPSButtonComboAPI_GetStatusStr(error))};
        }
        return std::move(*res);
    }

    ButtonCombo CreateComboHold(const std::string_view label,
                                const WUPSButtonCombo_Buttons combo,
                                const uint32_t holdDurationInMs,
                                const WUPSButtonCombo_ComboCallback callback,
                                void *context,
                                WUPSButtonCombo_ComboStatus &outStatus) {
        return CreateComboHoldEx(label, WUPS_BUTTON_COMBO_CONTROLLER_ALL, combo, holdDurationInMs, callback, context, false, outStatus);
    }

    ButtonCombo CreateComboHoldObserver(const std::string_view label,
                                        const WUPSButtonCombo_Buttons combo,
                                        const uint32_t holdDurationInMs,
                                        const WUPSButtonCombo_ComboCallback callback,
                                        void *context,
                                        WUPSButtonCombo_ComboStatus &outStatus) {
        return CreateComboHoldEx(label, WUPS_BUTTON_COMBO_CONTROLLER_ALL, combo, holdDurationInMs, callback, context, true, outStatus);
    }

    WUPSButtonCombo_Error CheckComboAvailable(const WUPSButtonCombo_ButtonComboOptions &options,
                                              WUPSButtonCombo_ComboStatus &outStatus) {
        return WUPSButtonComboAPI_CheckComboAvailable(&options, &outStatus);
    }

    WUPSButtonCombo_Error DetectButtonCombo_Blocking(const WUPSButtonCombo_DetectButtonComboOptions &options,
                                                     WUPSButtonCombo_Buttons &outButtons) {
        return WUPSButtonComboAPI_DetectButtonCombo_Blocking(&options, &outButtons);
    }
} // namespace WUPSButtonComboAPI