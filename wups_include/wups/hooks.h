/****************************************************************************
 * Copyright (C) 2018 Maschell
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

#ifndef WUPS_HOOKS_DEF_H_
#define WUPS_HOOKS_DEF_H_

#include "common.h"
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WUPS_HOOK_EX(type_def,original_func) \
    extern const wups_loader_hook_t wups_hooks_ ## original_func WUPS_SECTION("hooks"); \
    const wups_loader_hook_t wups_hooks_ ## original_func = { \
        .type = type_def, \
        .target = (const void*)&(original_func) \
    }

typedef enum wups_loader_hook_type_t {
    WUPS_LOADER_HOOK_INIT_FS,               /* Only for internal usage */
    WUPS_LOADER_HOOK_INIT_OVERLAY,          /* Only for internal usage */

    WUPS_LOADER_HOOK_INIT_PLUGIN,           /* Called when exiting the plugin loader */
    WUPS_LOADER_HOOK_DEINIT_PLUGIN,         /* Called when re-entering the plugin loader */
    WUPS_LOADER_HOOK_STARTING_APPLICATION,  /* Called when an application gets started */
    WUPS_LOADER_HOOK_FUNCTIONS_PATCHED,     /* Called when the functions where patched */
    WUPS_LOADER_HOOK_ENDING_APPLICATION,    /* Called when an application ends */
    WUPS_LOADER_HOOK_VSYNC,                 /* Called when an application calls GX2WaitForVsync (most times each frame) */
    WUPS_LOADER_HOOK_APP_STATUS_CHANGED,    /* Called when the app status changes (foreground, background, closing) */

    WUPS_LOADER_HOOK_INIT_KERNEL,           /* Only for internal usage */
    WUPS_LOADER_HOOK_GET_CONFIG,            /* Called when the config-menu will be loaded */
    WUPS_LOADER_HOOK_INIT_VID_MEM,          /* Only for internal usage */
    WUPS_LOADER_HOOK_VID_DRC_DRAW,          /* Called when the DRC was rendered */
    WUPS_LOADER_HOOK_VID_TV_DRAW,           /* Called when the TV was rendered */
} wups_loader_hook_type_t;

typedef struct wups_loader_hook_t {
    wups_loader_hook_type_t type;       /*  Defines the type of the hook */
    const void *target;                 /*  Address of our own, new function */
} wups_loader_hook_t;

typedef enum wups_loader_app_status_t {
    WUPS_APP_STATUS_FOREGROUND,         /* App is now running in foreground */
    WUPS_APP_STATUS_BACKGROUND,         /* App is now running in background */
    WUPS_APP_STATUS_CLOSED,             /* App is going to be closed */
    WUPS_APP_STATUS_UNKNOWN,            /* Unknown status _should_ never happen.*/
} wups_loader_app_status_t;

typedef struct wups_loader_vid_buffer_t {
    const void * color_buffer_ptr;
    const void * tv_texture_ptr;
    const void * drc_texture_ptr;
    const void * sampler_ptr;
} wups_loader_vid_buffer_t;

typedef struct wups_loader_app_started_args_t {
    bool sd_mounted;
    bool usb_mounted;
    bool kernel_access;
} wups_loader_app_started_args_t;

#define WUPS_FS_ACCESS() \
    void init_fs(wups_loader_init_fs_args_t);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_FS,init_fs); \
    void init_fs(wups_loader_init_fs_args_t args){ \
        WUPS_InitFS(args);\
    }

#define WUPS_ALLOW_OVERLAY() \
    void init_overlay(wups_loader_init_overlay_args_t);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_OVERLAY,init_overlay); \
    void init_overlay(wups_loader_init_overlay_args_t args){ \
        WUPS_InitOverlay(args);\
    }

#define WUPS_USE_VIDEO_MEMORY() \
    void init_vid_mem(wups_loader_init_vid_mem_args_t);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_VID_MEM,init_vid_mem); \
    void init_vid_mem(wups_loader_init_vid_mem_args_t args){ \
        WUPS_InitVidMem(args);\
    }

#define WUPS_ALLOW_KERNEL() \
    void init_kernel(wups_loader_init_kernel_args_t);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_KERNEL,init_kernel); \
    void init_kernel(wups_loader_init_kernel_args_t args){ \
        WUPS_InitKernel(args);\
    }

#define INITIALIZE_PLUGIN() \
    void init_plugin(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_PLUGIN,init_plugin); \
    void init_plugin()

#define DEINITIALIZE_PLUGIN() \
    void deinit_plugin(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_DEINIT_PLUGIN,deinit_plugin); \
    void deinit_plugin()

#define ON_APPLICATION_START(myargs) \
    void on_app_starting(wups_loader_app_started_args_t);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_STARTING_APPLICATION,on_app_starting); \
    void on_app_starting(wups_loader_app_started_args_t myargs)

#define ON_FUNCTIONS_PATCHED() \
    void on_functions_patched();\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FUNCTIONS_PATCHED,on_functions_patched); \
    void on_functions_patched()

#define ON_APPLICATION_ENDING() \
    void on_app_ending(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_ENDING_APPLICATION,on_app_ending); \
    void on_app_ending(void)

#define ON_VYSNC() \
    void on_vsync(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_VSYNC,on_vsync); \
    void on_vsync(void)

#define ON_APP_STATUS_CHANGED(status) \
    void on_app_status_changed(wups_loader_app_status_t);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_APP_STATUS_CHANGED,on_app_status_changed); \
    void on_app_status_changed(wups_loader_app_status_t status)

#define ON_DRC_TO_SCAN_BUFFER(myargs) \
    void on_drc_to_scan_buf(wups_loader_vid_buffer_t);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_VID_DRC_DRAW,on_drc_to_scan_buf); \
    void on_drc_to_scan_buf(wups_loader_vid_buffer_t myargs)

#define ON_TV_TO_SCAN_BUFFER(myargs) \
    void on_tv_to_scan_buf(wups_loader_vid_buffer_t);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_VID_TV_DRAW,on_tv_to_scan_buf); \
    void on_tv_to_scan_buf(wups_loader_vid_buffer_t myargs)

#ifdef __cplusplus
}
#endif

#endif /* WUPS_WUPS_H_ */
