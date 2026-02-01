#pragma once

#include "defines.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  * This function returns a string representation of the provided WUPSButtonCombo_Error.
  *
  * @param status The status to get the string representation for.
  * @return A pointer to a string describing the provided status.
 **/
const char *WUPSButtonComboAPI_GetStatusStr(WUPSButtonCombo_Error status);

/**
  * This function returns a string representation of the provided WUPSButtonCombo_ControllerTypes.
  *
  * @param controller The controller to get the string representation for.
  * @return A pointer to a string describing the provided controller.
 **/
const char *WUPSButtonComboAPI_GetControllerTypeStr(WUPSButtonCombo_ControllerTypes controller);

/**
  * This function returns a string representation of the provided WUPSButtonCombo_ComboStatus.
  *
  * @param status The combo status to get the string representation for.
  * @return A pointer to a string describing the provided combo status.
 **/
const char *WUPSButtonComboAPI_GetComboStatusStr(WUPSButtonCombo_ComboStatus status);


/**
 * Creates a button combo which triggers a callback if this combo is detected.
 *
 * This function takes a generic `WUPSButtonCombo_ComboOptions` which defines how the combo should be checked.
 *
 * Depending on the given type, the combo will either check for holding (for X ms) or pressing a button on for a given
 * controller mask. The type also defines if it's an "observer" or not. Observers won't check for combo conflicts.
 *
 * If the given type is not an `observer` and any other (valid) button combination overlaps with new combo, then the
 * `outStatus` will be set to `WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT` and the combo will be inactive. The original
 * ButtonCombo which had the combo first won't be affected at all.
 * In conflict state the callback will not be triggered even if the combination is pressed. If `observer` is set to
 * "true", the combo won't check for conflicts.
 *
 * To resolve a WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT combo state you **always** have to update combo information
 * via @WUPSButtonComboAPI_UpdateControllerMask or @WUPSButtonComboAPI_UpdateButtonCombo. The state won't update itself,
 * even it the combo has no conflicts in a later point in time (e.g. due to other conflicting combos being removed in the meantime)
 *
 * Conflict example (only relevant if combo type is not an observer):
 * It's not possible to add any new valid button combo containing "L+R" (e.g. "X+L+R"), if there already is a button
 * combination "L+R". Furthermore, it's also not possible to add a "L" or "R" combo if there already is a button
 * combination "L+R".
 *
 * See @WUPSButtonComboAPI_RemoveButtonCombo to remove an added button combo.
 *
 * Make sure to remove all button combos in DEINITIALIZE_PLUGIN!
 *
 * @param options options of this button combo
 * @param outHandle The handle of the button combo will be stored here on success.  Must not be nullptr.
 * @param outStatus (optional) The status of the combo will be stored here. Only if the status is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID the combo is valid. Can be NULL.
 * @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success. Please check the outStatus as well.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonCombo(const WUPSButtonCombo_ComboOptions *options,
                                                        WUPSButtonCombo_ComboHandle *outHandle,
                                                        WUPSButtonCombo_ComboStatus *outStatus);

/**
 * Creates a "PressDown" button combo which triggers a callback when given combo for a given controller has been pressed
 *
 * See @WUPSButtonComboAPI_AddButtonCombo for detailed information about button combos.
 * See @WUPSButtonComboAPI_RemoveButtonCombo to remove the added button combo.
 *
 * @param label label of the button combo
 * @param controllerMask Mask of controllers which should be checked. Must not be empty.
 * @param combo Combo which should be checked. Must not be empty
 * @param callback Callback that will be called if a button combo is detected. Must not be nullptr.
 * @param context Context for the callback. Can be nullptr.
 * @param observer Defines if this combo should check for conflicts. Set it to "true" to be an observer and ignore conflicts.
 * @param outHandle The handle of the button combo will be stored here on success.  Must not be nullptr.
 * @param outStatus (optional) The status of the combo will be stored here. Only if the status is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID the combo is valid. Can be NULL.
 * @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success. Please check the outStatus as well.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboPressDownEx(const char *label,
                                                                   WUPSButtonCombo_ControllerTypes controllerMask,
                                                                   WUPSButtonCombo_Buttons combo,
                                                                   WUPSButtonCombo_ComboCallback callback,
                                                                   void *context,
                                                                   bool observer,
                                                                   WUPSButtonCombo_ComboHandle *outHandle,
                                                                   WUPSButtonCombo_ComboStatus *outStatus);

/**
 * Wrapper for `WUPSButtonComboAPI_AddButtonComboPressDownEx` with
 * - `observer` set to "true"
 * - `controllerMask` set to "WUPS_BUTTON_COMBO_CONTROLLER_ALL"
 *
 * See: @WUPSButtonComboAPI_AddButtonComboPressDownEx for more information.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboPressDown(const char *label,
                                                                 WUPSButtonCombo_Buttons combo,
                                                                 WUPSButtonCombo_ComboCallback callback,
                                                                 void *context,
                                                                 WUPSButtonCombo_ComboHandle *outHandle,
                                                                 WUPSButtonCombo_ComboStatus *outStatus);

/**
 * Wrapper for `WUPSButtonComboAPI_AddButtonComboPressDownEx` with
 * - `observer` set to "true"
 * - `controllerMask` set to "WUPS_BUTTON_COMBO_CONTROLLER_ALL"
 *
 * See: @WUPSButtonComboAPI_AddButtonComboPressDownEx for more information.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboPressDownObserver(const char *label,
                                                                         WUPSButtonCombo_Buttons combo,
                                                                         WUPSButtonCombo_ComboCallback callback,
                                                                         void *context,
                                                                         WUPSButtonCombo_ComboHandle *outHandle,
                                                                         WUPSButtonCombo_ComboStatus *outStatus);

/**
  * Creates a "Hold" button combo which triggers a callback when given combo for a given controller has been hold for X ms
  *
  * See @WUPSButtonComboAPI_AddButtonCombo for detailed information about button combos.
  * See @WUPSButtonComboAPI_RemoveButtonCombo to remove the added button combo.
  *
  * @param label label of the button combo
  * @param controllerMask Mask of controllers which should be checked. Must not be empty.
  * @param combo Combo which should be checked. Must not be empty
  * @param holdDurationInMs Defines how long the button combination need to be hold down. Must not be 0.
  * @param callback Callback that will be called if a button combo is detected. Must not be nullptr.
  * @param context Context for the callback. Can be nullptr.
  * @param observer Defines if this combo should check for conflicts. Set it to "true" to be an observer and ignore conflicts.
  * @param outHandle The handle of the button combo will be stored here on success.  Must not be nullptr.
  * @param outStatus (optional) The status of the combo will be stored here. Only if the status is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID the combo is valid. Can be NULL.
  * @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success. Please check the outStatus as well.
 */
WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboHoldEx(const char *label,
                                                              WUPSButtonCombo_ControllerTypes controllerMask,
                                                              WUPSButtonCombo_Buttons combo,
                                                              uint32_t holdDurationInMs,
                                                              WUPSButtonCombo_ComboCallback callback,
                                                              void *context,
                                                              bool observer,
                                                              WUPSButtonCombo_ComboHandle *outHandle,
                                                              WUPSButtonCombo_ComboStatus *outStatus);

/**
 * Wrapper for `WUPSButtonComboAPI_AddButtonComboHoldEx` with
 * - `observer` set to "false"
 * - `controllerMask` set to "WUPS_BUTTON_COMBO_CONTROLLER_ALL"
 *
 * See: @WUPSButtonComboAPI_AddButtonComboHoldEx for more information.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboHold(const char *label,
                                                            WUPSButtonCombo_Buttons combo,
                                                            uint32_t holdDurationInMs,
                                                            WUPSButtonCombo_ComboCallback callback,
                                                            void *context,
                                                            WUPSButtonCombo_ComboHandle *outHandle,
                                                            WUPSButtonCombo_ComboStatus *outStatus);

/**
 * Wrapper for `WUPSButtonComboAPI_AddButtonComboHoldEx` with
 * - `observer` set to "true"
 * - `controllerMask` set to "WUPS_BUTTON_COMBO_CONTROLLER_ALL"
 *
 * See: @WUPSButtonComboAPI_AddButtonComboHoldEx for more information.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboHoldObserver(const char *label,
                                                                    WUPSButtonCombo_Buttons combo,
                                                                    uint32_t holdDurationInMs,
                                                                    WUPSButtonCombo_ComboCallback callback,
                                                                    void *context,
                                                                    WUPSButtonCombo_ComboHandle *outHandle,
                                                                    WUPSButtonCombo_ComboStatus *outStatus);

/**
* Removes a button combo for the given handle.
*
* @param handle handle of the button combo that should be removed.
* @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_RemoveButtonCombo(WUPSButtonCombo_ComboHandle handle);


/**
 * Gets a button combo status for the given handle.
 *
 * @param handle Handle of the button combo
 * @param outStatus The status of the combo will be stored here.
 * @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success.
 */
WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboStatus(WUPSButtonCombo_ComboHandle handle,
                                                              WUPSButtonCombo_ComboStatus *outStatus);
/**
* Updates the meta options for the given handle
*
* @param handle Handle of the button
* @param metaOptions new meta options
* @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateButtonComboMeta(WUPSButtonCombo_ComboHandle handle,
                                                               const WUPSButtonCombo_MetaOptions *metaOptions);

/**
* Updates the callback and context for the given handle
*
* @param handle Handle of the button combo
* @param callbackOptions new callback options
* @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateButtonComboCallback(WUPSButtonCombo_ComboHandle handle,
                                                                   const WUPSButtonCombo_CallbackOptions *callbackOptions);

/**
* Updates the controller mask for the given handle
*
* This will also re-check for conflicts and updates the combo status
*
* @param handle Handle of the button combo
* @param controllerMask new controller mask. must not be empty
* @param outStatus the new combo status after setting the mask will be written here.
* @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateControllerMask(WUPSButtonCombo_ComboHandle handle,
                                                              WUPSButtonCombo_ControllerTypes controllerMask,
                                                              WUPSButtonCombo_ComboStatus *outStatus);

/**
* Updates the combo for the given handle
*
* This will also re-check for conflicts and updates the combo status
*
* @param handle Handle of the button combo
* @param combo new combo. must not be empty.
* @param outStatus the new combo status after setting the mask will be written here.
* @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateButtonCombo(WUPSButtonCombo_ComboHandle handle,
                                                           WUPSButtonCombo_Buttons combo,
                                                           WUPSButtonCombo_ComboStatus *outStatus);

/**
* Updates hold duration for a given handle
*
* This only possible to "Hold"-button combos.
*
* @param handle Handle of the button combo
* @param holdDurationInMs the new hold duration in milliseconds
* @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateHoldDuration(WUPSButtonCombo_ComboHandle handle,
                                                            uint32_t holdDurationInMs);

/**
* Returns the current metadata for the given handle
*
* @param handle Handle of the button combo
* @param outOptions struct where the result will be written to. Must not be nullptr.
* @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboMeta(WUPSButtonCombo_ComboHandle handle,
                                                            WUPSButtonCombo_MetaOptionsOut *outOptions);

/**
* Returns the current callback and context for the given handle
*
* @param handle Handle of the button combo
* @param outOptions struct where the result will be written to. Must not be nullptr.
* @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboCallback(WUPSButtonCombo_ComboHandle handle,
                                                                WUPSButtonCombo_CallbackOptions *outOptions);

/**
* Returns the information about the controller mask and combo for the given handle
*
* @param handle Handle of the button combo
* @param outOptions struct where the result will be written to. Must not be nullptr.
* @return Returns WUPS_BUTTON_COMBO_ERROR_SUCCESS on success.
*/
WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboInfoEx(WUPSButtonCombo_ComboHandle handle,
                                                              WUPSButtonCombo_ButtonComboInfoEx *outOptions);


