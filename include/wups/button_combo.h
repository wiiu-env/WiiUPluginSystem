#pragma once
#include <stdint.h>
#include <wut_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum WUPSButtonCombo_Buttons {
    //! The A button.
    WUPS_BUTTON_COMBO_BUTTON_A = 0x8000,
    //! The B button.
    WUPS_BUTTON_COMBO_BUTTON_B = 0x4000,
    //! The X button.
    WUPS_BUTTON_COMBO_BUTTON_X = 0x2000,
    //! The Y button.
    WUPS_BUTTON_COMBO_BUTTON_Y = 0x1000,
    //! The left button of the D-pad.
    WUPS_BUTTON_COMBO_BUTTON_LEFT = 0x0800,
    //! The right button of the D-pad.
    WUPS_BUTTON_COMBO_BUTTON_RIGHT = 0x0400,
    //! The up button of the D-pad.
    WUPS_BUTTON_COMBO_BUTTON_UP = 0x0200,
    //! The down button of the D-pad.
    WUPS_BUTTON_COMBO_BUTTON_DOWN = 0x0100,
    //! The ZL button.
    WUPS_BUTTON_COMBO_BUTTON_ZL = 0x0080,
    //! The ZR button.
    WUPS_BUTTON_COMBO_BUTTON_ZR = 0x0040,
    //! The L button.
    WUPS_BUTTON_COMBO_BUTTON_L = 0x0020,
    //! The R button.
    WUPS_BUTTON_COMBO_BUTTON_R = 0x0010,
    //! The + button.
    WUPS_BUTTON_COMBO_BUTTON_PLUS = 0x0008,
    //! The - button.
    WUPS_BUTTON_COMBO_BUTTON_MINUS = 0x0004,
    //! The right stick button.
    WUPS_BUTTON_COMBO_BUTTON_STICK_R = 0x00020000,
    //! The left stick button.
    WUPS_BUTTON_COMBO_BUTTON_STICK_L = 0x00040000,
    //! The TV button.
    WUPS_BUTTON_COMBO_BUTTON_TV = 0x00010000,
    //! The reserved bit
    WUPS_BUTTON_COMBO_BUTTON_RESERVED_BIT = 0x80000,
} WUPSButtonCombo_Buttons;
WUT_ENUM_BITMASK_TYPE(WUPSButtonCombo_Buttons);

typedef enum WUPSButtonCombo_ControllerTypes {
    WUPS_BUTTON_COMBO_CONTROLLER_NONE   = 0,
    WUPS_BUTTON_COMBO_CONTROLLER_VPAD_0 = 1 << 0,
    WUPS_BUTTON_COMBO_CONTROLLER_VPAD_1 = 1 << 1,
    WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0 = 1 << 2,
    WUPS_BUTTON_COMBO_CONTROLLER_WPAD_1 = 1 << 3,
    WUPS_BUTTON_COMBO_CONTROLLER_WPAD_2 = 1 << 4,
    WUPS_BUTTON_COMBO_CONTROLLER_WPAD_3 = 1 << 5,
    WUPS_BUTTON_COMBO_CONTROLLER_WPAD_4 = 1 << 6,
    WUPS_BUTTON_COMBO_CONTROLLER_WPAD_5 = 1 << 7,
    WUPS_BUTTON_COMBO_CONTROLLER_WPAD_6 = 1 << 8,
    WUPS_BUTTON_COMBO_CONTROLLER_VPAD   = WUPS_BUTTON_COMBO_CONTROLLER_VPAD_0 | WUPS_BUTTON_COMBO_CONTROLLER_VPAD_1,
    WUPS_BUTTON_COMBO_CONTROLLER_WPAD   = (WUPS_BUTTON_COMBO_CONTROLLER_WPAD_0 |
                                         WUPS_BUTTON_COMBO_CONTROLLER_WPAD_1 |
                                         WUPS_BUTTON_COMBO_CONTROLLER_WPAD_2 |
                                         WUPS_BUTTON_COMBO_CONTROLLER_WPAD_3 |
                                         WUPS_BUTTON_COMBO_CONTROLLER_WPAD_4 |
                                         WUPS_BUTTON_COMBO_CONTROLLER_WPAD_5 |
                                         WUPS_BUTTON_COMBO_CONTROLLER_WPAD_6),
    WUPS_BUTTON_COMBO_CONTROLLER_ALL    = WUPS_BUTTON_COMBO_CONTROLLER_VPAD | WUPS_BUTTON_COMBO_CONTROLLER_WPAD,
} WUPSButtonCombo_ControllerTypes;
WUT_ENUM_BITMASK_TYPE(WUPSButtonCombo_ControllerTypes);

typedef enum WUPSButtonCombo_ComboType {
    WUPS_BUTTON_COMBO_COMBO_TYPE_INVALID             = 0,
    WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD                = 1, // Does check for conflicts
    WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN          = 2, // Does check for conflicts
    WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD_OBSERVER       = 3, // Does not check for conflicts
    WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN_OBSERVER = 4, // Does not check for conflicts
} WUPSButtonCombo_ComboType;

