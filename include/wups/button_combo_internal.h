#pragma once
#include <wups/button_combo.h>

/**
 * @typedef WUPSButtonCombo_AddButtonComboFunction
 * @brief Type alias for the function pointer to add a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_AddButtonComboFunction)(void *identifier,
                                                                        const WUPSButtonCombo_ComboOptions *options,
                                                                        WUPSButtonCombo_ComboHandle *outHandle,
                                                                        WUPSButtonCombo_ComboStatus *outStatus);

/**
 * @typedef WUPSButtonCombo_RemoveButtonComboFunction
 * @brief Type alias for the function pointer to remove a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_RemoveButtonComboFunction)(void *identifier,
                                                                           WUPSButtonCombo_ComboHandle handle);
/**
 * @typedef WUPSButtonCombo_GetButtonComboStatusFunction
 * @brief Type alias for the function pointer to check the status of a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_GetButtonComboStatusFunction)(void *identifier,
                                                                              WUPSButtonCombo_ComboHandle handle,
                                                                              WUPSButtonCombo_ComboStatus *outStatus);


/**
 * @typedef WUPSButtonCombo_UpdateButtonComboMetaFunction
 * @brief Type alias for the function pointer to update the meta data of a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_UpdateButtonComboMetaFunction)(void *identifier,
                                                                               WUPSButtonCombo_ComboHandle handle,
                                                                               const WUPSButtonCombo_MetaOptions *metaOptions);

/**
 * @typedef WUPSButtonCombo_UpdateButtonComboMetaFunction
 * @brief Type alias for the function pointer to update the meta data of a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_UpdateButtonComboCallbackFunction)(void *identifier,
                                                                                   WUPSButtonCombo_ComboHandle handle,
                                                                                   const WUPSButtonCombo_CallbackOptions *callbackOptions);
/**
 * @typedef WUPSButtonCombo_UpdateButtonComboMetaFunction
 * @brief Type alias for the function pointer to update the controller mask of a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_UpdateControllerMaskFunction)(void *identifier,
                                                                              WUPSButtonCombo_ComboHandle handle,
                                                                              WUPSButtonCombo_ControllerTypes controllerMask,
                                                                              WUPSButtonCombo_ComboStatus *outStatus);

/**
 * @typedef WUPSButtonCombo_UpdateButtonCallbackFunction
 * @brief Type alias for the function pointer to update the callback data of a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_UpdateButtonComboFunction)(void *identifier,
                                                                           WUPSButtonCombo_ComboHandle handle,
                                                                           WUPSButtonCombo_Buttons combo,
                                                                           WUPSButtonCombo_ComboStatus *outStatus);

/**
 * @typedef WUPSButtonCombo_UpdateButtonComboInfoFunction
 * @brief Type alias for the function pointer to update the info data of a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_UpdateHoldDurationFunction)(void *identifier,
                                                                            WUPSButtonCombo_ComboHandle handle,
                                                                            uint32_t holdDurationInMs);

/**
 * @typedef WUPSButtonCombo_GetButtonComboMetaFunction
 * @brief Type alias for the function pointer to get the meta data of a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_GetButtonComboMetaFunction)(void *identifier,
                                                                            WUPSButtonCombo_ComboHandle handle,
                                                                            WUPSButtonCombo_MetaOptionsOut *outOptions);

/**
 * @typedef WUPSButtonCombo_GetButtonComboCallbackFunction
 * @brief Type alias for the function pointer to get the callback data of a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_GetButtonComboCallbackFunction)(void *identifier,
                                                                                WUPSButtonCombo_ComboHandle handle,
                                                                                WUPSButtonCombo_CallbackOptions *outOptions);

/**
 * @typedef WUPSButtonCombo_GetButtonComboInfoFunction
 * @brief Type alias for the function pointer to get the info data of a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_GetButtonComboInfoExFunction)(void *identifier,
                                                                              WUPSButtonCombo_ComboHandle handle,
                                                                              WUPSButtonCombo_ButtonComboInfoEx *outOptions);

/**
 * @typedef WUPSButtonCombo_CheckComboAvailableFunction
 * @brief Type alias for the function pointer check if a button combo is available. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_CheckComboAvailableFunction)(void *identifier,
                                                                             const WUPSButtonCombo_ButtonComboOptions *options,
                                                                             WUPSButtonCombo_ComboStatus *outStatus);

/**
 * @typedef WUPSButtonCombo_CheckComboAvailableFunction
 * @brief Type alias for the function pointer to detect a button combo. For internal usage only.
 */