/**
* Helper function to check the combo status for a given button combo option struct.
*
* This can be used to check if a certain button combination is still "free" and won't cause any conflicts.
*
* The input for this function is a "WUPSButtonCombo_ButtonComboOptions" struct ptr. Fill in the values like this:
* `controllerMask` - Mask of which controllers would be checked for the button combo.
* `combo` - The button combo that should be checked.
*
* @param options Holds information about how the button combo should be detected.
* @param outStatus On success this will store the status of provided combo options.
* @return Returns "WUPS_BUTTON_COMBO_ERROR_SUCCESS" on success
**/
WUPSButtonCombo_Error WUPSButtonComboAPI_CheckComboAvailable(const WUPSButtonCombo_ButtonComboOptions *options,
                                                             WUPSButtonCombo_ComboStatus *outStatus);

/**
* Helper function to detect a pressed button combo.
*
* This function is blocking the current thread until it return, call it in an appropriate place.
*
* The input for this function is a "WUPSButtonCombo_DetectButtonComboOptions" struct ptr. Fill in the values like this:
* `controllerMask` - Mask of which controller should be checked for a button combo. Must not be empty
* `holdComboForInMs` - Defines how many ms a combo needs to be hold to be detected as a combo
* `holdAbortForInMs` - Defines how many ms the abort combo needs to be hold so the detection will be aborted.
* `abortButtonCombo` - Defines the combo that will trigger an abort.
*
* The abort button combo is checked on all controller, if they are not part of the `controllerMask`
*
* @param options Holds information about how the button combo should be detected.
* @param outButtons The detected button combo will be stored here if the functions returns WUPS_BUTTON_COMBO_ERROR_SUCCESS.
* @return Returns "WUPS_BUTTON_COMBO_ERROR_SUCCESS" on success, and "WUPS_BUTTON_COMBO_ERROR_ABORTED" if the detection was aborted.
**/
WUPSButtonCombo_Error WUPSButtonComboAPI_DetectButtonCombo_Blocking(const WUPSButtonCombo_DetectButtonComboOptions *options,
                                                                    WUPSButtonCombo_Buttons *outButtons);

#ifdef __cplusplus
}
#endif

#if defined(__cplusplus) && __cplusplus >= 201703L

#include "WUPSButtonCombo.h"
#include <optional>
#include <string_view>

namespace WUPSButtonComboAPI {
    /**
     * Wrapper for @WUPSButtonComboAPI_GetStatusStr
    **/
    const char *GetStatusStr(WUPSButtonCombo_Error status);

    /**
    * Wrapper for @WUPSButtonComboAPI_GetControllerTypeStr
   **/
    const char *GetControllerTypeStr(WUPSButtonCombo_ControllerTypes controller);

    /**
     * Wrapper for @WUPSButtonComboAPI_GetComboStatusStr
    **/
    const char *GetComboStatusStr(WUPSButtonCombo_ComboStatus status);

