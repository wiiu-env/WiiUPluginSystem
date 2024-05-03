/****************************************************************************
 * Copyright (C) 2021 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#pragma once

#include <padscore/kpad.h>
#include <stdint.h>
#include <vpad/input.h>

typedef uint32_t WUPSConfigButtons;

typedef enum WUPS_CONFIG_SIMPLE_INPUT {
    WUPS_CONFIG_BUTTON_NONE    = 0,
    WUPS_CONFIG_BUTTON_LEFT    = (1 << 0),
    WUPS_CONFIG_BUTTON_RIGHT   = (1 << 1),
    WUPS_CONFIG_BUTTON_UP      = (1 << 2),
    WUPS_CONFIG_BUTTON_DOWN    = (1 << 3),
    WUPS_CONFIG_BUTTON_A       = (1 << 4),
    WUPS_CONFIG_BUTTON_B       = (1 << 5),
    WUPS_CONFIG_BUTTON_ZL      = (1 << 6),
    WUPS_CONFIG_BUTTON_ZR      = (1 << 7),
    WUPS_CONFIG_BUTTON_L       = (1 << 8),
    WUPS_CONFIG_BUTTON_R       = (1 << 9),
    WUPS_CONFIG_BUTTON_X       = (1 << 10),
    WUPS_CONFIG_BUTTON_Y       = (1 << 11),
    WUPS_CONFIG_BUTTON_STICK_L = (1 << 12),
    WUPS_CONFIG_BUTTON_STICK_R = (1 << 13),
    WUPS_CONFIG_BUTTON_PLUS    = (1 << 14),
    WUPS_CONFIG_BUTTON_MINUS   = (1 << 15),
} WUPS_CONFIG_SIMPLE_INPUT;

typedef struct {
    WUPS_CONFIG_SIMPLE_INPUT buttons_h;
    WUPS_CONFIG_SIMPLE_INPUT buttons_d;
    WUPS_CONFIG_SIMPLE_INPUT buttons_r;
    bool validPointer;
    bool touched;
    float pointerAngle;
    int32_t x;
    int32_t y;
} WUPSConfigSimplePadData;

typedef struct {
    struct {
        VPADReadError vpadError;
        VPADTouchData tpCalib;
        VPADStatus data;
    } vpad;
    struct {
        KPADError kpadError[7];
        KPADStatus data[7];
    } kpad;
} WUPSConfigComplexPadData;

typedef enum WUPSConfigAPIStatus {
    WUPSCONFIG_API_RESULT_SUCCESS                   = 0,
    WUPSCONFIG_API_RESULT_INVALID_ARGUMENT          = -0x01,
    WUPSCONFIG_API_RESULT_OUT_OF_MEMORY             = -0x03,
    WUPSCONFIG_API_RESULT_NOT_FOUND                 = -0x06,
    WUPSCONFIG_API_RESULT_INVALID_PLUGIN_IDENTIFIER = -0x70,
    WUPSCONFIG_API_RESULT_MISSING_CALLBACK          = -0x71,
    WUPSCONFIG_API_RESULT_MODULE_NOT_FOUND          = -0x80,
    WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT     = -0x81,
    WUPSCONFIG_API_RESULT_UNSUPPORTED_VERSION       = -0x82,
    WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND       = -0x83,
    WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED         = -0x84,
    WUPSCONFIG_API_RESULT_UNKNOWN_ERROR             = -0x100,
} WUPSConfigAPIStatus;

typedef enum WUPSConfigAPICallbackStatus {
    WUPSCONFIG_API_CALLBACK_RESULT_SUCCESS = 0,
    WUPSCONFIG_API_CALLBACK_RESULT_ERROR   = -1,
} WUPSConfigAPICallbackStatus;

typedef struct {
    int32_t (*getCurrentValueDisplay)(void *context, char *out_buf, int32_t out_size);

    int32_t (*getCurrentValueSelectedDisplay)(void *context, char *out_buf, int32_t out_size);

    void (*onSelected)(void *context, bool isSelected);

    void (*restoreDefault)(void *context);

    bool (*isMovementAllowed)(void *context);

    bool (*callCallback)(void *context);

    void (*onButtonPressed)(void *context, WUPSConfigButtons button);

    void (*onDelete)(void *context);
} WUPSConfigAPIItemCallbacksV1;

typedef struct {
    /**
     * Set the string which is displayed for an item
     * @param context  The context which has been passed to the item during creating
     * @param out_buf  Buffer where the string should be written to
     * @param out_size Size of out_buf
     *
     * \result non-zero result indicates an error.
     */
    int32_t (*getCurrentValueDisplay)(void *context, char *out_buf, int32_t out_size);

    /**
     * Set the string which is displayed for an item when the cursor is on is item
     * @param context  The context which has been passed to the item during creating
     * @param out_buf  Buffer where the string should be written to
     * @param out_size Size of out_buf
     *
     * \result non-zero result indicates an error.
     */
    int32_t (*getCurrentValueSelectedDisplay)(void *context, char *out_buf, int32_t out_size);

    /**
     * Called when the cursor enters or leaves this item
     * @param context    The context which has been passed to the item during creating
     * @param isSelected True if the item cursor is now pointing to this item, \n
     *                   False if it's not pointing to this item anymore
     */
    void (*onSelected)(void *context, bool isSelected);

    /**
     * Called when the current value of this item should be set to the default value
     * @param context The context which has been passed to the item during creating
     */
    void (*restoreDefault)(void *context);

    /**
     * Determines if movement to different item is allowed.
     * @param context The context which has been passed to the item during creating
     * \return True if it should be not possible to select a different item or exit the current category \n
     *         False if it should be possible to select a different item or exit the current category
     */
    bool (*isMovementAllowed)(void *context);

    /**
     * Called when the config menu has been closed
     * @param context The context which has been passed to the item during creating
     */
    void (*onCloseCallback)(void *context);

    /**
     * This function is called on each frame and provides information about the current inputs.
     * The inputs are simplified and all 5 possible controller inputs (from Gamepad and up to 4 Wiimotes/Pro Controller)
     * are unified in this single unified struct.
     *
     * @param context The context which has been passed to the item during creating
     * @param input Simplified version of the current inputs
     *
     * \note To get the full input for all possible controllers see "onInputEx"
     *
     * @see onInputEx
     */
    void (*onInput)(void *context, WUPSConfigSimplePadData input);

    /**
     * This function is called on each frame and provides information about the current inputs.
     * The structs contains information for current individual Gampepad and Wiimote/Pro Contoller inputs.
     *
     * @param context The context which has been passed to the item during creating
     * @param input current input for all possibles controller
     *
     * \note To get a simplified input callback that combines all controller into a single struct see "onInput"
     *
     * @see onInput
     */
    void (*onInputEx)(void *context, WUPSConfigComplexPadData input);

    /**
     * This function is called when the item is about to be deleted. It can be used to free any alloated memory.
     *
     * @param context The context which has been passed to the item during creating
     */
    void (*onDelete)(void *context);
} WUPSConfigAPIItemCallbacksV2;

