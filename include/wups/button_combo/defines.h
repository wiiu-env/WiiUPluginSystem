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
    //! The 1 button
    WUPS_BUTTON_COMBO_BUTTON_1 = 0x0001,
    //! The 2 button
    WUPS_BUTTON_COMBO_BUTTON_2 = 0x0002,
    //! The C button
    WUPS_BUTTON_COMBO_BUTTON_C = 0x100000,
    //! The Z button
    WUPS_BUTTON_COMBO_BUTTON_Z = 0x200000,
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

typedef void (*WUPSButtonCombo_ComboCallback)(WUPSButtonCombo_ControllerTypes triggeredBy, WUPSButtonCombo_ComboHandle handle, void *context);

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
    uint32_t optionalHoldForXMs;
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

#ifdef __cplusplus
}
#endif
