#pragma once

#ifdef __cplusplus

#include "defines.h"

#include <coreinit/debug.h>
#include <optional>

class WUPSButtonCombo {
public:
    static std::optional<WUPSButtonCombo> Create(const WUPSButtonCombo_ComboOptions &options,
                                                 WUPSButtonCombo_ComboStatus &outStatus,
                                                 WUPSButtonCombo_Error &outError) noexcept;

    static WUPSButtonCombo Create(const WUPSButtonCombo_ComboOptions &options,
                                  WUPSButtonCombo_ComboStatus &outStatus);

    ~WUPSButtonCombo();
    WUPSButtonCombo(const WUPSButtonCombo &) = delete;

    WUPSButtonCombo(WUPSButtonCombo &&other) noexcept;

    WUPSButtonCombo &operator=(const WUPSButtonCombo &) = delete;

    WUPSButtonCombo &operator=(WUPSButtonCombo &&other) noexcept;

    [[nodiscard]] WUPSButtonCombo_ComboHandle getHandle() const;

    WUPSButtonCombo_Error GetButtonComboStatus(WUPSButtonCombo_ComboStatus &outStatus) const;

    [[nodiscard]] WUPSButtonCombo_Error UpdateButtonComboMeta(const WUPSButtonCombo_MetaOptions &metaOptions) const;

    [[nodiscard]] WUPSButtonCombo_Error UpdateButtonComboCallback(const WUPSButtonCombo_CallbackOptions &callbackOptions) const;

    [[nodiscard]] WUPSButtonCombo_Error UpdateControllerMask(WUPSButtonCombo_ControllerTypes controllerMask,
                                                             WUPSButtonCombo_ComboStatus &outStatus) const;

    [[nodiscard]] WUPSButtonCombo_Error UpdateButtonCombo(WUPSButtonCombo_Buttons combo,
                                                          WUPSButtonCombo_ComboStatus &outStatus) const;

    [[nodiscard]] WUPSButtonCombo_Error UpdateHoldDuration(uint32_t holdDurationInFrames) const;

    [[nodiscard]] WUPSButtonCombo_Error GetButtonComboMeta(WUPSButtonCombo_MetaOptionsOut &outOptions) const;

    WUPSButtonCombo_Error GetButtonComboCallback(WUPSButtonCombo_CallbackOptions &outOptions) const;

    WUPSButtonCombo_Error GetButtonComboInfoEx(WUPSButtonCombo_ButtonComboInfoEx &outOptions) const;

private:
    void ReleaseButtonComboHandle();

    explicit WUPSButtonCombo(WUPSButtonCombo_ComboHandle handle);

    WUPSButtonCombo_ComboHandle mHandle = WUPSButtonCombo_ComboHandle(nullptr);
};
#endif