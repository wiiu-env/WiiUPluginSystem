#include "wups/button_combo/WUPSButtonCombo.h"

#include <wups/button_combo/api.h>

#include <coreinit/debug.h>

#include <stdexcept>
#include <string>

#include <cstring>

namespace WUPSButtonComboAPI {
    std::optional<ButtonCombo> ButtonCombo::Create(const WUPSButtonCombo_ComboOptions &options,
                                                   WUPSButtonCombo_ComboStatus &outStatus,
                                                   WUPSButtonCombo_Error &outError) noexcept {
        WUPSButtonCombo_ComboHandle handle;
        if (outError = WUPSButtonComboAPI_AddButtonCombo(&options, &handle, &outStatus); outError == WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
            return ButtonCombo(handle);
        }
        return {};
    }

    ButtonCombo ButtonCombo::Create(const WUPSButtonCombo_ComboOptions &options,
                                    WUPSButtonCombo_ComboStatus &outStatus) {
        WUPSButtonCombo_Error error;
        auto res = Create(options, outStatus, error);
        if (!res) {
            throw std::runtime_error{std::string("Failed to create button combo: ").append(WUPSButtonComboAPI_GetStatusStr(error))};
        }
        return std::move(*res);
    }

    ButtonCombo::~ButtonCombo() {
        ReleaseButtonComboHandle();
    }

    void ButtonCombo::ReleaseButtonComboHandle() {
        if (mHandle != nullptr) {
            if (const auto res = WUPSButtonComboAPI_RemoveButtonCombo(mHandle); res != WUPS_BUTTON_COMBO_ERROR_SUCCESS) {
                OSReport("ButtonCombo::ReleaseButtonComboHandle(): WUPSButtonComboAPI_RemoveButtonCombo for %08X returned: %s\n", mHandle, WUPSButtonComboAPI_GetStatusStr(res));
            }
            mHandle = WUPSButtonCombo_ComboHandle(nullptr);
        }
    }

    ButtonCombo::ButtonCombo(ButtonCombo &&src) noexcept {
        ReleaseButtonComboHandle();

        mHandle = src.mHandle;

        src.mHandle = WUPSButtonCombo_ComboHandle(nullptr);
    }

    ButtonCombo &ButtonCombo::operator=(ButtonCombo &&src) noexcept {
        if (this != &src) {
            ReleaseButtonComboHandle();

            mHandle = src.mHandle;

            src.mHandle = WUPSButtonCombo_ComboHandle(nullptr);
        }
        return *this;
    }

    [[nodiscard]] WUPSButtonCombo_ComboHandle ButtonCombo::getHandle() const {
        return mHandle;
    }

    WUPSButtonCombo_Error ButtonCombo::GetButtonComboStatus(WUPSButtonCombo_ComboStatus &outStatus) const {
        return WUPSButtonComboAPI_GetButtonComboStatus(mHandle, &outStatus);
    }

    [[nodiscard]] WUPSButtonCombo_Error ButtonCombo::UpdateButtonComboMeta(const MetaOptions &metaOptions) const {
        const WUPSButtonCombo_MetaOptions options = {.label = metaOptions.label.c_str()};
        return WUPSButtonComboAPI_UpdateButtonComboMeta(mHandle, &options);
    }

    [[nodiscard]] WUPSButtonCombo_Error ButtonCombo::UpdateButtonComboCallback(const WUPSButtonCombo_CallbackOptions &callbackOptions) const {
        return WUPSButtonComboAPI_UpdateButtonComboCallback(mHandle, &callbackOptions);
    }

    [[nodiscard]] WUPSButtonCombo_Error ButtonCombo::UpdateControllerMask(const WUPSButtonCombo_ControllerTypes controllerMask,
                                                                          WUPSButtonCombo_ComboStatus &outStatus) const {
        return WUPSButtonComboAPI_UpdateControllerMask(mHandle, controllerMask, &outStatus);
    }

    [[nodiscard]] WUPSButtonCombo_Error ButtonCombo::UpdateButtonCombo(const WUPSButtonCombo_Buttons combo,
                                                                       WUPSButtonCombo_ComboStatus &outStatus) const {
        return WUPSButtonComboAPI_UpdateButtonCombo(mHandle, combo, &outStatus);
    }

    [[nodiscard]] WUPSButtonCombo_Error ButtonCombo::UpdateHoldDuration(const uint32_t holdDurationInFrames) const {
        return WUPSButtonComboAPI_UpdateHoldDuration(mHandle, holdDurationInFrames);
    }

    [[nodiscard]] WUPSButtonCombo_Error ButtonCombo::GetButtonComboMeta(MetaOptions &outOptions) const {
        outOptions.label.resize(512);
        memset(outOptions.label.data(), 0, outOptions.label.size());
        WUPSButtonCombo_MetaOptionsOut options;
        options.labelBuffer       = outOptions.label.data();
        options.labelBufferLength = outOptions.label.size();
        const auto res            = WUPSButtonComboAPI_GetButtonComboMeta(mHandle, &options);
        outOptions.label.resize(outOptions.label.find_first_of('\0'));
        return res;
    }

    WUPSButtonCombo_Error ButtonCombo::GetButtonComboCallback(WUPSButtonCombo_CallbackOptions &outOptions) const {
        return WUPSButtonComboAPI_GetButtonComboCallback(mHandle, &outOptions);
    }

    WUPSButtonCombo_Error ButtonCombo::GetButtonComboInfoEx(WUPSButtonCombo_ButtonComboInfoEx &outOptions) const {
        return WUPSButtonComboAPI_GetButtonComboInfoEx(mHandle, &outOptions);
    }

    ButtonCombo::ButtonCombo(const WUPSButtonCombo_ComboHandle handle) : mHandle(handle) {
    }
} // namespace WUPSButtonComboAPI