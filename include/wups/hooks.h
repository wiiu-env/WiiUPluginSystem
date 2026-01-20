/****************************************************************************
 * Copyright (C) 2018-2021 Maschell
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

#include "common.h"
#include <wups/button_combo_internal.h>
#include <wups/config.h>
#include <wups/storage.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WUPS_HOOK_EX(type_def, original_func)               \
    WUPS_SECTION("hooks")                                   \
    const wups_loader_hook_t wups_hooks_##original_func = { \
            .type   = type_def,                             \
            .target = (const void *) &(original_func)}

typedef enum wups_loader_hook_type_t {
    WUPS_LOADER_HOOK_INIT_WUT_MALLOC,
    WUPS_LOADER_HOOK_FINI_WUT_MALLOC,
    WUPS_LOADER_HOOK_INIT_WUT_NEWLIB,
    WUPS_LOADER_HOOK_FINI_WUT_NEWLIB,
    WUPS_LOADER_HOOK_INIT_WUT_STDCPP,
    WUPS_LOADER_HOOK_FINI_WUT_STDCPP,
    WUPS_LOADER_HOOK_INIT_WUT_DEVOPTAB,
    WUPS_LOADER_HOOK_FINI_WUT_DEVOPTAB,
    WUPS_LOADER_HOOK_INIT_WUT_SOCKETS,
    WUPS_LOADER_HOOK_FINI_WUT_SOCKETS,

    WUPS_LOADER_HOOK_INIT_WRAPPER, /* Calls __init */
    WUPS_LOADER_HOOK_FINI_WRAPPER, /* Calls __fini */

    WUPS_LOADER_HOOK_GET_CONFIG_DEPRECATED,    /* Deprecated implementation */
    WUPS_LOADER_HOOK_CONFIG_CLOSED_DEPRECATED, /* Deprecated implementation */

    WUPS_LOADER_HOOK_INIT_STORAGE_DEPRECATED, /* Deprecated implementation */

    WUPS_LOADER_HOOK_INIT_PLUGIN,               /* Called when exiting the plugin loader */
    WUPS_LOADER_HOOK_DEINIT_PLUGIN,             /* Called when re-entering the plugin loader */
    WUPS_LOADER_HOOK_APPLICATION_STARTS,        /* Called when an application gets started */
    WUPS_LOADER_HOOK_RELEASE_FOREGROUND,        /* Called when a foreground is going to be released */
    WUPS_LOADER_HOOK_ACQUIRED_FOREGROUND,       /* Called when a foreground is acquired */
    WUPS_LOADER_HOOK_APPLICATION_REQUESTS_EXIT, /* Called when an application wants to exit */
    WUPS_LOADER_HOOK_APPLICATION_ENDS,          /* Called when an application ends */

    WUPS_LOADER_HOOK_INIT_STORAGE,      /* Only for internal usage */
    WUPS_LOADER_HOOK_INIT_CONFIG,       /* Only for internal usage */
    WUPS_LOADER_HOOK_INIT_BUTTON_COMBO, /* Only for internal usage */
    WUPS_LOADER_HOOK_INIT_WUT_THREAD,   /* Only for internal usage */
} wups_loader_hook_type_t;

typedef struct wups_loader_hook_t {
    wups_loader_hook_type_t type; /*  Defines the type of the hook */
    const void *target;           /*  Address of our own, new function */
} wups_loader_hook_t;

#define INITIALIZE_PLUGIN()                                  \
    void init_plugin(void);                                  \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_PLUGIN, init_plugin); \
    void init_plugin()

#define DEINITIALIZE_PLUGIN()                                    \
    void deinit_plugin(void);                                    \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_DEINIT_PLUGIN, deinit_plugin); \
    void deinit_plugin()

#define ON_APPLICATION_START()                                          \
    void on_app_starting();                                             \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_APPLICATION_STARTS, on_app_starting); \
    void on_app_starting()

#define ON_RELEASE_FOREGROUND()                                               \
    void on_release_foreground(void);                                         \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_RELEASE_FOREGROUND, on_release_foreground); \
    void on_release_foreground(void)

#define ON_ACQUIRED_FOREGROUND()                                                \
    void on_acquired_foreground(void);                                          \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_ACQUIRED_FOREGROUND, on_acquired_foreground); \
    void on_acquired_foreground(void)

#define ON_APPLICATION_REQUESTS_EXIT()                                              \
    void on_app_requests_exit(void);                                                \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_APPLICATION_REQUESTS_EXIT, on_app_requests_exit); \
    void on_app_requests_exit(void)

#define ON_APPLICATION_ENDS()                                       \
    void on_app_ending(void);                                       \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_APPLICATION_ENDS, on_app_ending); \
    void on_app_ending(void)

#define WUPS_GET_CONFIG(x) \
    static_assert(false, "Please use \"WUPSConfigAPI_Init\" inside \"INITIALIZE_PLUGIN\" to provide a callback instead");

#define WUPS_CONFIG_CLOSED() \
    static_assert(false, "Please use \"WUPSConfigAPI_Init\" inside \"INITIALIZE_PLUGIN\" to provide a callback instead");

#define WUPS_USE_STORAGE(x)                                    \
    WUPS_META(storage_id, x);                                  \
    void init_storage(wups_loader_init_storage_args_t);        \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_STORAGE, init_storage); \
    void init_storage(wups_loader_init_storage_args_t args) {  \
        WUPSStorageAPI_InitInternal(args);                     \
    }


#ifdef __cplusplus
#define __EXTERN_C_MACRO extern "C"
#else
#define __EXTERN_C_MACRO
#endif

