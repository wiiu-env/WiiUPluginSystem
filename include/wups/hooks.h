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
    WUPS_LOADER_HOOK_INIT_WUT_MALLOC,
    WUPS_LOADER_HOOK_FINI_WUT_MALLOC,
    WUPS_LOADER_HOOK_INIT_WUT_NEWLIB,
    WUPS_LOADER_HOOK_FINI_WUT_NEWLIB,
    WUPS_LOADER_HOOK_INIT_WUT_STDCPP,
    WUPS_LOADER_HOOK_FINI_WUT_STDCPP,
    WUPS_LOADER_HOOK_INIT_WUT_DEVOPTAB,
    WUPS_LOADER_HOOK_FINI_WUT_DEVOPTAB,

    WUPS_LOADER_HOOK_INIT_PLUGIN,                   /* Called when exiting the plugin loader */
    WUPS_LOADER_HOOK_DEINIT_PLUGIN,                 /* Called when re-entering the plugin loader */
    WUPS_LOADER_HOOK_APPLICATION_STARTS,            /* Called when an application gets started */
    WUPS_LOADER_HOOK_FUNCTIONS_PATCHED,             /* Called when the functions where patched */
    WUPS_LOADER_HOOK_RELEASE_FOREGROUND,            /* Called when an foreground is going to be released */
    WUPS_LOADER_HOOK_ACQUIRED_FOREGROUND,           /* Called when an foreground is acquired */
    WUPS_LOADER_HOOK_APPLICATION_REQUESTS_EXIT,     /* Called when an application wants to exit */
    WUPS_LOADER_HOOK_APPLICATION_ENDS,              /* Called when an application ends */
    WUPS_LOADER_HOOK_VSYNC,                         /* Called when an application calls GX2WaitForVsync (most times each frame) */
} wups_loader_hook_type_t;

typedef struct wups_loader_hook_t {
    wups_loader_hook_type_t type;       /*  Defines the type of the hook */
    const void *target;                 /*  Address of our own, new function */
} wups_loader_hook_t;

#define INITIALIZE_PLUGIN() \
    void init_plugin(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_PLUGIN,init_plugin); \
    void init_plugin()

#define DEINITIALIZE_PLUGIN() \
    void deinit_plugin(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_DEINIT_PLUGIN,deinit_plugin); \
    void deinit_plugin()

#define ON_APPLICATION_START() \
    void on_app_starting();\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_APPLICATION_STARTS,on_app_starting); \
    void on_app_starting()

#define ON_FUNCTIONS_PATCHED() \
    void on_functions_patched();\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FUNCTIONS_PATCHED,on_functions_patched); \
    void on_functions_patched()

#define ON_RELEASE_FOREGROUND() \
    void on_release_foreground(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_RELEASE_FOREGROUND,on_release_foreground); \
    void on_release_foreground(void)

#define ON_ACQUIRED_FOREGROUND() \
    void on_acquired_foreground(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_ACQUIRED_FOREGROUND,on_acquired_foreground); \
    void on_acquired_foreground(void)
    
#define ON_APPLICATION_REQUESTS_EXIT() \
    void on_app_requests_exit(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_APPLICATION_REQUESTS_EXIT,on_app_requests_exit); \
    void on_app_requests_exit(void)

#define ON_APPLICATION_ENDS() \
    void on_app_ending(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_APPLICATION_ENDS,on_app_ending); \
    void on_app_ending(void)

#define ON_VYSNC() \
    void on_vsync(void);\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_VSYNC,on_vsync); \
    void on_vsync(void)


#ifdef __cplusplus
#define __EXTERN_C_MACRO extern "C" 
#else
#define __EXTERN_C_MACRO
#endif

#define WUPS_USE_WUT_MALLOC() \
    __EXTERN_C_MACRO void __init_wut_malloc(); \
    void on_init_wut_malloc(){ \
        __init_wut_malloc(); \
    }\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WUT_MALLOC,on_init_wut_malloc); \
    __EXTERN_C_MACRO void __fini_wut_malloc(); \
    void on_fini_wut_malloc(){ \
        __fini_wut_malloc(); \
    } \
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FINI_WUT_MALLOC,on_fini_wut_malloc); \

#define WUPS_USE_WUT_DEVOPTAB() \
    __EXTERN_C_MACRO void __init_wut_devoptab(); \
    void on_init_wut_devoptab(){ \
        __init_wut_devoptab(); \
    }\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WUT_DEVOPTAB,on_init_wut_devoptab); \
    __EXTERN_C_MACRO void __fini_wut_devoptab(); \
    void on_fini_wut_devoptab(){ \
        __fini_wut_devoptab(); \
    }\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FINI_WUT_DEVOPTAB,on_fini_wut_devoptab);

#define WUPS_USE_WUT_NEWLIB() \
    __EXTERN_C_MACRO void __init_wut_newlib(); \
    void on_init_wut_newlib(){ \
        __init_wut_newlib(); \
    }\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WUT_NEWLIB,on_init_wut_newlib); \
    __EXTERN_C_MACRO void __fini_wut_newlib(); \
    void on_fini_wut_newlib(){ \
        __fini_wut_newlib(); \
    }\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FINI_WUT_NEWLIB,on_fini_wut_newlib);    
    
#define WUPS_USE_WUT_STDCPP() \
    __EXTERN_C_MACRO void __init_wut_stdcpp(); \
    void on_init_wut_stdcpp(){ \
        __init_wut_stdcpp(); \
    }\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_INIT_WUT_STDCPP,on_init_wut_stdcpp); \
    __EXTERN_C_MACRO  void __fini_wut_stdcpp(); \
    void on_fini_wut_stdcpp(){ \
        __fini_wut_stdcpp(); \
    }\
    WUPS_HOOK_EX(WUPS_LOADER_HOOK_FINI_WUT_STDCPP,on_fini_wut_stdcpp);  

#ifdef __cplusplus
}
#endif

#endif /* WUPS_WUPS_H_ */