    /**
     * Creates a button combo which triggers a callback if this combo is detected.
     *
     * This function takes a generic `WUPSButtonCombo_ComboOptions` which defines how the combo should be checked.
     *
     * Depending on the given type, the combo will either check for holding (for X ms) or pressing a button on for a given
     * controller mask. The type also defines if it's an "observer" or not. Observers won't check for combo conflicts.
     *
     * If the given type is not an `observer` and any other (valid) button combination overlaps with new combo, then the
     * `outStatus` will be set to `WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT` and the combo will be inactive. The original
     * ButtonCombo which had the combo first won't be affected at all.
     * In conflict state the callback will not be triggered even if the combination is pressed. If `observer` is set to
     * "true", the combo won't check for conflicts.
     *
     * To resolve a WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT combo state you **always** have to update combo information
     * via @ButtonCombo::UpdateControllerMask or @ButtonCombo::UpdateButtonCombo. The state won't update itself,
     * even it the combo has no conflicts in a later point in time (e.g. due to other conflicting combos being removed in the meantime)
     *
     * Conflict example (only relevant if combo type is not an observer):
     * It's not possible to add any new valid button combo containing "L+R" (e.g. "X+L+R"), if there already is a button
     * combination "L+R". Furthermore, it's also not possible to add a "L" or "R" combo if there already is a button
     * combination "L+R".
     *
     * @param options options of this button combo
     * @param outStatus (optional) The status of the combo will be stored here. Only if the status is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID the combo is valid.
     * @param outError The error of this operation will be stored here. Only if the error is WUPS_BUTTON_COMBO_ERROR_SUCCESS creating the combo was successful.
     * @return An optional `ButtonCombo` object if the combo registration succeeds; otherwise, an empty optional.
    */
    std::optional<ButtonCombo> CreateComboEx(const WUPSButtonCombo_ComboOptions &options,
                                             WUPSButtonCombo_ComboStatus &outStatus,
                                             WUPSButtonCombo_Error &outError) noexcept;

    /**
     * Creates a button combo which triggers a callback if this combo is detected.
     *
     * This function creates a "PressDown"-combo. This means the callback is triggered once the combo is pressed down
     * on one of the provided controllers. The provided controller mask can be a combination of any
     * `WUPSButtonCombo_ControllerTypes` values.
     *
     * The "observer" parameter defines if this button combo should check for conflicts with other button combos.
     *
     * See @CreateComboEx for more information.
     *
     * The returned objected will automatically remove the button combo in the destructor. Make sure to keep it around
     * along as the button combo should be valid! You have to use `std::move` to move it around.
     *
     * @param label Label of this button combo
     * @param controllerMask Mask of controllers which should be checked. Must not be empty.
     * @param combo Combination which should be checked. Must not be empty.
     * @param callback Callback that will be called if a button combo is detected. Must not be nullptr.
     * @param context Context for the callback. Can be nullptr.
     * @param observer Defines if this combo should check for conflicts. Set to "true" to be an observer and ignore conflicts.
     * @param outStatus The status of the combo will be stored here. Only if the status is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID the combo is valid.
     * @param outError The error of this operation will be stored here. Only if the error is WUPS_BUTTON_COMBO_ERROR_SUCCESS creating the combo was successful.
     * @return An optional `ButtonCombo` object if the combo registration succeeds; otherwise, an empty optional.
    */
    std::optional<ButtonCombo> CreateComboPressDownEx(std::string_view label,
                                                      WUPSButtonCombo_ControllerTypes controllerMask,
                                                      WUPSButtonCombo_Buttons combo,
                                                      WUPSButtonCombo_ComboCallback callback,
                                                      void *context,
                                                      bool observer,
                                                      WUPSButtonCombo_ComboStatus &outStatus,
                                                      WUPSButtonCombo_Error &outError) noexcept;

    /**
     * Wrapper for `CreateComboPressDownEx` with `observer` set to "false" and `controllerMask` set to
     * "WUPS_BUTTON_COMBO_CONTROLLER_ALL"
     *
     * See: @CreateComboPressDownEx for more information.
     *
     * The returned objected will automatically remove the button combo in the destructor. Make sure to keep it around
     * along as the button combo should be valid! You have to use `std::move` to move it around.
    */
    std::optional<ButtonCombo> CreateComboPressDown(std::string_view label,
                                                    WUPSButtonCombo_Buttons combo,
                                                    WUPSButtonCombo_ComboCallback callback,
                                                    void *context,
                                                    WUPSButtonCombo_ComboStatus &outStatus,
                                                    WUPSButtonCombo_Error &outError) noexcept;