typedef enum WUPSButtonCombo_ComboStatus {
    WUPS_BUTTON_COMBO_COMBO_STATUS_INVALID_STATUS = 0,
    WUPS_BUTTON_COMBO_COMBO_STATUS_VALID          = 1,
    WUPS_BUTTON_COMBO_COMBO_STATUS_CONFLICT       = 2,
} WUPSButtonCombo_ComboStatus;

/**
 * @enum WUPSButtonComboError
 * @brief Represents error codes returned by storage API functions.
 */
typedef enum {
    WUPS_BUTTON_COMBO_ERROR_SUCCESS                  = 0,     /**< Success. */
    WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS             = -0x01, /**< Invalid arguments passed to the function. */
    WUPS_BUTTON_COMBO_ERROR_MALLOC_FAILED            = -0x02, /**< Memory allocation failed. */
    WUPS_BUTTON_COMBO_ERROR_NOT_FOUND                = -0x03, /**< Not found. */
    WUPS_BUTTON_COMBO_ERROR_ABORTED                  = -0x04, /**< Not found. */
    WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED = -0xF0, /**< Library not initialized properly. */
    WUPS_BUTTON_COMBO_ERROR_INTERNAL_INVALID_VERSION = -0xF1, /**< Invalid API version. */
    WUPS_BUTTON_COMBO_ERROR_UNKNOWN_ERROR            = -0x100 /**< Unknown error. */
} WUPSButtonCombo_Error;

typedef struct WUPSButtonCombo_ComboHandle {
    void *handle;
#ifdef __cplusplus
    WUPSButtonCombo_ComboHandle() {
        handle = nullptr;
    }
    explicit WUPSButtonCombo_ComboHandle(void *handle) : handle(handle) {}
    bool operator==(const WUPSButtonCombo_ComboHandle other) const {
        return handle == other.handle;
    }
    bool operator==(const void *other) const {
        return handle == other;
    }
#endif
} WUPSButtonCombo_ComboHandle;

typedef void (*WUPSButtonCombo_ComboCallback)(WUPSButtonCombo_ComboHandle handle, void *context);

typedef struct WUPSButtonCombo_MetaOptions {
    const char *label;
} WUPSButtonCombo_MetaOptions;

typedef struct WUPSButtonCombo_MetaOptionsOut {
    char *labelBuffer;
    uint32_t labelBufferLength;
} WUPSButtonCombo_MetaOptionsOut;

typedef struct WUPSButtonCombo_CallbackOptions {
    WUPSButtonCombo_ComboCallback callback;
    void *context;
} WUPSButtonCombo_CallbackOptions;

typedef struct WUPSButtonCombo_ButtonComboOptions {
    WUPSButtonCombo_ControllerTypes controllerMask;
    WUPSButtonCombo_Buttons combo;
} WUPSButtonCombo_ButtonComboOptions;

typedef struct WUPSButtonCombo_ButtonComboInfoEx {
    WUPSButtonCombo_ComboType type;
    WUPSButtonCombo_ButtonComboOptions basicCombo;
    uint32_t optionalHoldForXFrames;
} WUPSButtonCombo_ButtonComboInfoEx;

typedef struct WUPSButtonCombo_ComboOptions {
    WUPSButtonCombo_MetaOptions metaOptions;
    WUPSButtonCombo_CallbackOptions callbackOptions;
    WUPSButtonCombo_ButtonComboInfoEx buttonComboOptions;
} WUPSButtonCombo_ComboOptions;

typedef struct WUPSButtonCombo_DetectButtonComboOptions {
    WUPSButtonCombo_ControllerTypes controllerMask;
    uint32_t holdComboForInMs;
    uint32_t holdAbortForInMs;
    WUPSButtonCombo_Buttons abortButtonCombo;
} WUPSButtonCombo_DetectButtonComboOptions;

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
 * @param callbackOptions Information about the callbacks that will be called if the combo is triggered
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
 * @brief Adds a button combo which triggers a callback if a certain button combination was hold for X frames on any controller.
 *
 * Registers a unique button combination which will trigger a callback if the combo is held for a certain amount of frames **on any connected controller** and the button combo is valid.
 *
 * The callback is triggered if the given button combination has been held down for the given number of frames (frame rate might vary from game to game).
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
 * @param holdDurationInFrames
 * @param callbackOptions Information about the callbacks that will be called if the combo is triggered
 * @param outHandle The handle of the combo will be stored here. Must not be nullptr.
 * @param outStatus The status of the combo will be stored here. Only if the status is WUPS_BUTTON_COMBO_COMBO_STATUS_VALID the combo is valid. Must not be nullptr.
 * @return
 **/
WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboHold(const char *label,
                                                            WUPSButtonCombo_Buttons combo,
                                                            uint32_t holdDurationInFrames,
                                                            WUPSButtonCombo_ComboCallback callback,
                                                            void *context,
                                                            WUPSButtonCombo_ComboHandle *outHandle,
                                                            WUPSButtonCombo_ComboStatus *outStatus);

WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboHoldObserver(const char *label,
                                                                    WUPSButtonCombo_Buttons combo,
                                                                    uint32_t holdDurationInFrames,
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
                                                            uint32_t holdDurationInFrames);

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