typedef WUPSButtonCombo_Error (*WUPSButtonCombo_DetectButtonComboBlockingFunction)(void *identifier,
                                                                                   const WUPSButtonCombo_DetectButtonComboOptions *options,
                                                                                   WUPSButtonCombo_Buttons *outButtons);

/**
 * @typedef WUPS_BUTTON_COMBO_API_VERSION
 * @brief Type alias for the API version. For internal usage only.
 */
typedef uint32_t WUPS_BUTTON_COMBO_API_VERSION;

/**
 * @def WUPS_BUTTON_COMBO_CUR_API_VERSION
 * @brief Current version of the button_combo API. For internal usage only.
 */
#define WUPS_BUTTON_COMBO_CUR_API_VERSION 0x01

/**
 * @struct wups_loader_init_button_combo_args_t_
 * @brief Structure containing initialization arguments for the button_combo API. For internal usage only.
 */
typedef struct wups_loader_init_button_combo_args_t_ {
    WUPS_BUTTON_COMBO_API_VERSION version;                                                       /**< API version. */
    void *identifier;                                                                            /**< Unique identifier */
    WUPSButtonCombo_AddButtonComboFunction add_button_combo_function_ptr;                        /**< AddButtonCombo function pointer. */
    WUPSButtonCombo_RemoveButtonComboFunction remove_button_combo_function_ptr;                  /**< RemoveButtonCombo function pointer. */
    WUPSButtonCombo_GetButtonComboStatusFunction get_button_combo_status_function_ptr;           /**< GetButtonComboStatus function pointer. */
    WUPSButtonCombo_UpdateButtonComboMetaFunction update_button_combo_meta_function_ptr;         /**< UpdateButtonComboMeta function pointer. */
    WUPSButtonCombo_UpdateButtonComboCallbackFunction update_button_combo_callback_function_ptr; /**< UpdateButtonComboCallback function pointer. */
    WUPSButtonCombo_UpdateControllerMaskFunction update_controller_mask_function_ptr;            /**< UpdateButtonComboExFunction function pointer. */
    WUPSButtonCombo_UpdateButtonComboFunction update_button_combo_function_ptr;                  /**< UpdateButtonComboExFunction function pointer. */
    WUPSButtonCombo_UpdateHoldDurationFunction update_hold_duration_function_ptr;                /**< UpdateButtonComboExFunction function pointer. */
    WUPSButtonCombo_GetButtonComboMetaFunction get_button_combo_meta_function_ptr;               /**< GetButtonComboMeta function pointer. */
    WUPSButtonCombo_GetButtonComboCallbackFunction get_button_combo_callback_function_ptr;       /**< GetButtonComboCallback function pointer. */
    WUPSButtonCombo_GetButtonComboInfoExFunction get_button_combo_info_ex_function_ptr;          /**< GetButtonComboEx function pointer. */
    WUPSButtonCombo_CheckComboAvailableFunction check_button_combo_available_function_ptr;       /**< CheckComboAvailable function pointer. */
    WUPSButtonCombo_DetectButtonComboBlockingFunction detect_button_combo_blocking_function_ptr; /**< DetectButtonCombo_Blocking function pointer. */
} wups_loader_init_button_combo_args_t;
