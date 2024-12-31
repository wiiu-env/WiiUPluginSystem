#pragma once
#include "WUPSButtonCombo.h"

#include <optional>
#include <stdint.h>
#include <string_view>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get a string representation of the specified button_combo status.
 *
 * This function returns a string representation of the provided button_combo status.
 *
 * @param status The button_combo status to get the string representation for.
 * @return A pointer to a string describing the provided button_combo status.
 **/
const char *WUPSButtonComboAPI_GetStatusStr(WUPSButtonCombo_Error status);

/**
 * @brief Adds a button combo which triggers a callback if a certain button combination is pressed on any controller.
 *
 * Registers a unique button combination which will trigger a callback if the combo is pressed down **on any connected controller** and the button combo is valid
 *
 * A press down is only detecting while pressing down buttons. Releasing a button will never trigger the callback.
 * For example if the combo is "L+R+X" but you hold down "L+R+Y", press then "X" and release "Y" the combo won't be triggered.
 *
 * Conflict management:
 * The button combination is only active if this function returns WUPS_BUTTON_COMBO_ERROR_SUCCESS and outStatus is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID.
 *
 * If any other (valid) button combination overlaps with the button combo, the outStatus WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT and the callback will not be triggered
 * even if the combination is pressed.
 *
 * Even if the button combination would be unique and valid (due to the change or removal of other combos), the status won't update itself.
 * To resolve a WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT combo state you **always** have to the combo information via WUPSButtonComboAPI_UpdateButtonComboInfo or WUPSButtonComboAPI_UpdateButtonComboSimple.
 *
 * Conflict example: It's not possible to add any new valid button combo containing "L+R" (e.g. "X+L+R"), if there already is a button combination "L+R".
 * Furthermore, it's also not possible to add a "L" or "R" combo if there already is a button combination "L+R".
 *
 * @param label Label of this button combo
 * @param combo Combination which should be checked
 * @param callback
 * @param context
 * @param outHandle The handle of the combo will be stored here. Must not be nullptr.
 * @param outStatus The status of the combo will be stored here. Only if the status is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID the combo is valid. Must not be nullptr.
 * @return
 **/
WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboPressDown(const char *label,
                                                                 WUPSButtonCombo_Buttons combo,
                                                                 WUPSButtonCombo_ComboCallback callback,
                                                                 void *context,
                                                                 WUPSButtonCombo_ComboHandle *outHandle,
                                                                 WUPSButtonCombo_ComboStatus *outStatus);

WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboPressDownObserver(const char *label,
                                                                         WUPSButtonCombo_Buttons combo,
                                                                         WUPSButtonCombo_ComboCallback callback,
                                                                         void *context,
                                                                         WUPSButtonCombo_ComboHandle *outHandle,
                                                                         WUPSButtonCombo_ComboStatus *outStatus);
/**
 * @brief Adds a button combo which triggers a callback if a certain button combination was hold for X ms on any controller.
 *
 * Registers a unique button combination which will trigger a callback if the combo is held for a certain amount of ms **on any connected controller** and the button combo is valid.
 *
 * The callback is triggered if the given button combination has been held down for at least X ms
 *
 * Conflict management:
 * The button combination is only active if this function returns WUPS_BUTTON_COMBO_ERROR_SUCCESS and outStatus is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID.
 *
 * If any other (valid) button combination overlaps with the button combo, the outStatus WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT and the callback will not be triggered
 * even if the combination is pressed.
 *
 * Even if the button combination would be unique and valid (due to the change or removal of other combos), the status won't update itself.
 * To resolve a WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT combo state you **always** have to the combo information via WUPSButtonComboAPI_UpdateButtonComboInfo or WUPSButtonComboAPI_UpdateButtonComboSimple.
 *
 * Conflict example: It's not possible to add any new valid button combo containing "L+R" (e.g. "X+L+R"), if there already is a button combination "L+R".
 * Furthermore, it's also not possible to add a "L" or "R" combo if there already is a button combination "L+R".
 *
 * @param label Label of this button combo
 * @param combo Combination which should be checked
 * @param holdDurationInMs
 * @param callback
 * @param context
 * @param outHandle The handle of the combo will be stored here. Must not be nullptr.
 * @param outStatus The status of the combo will be stored here. Only if the status is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID the combo is valid. Must not be nullptr.
 * @return
 **/
WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboHold(const char *label,
                                                            WUPSButtonCombo_Buttons combo,
                                                            uint32_t holdDurationInMs,
                                                            WUPSButtonCombo_ComboCallback callback,
                                                            void *context,
                                                            WUPSButtonCombo_ComboHandle *outHandle,
                                                            WUPSButtonCombo_ComboStatus *outStatus);

WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboHoldObserver(const char *label,
                                                                    WUPSButtonCombo_Buttons combo,
                                                                    uint32_t holdDurationInMs,
                                                                    WUPSButtonCombo_ComboCallback callback,
                                                                    void *context,
                                                                    WUPSButtonCombo_ComboHandle *outHandle,
                                                                    WUPSButtonCombo_ComboStatus *outStatus);

WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonCombo(const WUPSButtonCombo_ComboOptions *options,
                                                        WUPSButtonCombo_ComboHandle *outHandle,
                                                        WUPSButtonCombo_ComboStatus *outStatus);

/**
* Removes a button combo for the given handle.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_RemoveButtonCombo(WUPSButtonCombo_ComboHandle handle);

/**
* Returns the combo status for the given handle
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboStatus(WUPSButtonCombo_ComboHandle handle,
                                                              WUPSButtonCombo_ComboStatus *outStatus);

WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateButtonComboMeta(WUPSButtonCombo_ComboHandle handle,
                                                               const WUPSButtonCombo_MetaOptions *metaOptions);

WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateButtonComboCallback(WUPSButtonCombo_ComboHandle handle,
                                                                   const WUPSButtonCombo_CallbackOptions *callbackOptions);

WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateControllerMask(WUPSButtonCombo_ComboHandle handle,
                                                              WUPSButtonCombo_ControllerTypes controllerMask,
                                                              WUPSButtonCombo_ComboStatus *outStatus);

WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateButtonCombo(WUPSButtonCombo_ComboHandle handle,
                                                           WUPSButtonCombo_Buttons combo,
                                                           WUPSButtonCombo_ComboStatus *outStatus);

WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateHoldDuration(WUPSButtonCombo_ComboHandle handle,
                                                            uint32_t holdDurationInMs);

WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboMeta(WUPSButtonCombo_ComboHandle handle,
                                                            WUPSButtonCombo_MetaOptionsOut *outOptions);

WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboCallback(WUPSButtonCombo_ComboHandle handle,
                                                                WUPSButtonCombo_CallbackOptions *outOptions);

WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboInfoEx(WUPSButtonCombo_ComboHandle handle,
                                                              WUPSButtonCombo_ButtonComboInfoEx *outOptions);

WUPSButtonCombo_Error WUPSButtonComboAPI_CheckComboAvailable(const WUPSButtonCombo_ButtonComboOptions *options,
                                                             WUPSButtonCombo_ComboStatus *outStatus);

WUPSButtonCombo_Error WUPSButtonComboAPI_DetectButtonCombo_Blocking(const WUPSButtonCombo_DetectButtonComboOptions *options,
                                                                    WUPSButtonCombo_Buttons *outButtons);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace WUPSButtonComboAPI {


    std::string_view GetStatusStr(WUPSButtonCombo_Error status);

    std::optional<WUPSButtonCombo> CreateComboPressDownEx(std::string_view label,
                                                          WUPSButtonCombo_ControllerTypes controllerMask,
                                                          WUPSButtonCombo_Buttons combo,
                                                          WUPSButtonCombo_ComboCallback callback,
                                                          void *context,
                                                          bool observer,
                                                          WUPSButtonCombo_ComboStatus &outStatus,
                                                          WUPSButtonCombo_Error &outError) noexcept;

    std::optional<WUPSButtonCombo> CreateComboPressDown(std::string_view label,
                                                        WUPSButtonCombo_Buttons combo,
                                                        WUPSButtonCombo_ComboCallback callback,
                                                        void *context,
                                                        WUPSButtonCombo_ComboStatus &outStatus,
                                                        WUPSButtonCombo_Error &outError) noexcept;

    std::optional<WUPSButtonCombo> CreateComboPressDownObserver(std::string_view label,
                                                                WUPSButtonCombo_Buttons combo,
                                                                WUPSButtonCombo_ComboCallback callback,
                                                                void *context,
                                                                WUPSButtonCombo_ComboStatus &outStatus,
                                                                WUPSButtonCombo_Error &outError) noexcept;

    std::optional<WUPSButtonCombo> CreateComboHoldEx(std::string_view label,
                                                     WUPSButtonCombo_ControllerTypes controllerMask,
                                                     WUPSButtonCombo_Buttons combo,
                                                     uint32_t holdDurationInMs,
                                                     WUPSButtonCombo_ComboCallback callback,
                                                     void *context,
                                                     bool observer,
                                                     WUPSButtonCombo_ComboStatus &outStatus,
                                                     WUPSButtonCombo_Error &outError) noexcept;

    std::optional<WUPSButtonCombo> CreateComboHold(std::string_view label,
                                                   WUPSButtonCombo_Buttons combo,
                                                   uint32_t holdDurationInMs,
                                                   WUPSButtonCombo_ComboCallback callback,
                                                   void *context,
                                                   WUPSButtonCombo_ComboStatus &outStatus,
                                                   WUPSButtonCombo_Error &outError) noexcept;

    std::optional<WUPSButtonCombo> CreateComboHoldObserver(std::string_view label,
                                                           WUPSButtonCombo_Buttons combo,
                                                           uint32_t holdDurationInMs,
                                                           WUPSButtonCombo_ComboCallback callback,
                                                           void *context,
                                                           WUPSButtonCombo_ComboStatus &outStatus,
                                                           WUPSButtonCombo_Error &outError) noexcept;


    WUPSButtonCombo CreateComboPressDownEx(std::string_view label,
                                           WUPSButtonCombo_ControllerTypes controllerMask,
                                           WUPSButtonCombo_Buttons combo,
                                           WUPSButtonCombo_ComboCallback callback,
                                           void *context,
                                           bool observer,
                                           WUPSButtonCombo_ComboStatus &outStatus);

    WUPSButtonCombo CreateComboPressDown(std::string_view label,
                                         WUPSButtonCombo_Buttons combo,
                                         WUPSButtonCombo_ComboCallback callback,
                                         void *context,
                                         WUPSButtonCombo_ComboStatus &outStatus);

    WUPSButtonCombo CreateComboPressDownObserver(std::string_view label,
                                                 WUPSButtonCombo_Buttons combo,
                                                 WUPSButtonCombo_ComboCallback callback,
                                                 void *context,
                                                 WUPSButtonCombo_ComboStatus &outStatus);

    WUPSButtonCombo CreateComboHoldEx(std::string_view label,
                                      WUPSButtonCombo_ControllerTypes controllerMask,
                                      WUPSButtonCombo_Buttons combo,
                                      uint32_t holdDurationInMs,
                                      WUPSButtonCombo_ComboCallback callback,
                                      void *context,
                                      bool observer,
                                      WUPSButtonCombo_ComboStatus &outStatus);

    WUPSButtonCombo CreateComboHold(std::string_view label,
                                    WUPSButtonCombo_Buttons combo,
                                    uint32_t holdDurationInMs,
                                    WUPSButtonCombo_ComboCallback callback,
                                    void *context,
                                    WUPSButtonCombo_ComboStatus &outStatus);

    WUPSButtonCombo CreateComboHoldObserver(std::string_view label,
                                            WUPSButtonCombo_Buttons combo,
                                            uint32_t holdDurationInMs,
                                            WUPSButtonCombo_ComboCallback callback,
                                            void *context,
                                            WUPSButtonCombo_ComboStatus &outStatus);
    WUPSButtonCombo_Error CheckComboAvailable(const WUPSButtonCombo_ButtonComboOptions &options,
                                              WUPSButtonCombo_ComboStatus &outStatus);

    WUPSButtonCombo_Error DetectButtonCombo_Blocking(const WUPSButtonCombo_DetectButtonComboOptions &options,
                                                     WUPSButtonCombo_Buttons &outButtons);

} // namespace WUPSButtonComboAPI
#endif