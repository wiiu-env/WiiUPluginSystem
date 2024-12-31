#include <cstdint>
#include <wups/button_combo/api.h>
#include <wups/button_combo_internal.h>

struct wups_internal_button_combo_functions_t {
    void *identifier                                                                            = nullptr;
    WUPSButtonCombo_AddButtonComboFunction add_button_combo_function_ptr                        = nullptr;
    WUPSButtonCombo_RemoveButtonComboFunction remove_button_combo_function_ptr                  = nullptr;
    WUPSButtonCombo_GetButtonComboStatusFunction get_button_combo_status_function_ptr           = nullptr;
    WUPSButtonCombo_UpdateButtonComboMetaFunction update_button_combo_meta_function_ptr         = nullptr;
    WUPSButtonCombo_UpdateButtonComboCallbackFunction update_button_combo_callback_function_ptr = nullptr;
    WUPSButtonCombo_UpdateControllerMaskFunction update_controller_mask_function_ptr            = nullptr;
    WUPSButtonCombo_UpdateButtonComboFunction update_button_combo_function_ptr                  = nullptr;
    WUPSButtonCombo_UpdateHoldDurationFunction update_hold_duration_function_ptr                = nullptr;
    WUPSButtonCombo_GetButtonComboMetaFunction get_button_combo_meta_function_ptr               = nullptr;
    WUPSButtonCombo_GetButtonComboCallbackFunction get_button_combo_callback_function_ptr       = nullptr;
    WUPSButtonCombo_GetButtonComboInfoExFunction get_button_combo_info_ex_function_ptr          = nullptr;
    WUPSButtonCombo_CheckComboAvailableFunction check_button_combo_available_function_ptr       = nullptr;
    WUPSButtonCombo_DetectButtonComboBlockingFunction detect_button_combo_blocking_function_ptr = nullptr;
};

static wups_internal_button_combo_functions_t __internal_functions = {};

extern "C" WUPSButtonCombo_Error WUPSButtonComboAPI_InitInternal(const wups_loader_init_button_combo_args_t &args) {
    if (args.version > WUPS_BUTTON_COMBO_CUR_API_VERSION) {
        __internal_functions = {};
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_INVALID_VERSION;
    }
    __internal_functions.identifier                                = args.identifier;
    __internal_functions.add_button_combo_function_ptr             = args.add_button_combo_function_ptr;
    __internal_functions.remove_button_combo_function_ptr          = args.remove_button_combo_function_ptr;
    __internal_functions.get_button_combo_status_function_ptr      = args.get_button_combo_status_function_ptr;
    __internal_functions.update_button_combo_meta_function_ptr     = args.update_button_combo_meta_function_ptr;
    __internal_functions.update_button_combo_callback_function_ptr = args.update_button_combo_callback_function_ptr;
    __internal_functions.update_controller_mask_function_ptr       = args.update_controller_mask_function_ptr;
    __internal_functions.update_button_combo_function_ptr          = args.update_button_combo_function_ptr;
    __internal_functions.update_hold_duration_function_ptr         = args.update_hold_duration_function_ptr;
    __internal_functions.get_button_combo_meta_function_ptr        = args.get_button_combo_meta_function_ptr;
    __internal_functions.get_button_combo_callback_function_ptr    = args.get_button_combo_callback_function_ptr;
    __internal_functions.get_button_combo_info_ex_function_ptr     = args.get_button_combo_info_ex_function_ptr;
    __internal_functions.check_button_combo_available_function_ptr = args.check_button_combo_available_function_ptr;
    __internal_functions.detect_button_combo_blocking_function_ptr = args.detect_button_combo_blocking_function_ptr;

    return WUPS_BUTTON_COMBO_ERROR_SUCCESS;
}

const char *WUPSButtonComboAPI_GetStatusStr(const WUPSButtonCombo_Error status) {
    switch (status) {
        case WUPS_BUTTON_COMBO_ERROR_SUCCESS:
            return "WUPS_BUTTON_COMBO_ERROR_SUCCESS";
        case WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS:
            return "WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS";
        case WUPS_BUTTON_COMBO_ERROR_MALLOC_FAILED:
            return "WUPS_BUTTON_COMBO_ERROR_MALLOC_FAILED";
        case WUPS_BUTTON_COMBO_ERROR_NOT_FOUND:
            return "WUPS_BUTTON_COMBO_ERROR_NOT_FOUND";
        case WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED:
            return "WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED";
        case WUPS_BUTTON_COMBO_ERROR_INTERNAL_INVALID_VERSION:
            return "WUPS_BUTTON_COMBO_ERROR_INTERNAL_INVALID_VERSION";
        case WUPS_BUTTON_COMBO_ERROR_UNKNOWN_ERROR:
            return "WUPS_BUTTON_COMBO_ERROR_UNKNOWN_ERROR";
        case WUPS_BUTTON_COMBO_ERROR_ABORTED:
            return "WUPS_BUTTON_COMBO_ERROR_ABORTED";
    }
    return "WUPS_BUTTON_COMBO_ERROR_UNKNOWN_ERROR";
}

static WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboPressEx(const char *label,
                                                                      const WUPSButtonCombo_Buttons combo,
                                                                      const WUPSButtonCombo_ComboCallback callback,
                                                                      void *context,
                                                                      const bool observer,
                                                                      WUPSButtonCombo_ComboHandle *outHandle,
                                                                      WUPSButtonCombo_ComboStatus *outStatus) {
    WUPSButtonCombo_ComboOptions options                 = {};
    options.metaOptions.label                            = label;
    options.callbackOptions                              = {.callback = callback, .context = context};
    options.buttonComboOptions.type                      = observer ? WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN_OBSERVER : WUPS_BUTTON_COMBO_COMBO_TYPE_PRESS_DOWN;
    options.buttonComboOptions.basicCombo.combo          = combo;
    options.buttonComboOptions.basicCombo.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;

    return WUPSButtonComboAPI_AddButtonCombo(&options, outHandle, outStatus);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboPressDown(const char *label,
                                                                 const WUPSButtonCombo_Buttons combo,
                                                                 const WUPSButtonCombo_ComboCallback callback,
                                                                 void *context,
                                                                 WUPSButtonCombo_ComboHandle *outHandle,
                                                                 WUPSButtonCombo_ComboStatus *outStatus) {
    return WUPSButtonComboAPI_AddButtonComboPressEx(label, combo, callback, context, false, outHandle, outStatus);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboPressDownObserver(const char *label,
                                                                         const WUPSButtonCombo_Buttons combo,
                                                                         const WUPSButtonCombo_ComboCallback callback,
                                                                         void *context,
                                                                         WUPSButtonCombo_ComboHandle *outHandle,
                                                                         WUPSButtonCombo_ComboStatus *outStatus) {
    return WUPSButtonComboAPI_AddButtonComboPressEx(label, combo, callback, context, true, outHandle, outStatus);
}

static WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboHoldEx(const char *label,
                                                                     const WUPSButtonCombo_Buttons combo,
                                                                     const uint32_t holdDurationInMs,
                                                                     const WUPSButtonCombo_ComboCallback callback,
                                                                     void *context,
                                                                     const bool observer,
                                                                     WUPSButtonCombo_ComboHandle *outHandle,
                                                                     WUPSButtonCombo_ComboStatus *outStatus) {
    WUPSButtonCombo_ComboOptions options                 = {};
    options.metaOptions.label                            = label;
    options.callbackOptions                              = {.callback = callback, .context = context};
    options.buttonComboOptions.type                      = observer ? WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD_OBSERVER : WUPS_BUTTON_COMBO_COMBO_TYPE_HOLD;
    options.buttonComboOptions.basicCombo.combo          = combo;
    options.buttonComboOptions.basicCombo.controllerMask = WUPS_BUTTON_COMBO_CONTROLLER_ALL;
    options.buttonComboOptions.optionalHoldForXMs        = holdDurationInMs;

    return WUPSButtonComboAPI_AddButtonCombo(&options, outHandle, outStatus);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboHold(const char *label,
                                                            const WUPSButtonCombo_Buttons combo,
                                                            const uint32_t holdDurationInMs,
                                                            const WUPSButtonCombo_ComboCallback callback,
                                                            void *context,
                                                            WUPSButtonCombo_ComboHandle *outHandle,
                                                            WUPSButtonCombo_ComboStatus *outStatus) {
    return WUPSButtonComboAPI_AddButtonComboHoldEx(label, combo, holdDurationInMs, callback, context, false, outHandle, outStatus);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonComboHoldObserver(const char *label,
                                                                    const WUPSButtonCombo_Buttons combo,
                                                                    const uint32_t holdDurationInMs,
                                                                    const WUPSButtonCombo_ComboCallback callback,
                                                                    void *context,
                                                                    WUPSButtonCombo_ComboHandle *outHandle,
                                                                    WUPSButtonCombo_ComboStatus *outStatus) {
    return WUPSButtonComboAPI_AddButtonComboHoldEx(label, combo, holdDurationInMs, callback, context, true, outHandle, outStatus);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_AddButtonCombo(const WUPSButtonCombo_ComboOptions *options,
                                                        WUPSButtonCombo_ComboHandle *outHandle,
                                                        WUPSButtonCombo_ComboStatus *outStatus) {
    if (__internal_functions.add_button_combo_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (options == nullptr || outHandle == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS;
    }
    return __internal_functions.add_button_combo_function_ptr(__internal_functions.identifier, options, outHandle, outStatus);
}


WUPSButtonCombo_Error WUPSButtonComboAPI_RemoveButtonCombo(const WUPSButtonCombo_ComboHandle handle) {
    if (__internal_functions.remove_button_combo_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.remove_button_combo_function_ptr(__internal_functions.identifier, handle);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboStatus(const WUPSButtonCombo_ComboHandle handle,
                                                              WUPSButtonCombo_ComboStatus *outStatus) {
    if (__internal_functions.get_button_combo_status_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (outStatus == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS;
    }
    return __internal_functions.get_button_combo_status_function_ptr(__internal_functions.identifier, handle, outStatus);
}
WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateButtonComboMeta(const WUPSButtonCombo_ComboHandle handle,
                                                               const WUPSButtonCombo_MetaOptions *metaOptions) {
    if (__internal_functions.update_button_combo_meta_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (metaOptions == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS;
    }
    return __internal_functions.update_button_combo_meta_function_ptr(__internal_functions.identifier, handle, metaOptions);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateButtonComboCallback(const WUPSButtonCombo_ComboHandle handle,
                                                                   const WUPSButtonCombo_CallbackOptions *callbackOptions) {
    if (__internal_functions.update_button_combo_callback_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (callbackOptions == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS;
    }
    return __internal_functions.update_button_combo_callback_function_ptr(__internal_functions.identifier, handle, callbackOptions);
}


WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateControllerMask(const WUPSButtonCombo_ComboHandle handle,
                                                              const WUPSButtonCombo_ControllerTypes controllerMask,
                                                              WUPSButtonCombo_ComboStatus *outStatus) {
    if (__internal_functions.update_controller_mask_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.update_controller_mask_function_ptr(__internal_functions.identifier, handle, controllerMask, outStatus);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateButtonCombo(const WUPSButtonCombo_ComboHandle handle,
                                                           const WUPSButtonCombo_Buttons combo,
                                                           WUPSButtonCombo_ComboStatus *outStatus) {
    if (__internal_functions.update_button_combo_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.update_button_combo_function_ptr(__internal_functions.identifier, handle, combo, outStatus);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_UpdateHoldDuration(const WUPSButtonCombo_ComboHandle handle,
                                                            const uint32_t holdDurationInMs) {
    if (__internal_functions.update_hold_duration_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    return __internal_functions.update_hold_duration_function_ptr(__internal_functions.identifier, handle, holdDurationInMs);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboMeta(const WUPSButtonCombo_ComboHandle handle,
                                                            WUPSButtonCombo_MetaOptionsOut *outOptions) {
    if (__internal_functions.get_button_combo_meta_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (outOptions == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS;
    }
    return __internal_functions.get_button_combo_meta_function_ptr(__internal_functions.identifier, handle, outOptions);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboCallback(const WUPSButtonCombo_ComboHandle handle,
                                                                WUPSButtonCombo_CallbackOptions *outOptions) {
    if (__internal_functions.get_button_combo_callback_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (outOptions == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS;
    }
    return __internal_functions.get_button_combo_callback_function_ptr(__internal_functions.identifier, handle, outOptions);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_GetButtonComboInfoEx(const WUPSButtonCombo_ComboHandle handle,
                                                              WUPSButtonCombo_ButtonComboInfoEx *outOptions) {
    if (__internal_functions.get_button_combo_info_ex_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (outOptions == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS;
    }
    return __internal_functions.get_button_combo_info_ex_function_ptr(__internal_functions.identifier, handle, outOptions);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_CheckComboAvailable(const WUPSButtonCombo_ButtonComboOptions *options,
                                                             WUPSButtonCombo_ComboStatus *outStatus) {
    if (__internal_functions.check_button_combo_available_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (options == nullptr || outStatus == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS;
    }
    return __internal_functions.check_button_combo_available_function_ptr(__internal_functions.identifier, options, outStatus);
}

WUPSButtonCombo_Error WUPSButtonComboAPI_DetectButtonCombo_Blocking(const WUPSButtonCombo_DetectButtonComboOptions *options,
                                                                    WUPSButtonCombo_Buttons *outButtons) {
    if (__internal_functions.detect_button_combo_blocking_function_ptr == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (__internal_functions.identifier == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INTERNAL_NOT_INITIALIZED;
    }
    if (options == nullptr || outButtons == nullptr) {
        return WUPS_BUTTON_COMBO_ERROR_INVALID_ARGS;
    }
    return __internal_functions.detect_button_combo_blocking_function_ptr(__internal_functions.identifier, options, outButtons);
}