#define WUPS_API_ITEM_OPTION_VERSION_V1 1
#define WUPS_API_ITEM_OPTION_VERSION_V2 2

typedef struct WUPSConfigAPIItemOptionsV1 {
    const char *configId;
    const char *displayName;
    void *context;
    WUPSConfigAPIItemCallbacksV1 callbacks;
} WUPSConfigAPIItemOptionsV1;

typedef struct WUPSConfigAPIItemOptionsV2 {
    const char *displayName;
    void *context;
    WUPSConfigAPIItemCallbacksV2 callbacks;
} WUPSConfigAPIItemOptionsV2;

typedef struct WUPSConfigAPICreateItemOptions {
    uint32_t version;
    union {
        WUPSConfigAPIItemOptionsV1 v1;
        WUPSConfigAPIItemOptionsV2 v2;
    } data;
} WUPSConfigAPICreateItemOptions;

typedef uint32_t WUPSConfigAPIVersion;

typedef struct WUPSConfigItemHandle {
    void *handle;
#ifdef __cplusplus
    WUPSConfigItemHandle() {
        handle = nullptr;
    }
    explicit WUPSConfigItemHandle(void *handle) : handle(handle) {}
    bool operator==(const WUPSConfigItemHandle other) const {
        return handle == other.handle;
    }
    bool operator==(const void *other) const {
        return handle == other;
    }
#endif
} WUPSConfigItemHandle;

typedef struct WUPSConfigHandle {
    void *handle;
#ifdef __cplusplus
    WUPSConfigHandle() {
        handle = nullptr;
    }
    explicit WUPSConfigHandle(void *handle) : handle(handle) {}
    bool operator==(const WUPSConfigHandle other) const {
        return handle == other.handle;
    }
    bool operator==(const void *other) const {
        return handle == other;
    }
#endif
} WUPSConfigHandle;

typedef struct WUPSConfigCategoryHandle {
    void *handle;
#ifdef __cplusplus
    WUPSConfigCategoryHandle() {
        handle = nullptr;
    }
    explicit WUPSConfigCategoryHandle(void *handle) : handle(handle) {}
    bool operator==(const WUPSConfigCategoryHandle other) const {
        return handle == other.handle;
    }
    bool operator==(const void *other) const {
        return handle == other;
    }
#endif
} WUPSConfigCategoryHandle;

#define WUPS_API_CATEGORY_OPTION_VERSION_V1 1

typedef struct WUPSConfigAPICreateCategoryOptionsV1 {
    const char *name;
} WUPSConfigAPICreateCategoryOptionsV1;

typedef struct WUPSConfigAPICreateCategoryOptions {
    uint32_t version;
    union {
        WUPSConfigAPICreateCategoryOptionsV1 v1;
    } data;
} WUPSConfigAPICreateCategoryOptions;

#define WUPS_API_CONFIG_API_OPTION_VERSION_V1 1

typedef struct WUPSConfigAPIOptionsV1 {
    const char *name;
} WUPSConfigAPIOptionsV1;

typedef struct WUPSConfigAPIOptions {
    uint32_t version;
    union {
        WUPSConfigAPIOptionsV1 v1;
    } data;
} WUPSConfigAPIOptions;

#define WUPS_CONFIG_API_VERSION_ERROR 0xFFFFFFFF

typedef struct wups_loader_init_config_args_t {
    uint32_t arg_version;
    uint32_t plugin_identifier;
} wups_loader_init_config_args_t;