    /**
     * Wrapper for `CreateComboPressDownEx` with `observer` set to "true" and `controllerMask` set to
     * "WUPS_BUTTON_COMBO_CONTROLLER_ALL"
     *
     * See: @CreateComboPressDownEx for more information.
     *
     * The returned objected will automatically remove the button combo in the destructor. Make sure to keep it around
     * along as the button combo should be valid! You have to use `std::move` to move it around.
    */
    std::optional<ButtonCombo> CreateComboPressDownObserver(std::string_view label,
                                                            WUPSButtonCombo_Buttons combo,
                                                            WUPSButtonCombo_ComboCallback callback,
                                                            void *context,
                                                            WUPSButtonCombo_ComboStatus &outStatus,
                                                            WUPSButtonCombo_Error &outError) noexcept;

    /**
     * Creates a button combo which triggers a callback if a given button Combination has been hold for given duration.
     *
     * This function creates a "Hold"-combo. This means the callback is triggered once the combo is hold for a given duration
     * on one of the provided controllers. The provided controller mask can be a combination of any `WUPSButtonCombo_ControllerTypes` values.
     *
     *  The "observer" parameter defines if this button combo should check for conflicts with other button combos.
     *
     *  See: @CreateComboEx for more information about the details.
     *
     * The returned objected will automatically remove the button combo in the destructor. Make sure to keep it around
     * along as the button combo should be valid! You have to use `std::move` to move it around.
     *
     * @param label Label of this button combo
     * @param controllerMask Mask of controllers which should be checked. Must not be empty.
     * @param combo Combination which should be checked. Must not be empty.
     * @param holdDurationInMs Defines how long the button combination need to be hold down. Must not be 0.
     * @param callback Callback that will be called if a button combo is detected. Must not be nullptr.
     * @param context Context for the callback
     * @param observer Defines if this combo should check for conflicts. Set to "true" to be an observer and ignore conflicts.
     * @param outStatus The status of the combo will be stored here. Only if the status is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID the combo is valid. Must not be nullptr.
     * @param outError The error of this operation will be stored here. Only if the error is WUPS_BUTTON_COMBO_ERROR_SUCCESS creating the combo was successful. Must not be nullptr.
     * @return An optional `ButtonCombo` object if the combo registration succeeds; otherwise, an empty optional.
    */
    std::optional<ButtonCombo> CreateComboHoldEx(std::string_view label,
                                                 WUPSButtonCombo_ControllerTypes controllerMask,
                                                 WUPSButtonCombo_Buttons combo,
                                                 uint32_t holdDurationInMs,
                                                 WUPSButtonCombo_ComboCallback callback,
                                                 void *context,
                                                 bool observer,
                                                 WUPSButtonCombo_ComboStatus &outStatus,
                                                 WUPSButtonCombo_Error &outError) noexcept;

    /**
     * Wrapper for `CreateComboHoldEx` with `observer` set to "false" and `controllerMask` set to
     * "WUPS_BUTTON_COMBO_CONTROLLER_ALL"
     *
     * See: @CreateComboHoldEx for more information.
     *
     * The returned objected will automatically remove the button combo in the destructor. Make sure to keep it around
     * along as the button combo should be valid! You have to use `std::move` to move it around.
    */
    std::optional<ButtonCombo> CreateComboHold(std::string_view label,
                                               WUPSButtonCombo_Buttons combo,
                                               uint32_t holdDurationInMs,
                                               WUPSButtonCombo_ComboCallback callback,
                                               void *context,
                                               WUPSButtonCombo_ComboStatus &outStatus,
                                               WUPSButtonCombo_Error &outError) noexcept;