#define WUPS_INIT_CONFIG_FUNCTIONS()                                                                              \
    __EXTERN_C_MACRO WUPSConfigAPIStatus WUPSConfigAPI_InitLibrary_Internal(wups_loader_init_config_args_t args); \
    WUPSConfigAPIStatus wups_init_config_functions(wups_loader_init_config_args_t);                               \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_CONFIG, wups_init_config_functions);                                       \
    WUPSConfigAPIStatus wups_init_config_functions(wups_loader_init_config_args_t args) {                         \
        return WUPSConfigAPI_InitLibrary_Internal(args);                                                          \
    }

#define WUPS_INIT_BUTTON_COMBO_FUNCTIONS()                                                                             \
    __EXTERN_C_MACRO WUPSButtonCombo_Error WUPSButtonComboAPI_InitInternal(wups_loader_init_button_combo_args_t args); \
    WUPSButtonCombo_Error wups_init_button_combo_functions(wups_loader_init_button_combo_args_t);                      \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_BUTTON_COMBO, wups_init_button_combo_functions);                                \
    WUPSButtonCombo_Error wups_init_button_combo_functions(wups_loader_init_button_combo_args_t args) {                \
        return WUPSButtonComboAPI_InitInternal(args);                                                                  \
    }

#define WUPS_USE_WUT_MALLOC()                                           \
    __EXTERN_C_MACRO void __init_wut_malloc();                          \
    void on_init_wut_malloc() {                                         \
        __init_wut_malloc();                                            \
    }                                                                   \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WUT_MALLOC, on_init_wut_malloc); \
    __EXTERN_C_MACRO void __fini_wut_malloc();                          \
    void on_fini_wut_malloc() {                                         \
        __fini_wut_malloc();                                            \
    }                                                                   \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FINI_WUT_MALLOC, on_fini_wut_malloc)

#define WUPS_USE_WUT_DEVOPTAB()                                             \
    __EXTERN_C_MACRO void __init_wut_devoptab();                            \
    void on_init_wut_devoptab() {                                           \
        __init_wut_devoptab();                                              \
    }                                                                       \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WUT_DEVOPTAB, on_init_wut_devoptab); \
    __EXTERN_C_MACRO void __fini_wut_devoptab();                            \
    void on_fini_wut_devoptab() {                                           \
        __fini_wut_devoptab();                                              \
    }                                                                       \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FINI_WUT_DEVOPTAB, on_fini_wut_devoptab)

#define WUPS_USE_WUT_NEWLIB()                                           \
    __EXTERN_C_MACRO void __init_wut_newlib();                          \
    void on_init_wut_newlib() {                                         \
        __init_wut_newlib();                                            \
    }                                                                   \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WUT_NEWLIB, on_init_wut_newlib); \
    __EXTERN_C_MACRO void __fini_wut_newlib();                          \
    void on_fini_wut_newlib() {                                         \
        __fini_wut_newlib();                                            \
    }                                                                   \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FINI_WUT_NEWLIB, on_fini_wut_newlib)

#define WUPS_USE_WUT_STDCPP()                                           \
    __EXTERN_C_MACRO void __init_wut_stdcpp();                          \
    void on_init_wut_stdcpp() {                                         \
    }                                                                   \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WUT_STDCPP, on_init_wut_stdcpp); \
    __EXTERN_C_MACRO void __fini_wut_stdcpp();                          \
    void on_fini_wut_stdcpp() {                                         \
    }                                                                   \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FINI_WUT_STDCPP, on_fini_wut_stdcpp)

#define WUPS_USE_WUT_THREAD()                  \
    __EXTERN_C_MACRO void __init_wut_thread(); \
    void on_init_wut_thread() {                \
        __init_wut_thread();                   \
    }                                          \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WUT_THREAD, on_init_wut_thread);

#ifdef __cplusplus
extern "C" uint32_t __attribute__((weak)) wut_get_thread_specific(int id);
extern "C" const char wups_meta_info_linking_order[];
extern "C" void OSFatal(const char *msg);
#else
extern uint32_t __attribute__((weak)) wut_get_thread_specific(int id);
extern const char wups_meta_info_linking_order[];
extern void OSFatal(const char *msg);
#endif

#define WUPS___INIT_WRAPPER()                                    \
    __EXTERN_C_MACRO void __init();                              \
    void __init_wrapper() {                                      \
        if (wut_get_thread_specific(0x13371337) != 0x42424242) { \
            OSFatal(wups_meta_info_linking_order);               \
        }                                                        \
        __init();                                                \
    }                                                            \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WRAPPER, __init_wrapper);

#define WUPS___FINI_WRAPPER()       \
    __EXTERN_C_MACRO void __fini(); \
    void __fini_wrapper() {         \
        __fini();                   \
    }                               \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FINI_WRAPPER, __fini_wrapper);

#define WUPS_USE_WUT_SOCKETS()                                            \
    __EXTERN_C_MACRO void __attribute__((weak)) __init_wut_socket();      \
    void on_init_wut_sockets() {                                          \
        if (&__init_wut_socket) __init_wut_socket();                      \
    }                                                                     \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WUT_SOCKETS, on_init_wut_sockets); \
    __EXTERN_C_MACRO void __attribute__((weak)) __fini_wut_socket();      \
    void on_fini_wut_sockets() {                                          \
        if (&__fini_wut_socket) __fini_wut_socket();                      \
    }                                                                     \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FINI_WUT_SOCKETS, on_fini_wut_sockets)

#ifdef __cplusplus
}
#endif
