#pragma once

#ifdef __cplusplus

#include "defines.h"

#include <optional>
#include <string>

namespace WUPSButtonComboAPI {
    class ButtonCombo {
    public:
        struct MetaOptions {
            std::string label;
        };

        static std::optional<ButtonCombo> Create(const WUPSButtonCombo_ComboOptions &options,
                                                 WUPSButtonCombo_ComboStatus &outStatus,
                                                 WUPSButtonCombo_Error &outError) noexcept;

        static ButtonCombo Create(const WUPSButtonCombo_ComboOptions &options,
                                  WUPSButtonCombo_ComboStatus &outStatus);

        ~ButtonCombo();
        ButtonCombo(const ButtonCombo &) = delete;

        ButtonCombo(ButtonCombo &&other) noexcept;

        ButtonCombo &operator=(const ButtonCombo &) = delete;

        ButtonCombo &operator=(ButtonCombo &&other) noexcept;

        [[nodiscard]] WUPSButtonCombo_ComboHandle getHandle() const;

        WUPSButtonCombo_Error GetButtonComboStatus(WUPSButtonCombo_ComboStatus &outStatus) const;

        [[nodiscard]] WUPSButtonCombo_Error UpdateButtonComboMeta(const MetaOptions &metaOptions) const;

        [[nodiscard]] WUPSButtonCombo_Error UpdateButtonComboCallback(const WUPSButtonCombo_CallbackOptions &callbackOptions) const;

        [[nodiscard]] WUPSButtonCombo_Error UpdateControllerMask(WUPSButtonCombo_ControllerTypes controllerMask,
                                                                 WUPSButtonCombo_ComboStatus &outStatus) const;

        [[nodiscard]] WUPSButtonCombo_Error UpdateButtonCombo(WUPSButtonCombo_Buttons combo,
                                                              WUPSButtonCombo_ComboStatus &outStatus) const;

        [[nodiscard]] WUPSButtonCombo_Error UpdateHoldDuration(uint32_t holdDurationInFrames) const;

        [[nodiscard]] WUPSButtonCombo_Error GetButtonComboMeta(MetaOptions &outOptions) const;

        WUPSButtonCombo_Error GetButtonComboCallback(WUPSButtonCombo_CallbackOptions &outOptions) const;

        WUPSButtonCombo_Error GetButtonComboInfoEx(WUPSButtonCombo_ButtonComboInfoEx &outOptions) const;

    private:
        void ReleaseButtonComboHandle();

        explicit ButtonCombo(WUPSButtonCombo_ComboHandle handle);

        WUPSButtonCombo_ComboHandle mHandle = WUPSButtonCombo_ComboHandle(nullptr);
    };
} // namespace WUPSButtonComboAPI
#endif