    /**
     * Wrapper for `CreateComboHoldEx` with `observer` set to "true" and `controllerMask` set to
     * "WUPS_BUTTON_COMBO_CONTROLLER_ALL"
     *
     * See: @CreateComboHoldEx for more information.
     *
     * The returned objected will automatically remove the button combo in the destructor. Make sure to keep it around
     * along as the button combo should be valid! You have to use `std::move` to move it around.
    */
    std::optional<ButtonCombo> CreateComboHoldObserver(std::string_view label,
                                                       WUPSButtonCombo_Buttons combo,
                                                       uint32_t holdDurationInMs,
                                                       WUPSButtonCombo_ComboCallback callback,
                                                       void *context,
                                                       WUPSButtonCombo_ComboStatus &outStatus,
                                                       WUPSButtonCombo_Error &outError) noexcept;


    /**
    *  Same as @CreateComboEx, but throwing an exception on error instead of returning an optional.
    */
    ButtonCombo CreateComboEx(const WUPSButtonCombo_ComboOptions &options,
                              WUPSButtonCombo_ComboStatus &outStatus);

    /**
    *  Same as @CreateComboPressDownEx, but throwing an exception on error instead of returning an optional.
    */
    ButtonCombo CreateComboPressDownEx(std::string_view label,
                                       WUPSButtonCombo_ControllerTypes controllerMask,
                                       WUPSButtonCombo_Buttons combo,
                                       WUPSButtonCombo_ComboCallback callback,
                                       void *context,
                                       bool observer,
                                       WUPSButtonCombo_ComboStatus &outStatus);
    /**
    *  Same as @CreateComboPressDown, but throwing an exception on error instead of returning an optional.
    */
    ButtonCombo CreateComboPressDown(std::string_view label,
                                     WUPSButtonCombo_Buttons combo,
                                     WUPSButtonCombo_ComboCallback callback,
                                     void *context,
                                     WUPSButtonCombo_ComboStatus &outStatus);
    /**
    *  Same as @CreateComboPressDownObserver, but throwing an exception on error instead of returning an optional.
    */
    ButtonCombo CreateComboPressDownObserver(std::string_view label,
                                             WUPSButtonCombo_Buttons combo,
                                             WUPSButtonCombo_ComboCallback callback,
                                             void *context,
                                             WUPSButtonCombo_ComboStatus &outStatus);
    /**
    *  Same as @CreateComboHoldEx, but throwing an exception on error instead of returning an optional.
    */
    ButtonCombo CreateComboHoldEx(std::string_view label,
                                  WUPSButtonCombo_ControllerTypes controllerMask,
                                  WUPSButtonCombo_Buttons combo,
                                  uint32_t holdDurationInMs,
                                  WUPSButtonCombo_ComboCallback callback,
                                  void *context,
                                  bool observer,
                                  WUPSButtonCombo_ComboStatus &outStatus);
    /**
    *  Same as @CreateComboHold, but throwing an exception on error instead of returning an optional.
    */
    ButtonCombo CreateComboHold(std::string_view label,
                                WUPSButtonCombo_Buttons combo,
                                uint32_t holdDurationInMs,
                                WUPSButtonCombo_ComboCallback callback,
                                void *context,
                                WUPSButtonCombo_ComboStatus &outStatus);
    /**
    *  Same as @CreateComboHoldObserver, but throwing an exception on error instead of returning an optional.
    */
    ButtonCombo CreateComboHoldObserver(std::string_view label,
                                        WUPSButtonCombo_Buttons combo,
                                        uint32_t holdDurationInMs,
                                        WUPSButtonCombo_ComboCallback callback,
                                        void *context,
                                        WUPSButtonCombo_ComboStatus &outStatus);

    /**
    * See @WUPSButtonComboAPI_CheckComboAvailable
    **/
    WUPSButtonCombo_Error CheckComboAvailable(const WUPSButtonCombo_ButtonComboOptions &options,
                                              WUPSButtonCombo_ComboStatus &outStatus);


    /**
    * See @WUPSButtonComboAPI_DetectButtonCombo_Blocking
    **/
    WUPSButtonCombo_Error DetectButtonCombo_Blocking(const WUPSButtonCombo_DetectButtonComboOptions &options,
                                                     WUPSButtonCombo_Buttons &outButtons);

} // namespace WUPSButtonComboAPI
#endif