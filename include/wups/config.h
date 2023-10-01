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

#include <stdint.h>

#define WUPS_CONFIG_BUTTON_NONE    0
#define WUPS_CONFIG_BUTTON_LEFT    (1 << 0)
#define WUPS_CONFIG_BUTTON_RIGHT   (1 << 1)
#define WUPS_CONFIG_BUTTON_UP      (1 << 2)
#define WUPS_CONFIG_BUTTON_DOWN    (1 << 3)
#define WUPS_CONFIG_BUTTON_A       (1 << 4)
#define WUPS_CONFIG_BUTTON_B       (1 << 5)
#define WUPS_CONFIG_BUTTON_ZL      (1 << 6)
#define WUPS_CONFIG_BUTTON_ZR      (1 << 7)
#define WUPS_CONFIG_BUTTON_L       (1 << 8)
#define WUPS_CONFIG_BUTTON_R       (1 << 9)
#define WUPS_CONFIG_BUTTON_X       (1 << 10)
#define WUPS_CONFIG_BUTTON_Y       (1 << 11)
#define WUPS_CONFIG_BUTTON_STICK_L (1 << 12)
#define WUPS_CONFIG_BUTTON_STICK_R (1 << 13)
#define WUPS_CONFIG_BUTTON_PLUS    (1 << 14)
#define WUPS_CONFIG_BUTTON_MINUS   (1 << 15)
typedef int32_t WUPSConfigButtons;

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

#define WUPS_API_ITEM_OPTION_VERSION_V1 1
typedef struct WUPSConfigAPIItemOptionsV1 {
    const char *displayName;
    void *context;
    WUPSConfigAPIItemCallbacksV1 callbacks;
} WUPSConfigAPIItemOptionsV1;

typedef struct WUPSConfigAPICreateItemOptions {
    uint32_t version;
    union {
        WUPSConfigAPIItemOptionsV1 v1;
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
