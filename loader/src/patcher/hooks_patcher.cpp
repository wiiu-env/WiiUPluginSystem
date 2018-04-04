#include <utils/logger.h>
#include <utils/function_patcher.h>
#include "common/retain_vars.h"
#include "hooks_patcher.h"
#include "myutils/overlay_helper.h"
#include "main.h"
#include "utils.h"

DECL(void, __PPCExit, void) {
    // Only continue if we are in the "right" application.
    if(OSGetTitleID() == gGameTitleID) {
        DEBUG_FUNCTION_LINE("__PPCExit\n");
        CallHook(WUPS_LOADER_HOOK_ENDING_APPLICATION);
        DeInit();
    }

    real___PPCExit();
}

DECL(u32, ProcUIProcessMessages, u32 u) {
    u32 res = real_ProcUIProcessMessages(u);
    // Only continue if we are in the "right" application.
    if(res != gAppStatus && OSGetTitleID() == gGameTitleID) {
        DEBUG_FUNCTION_LINE("App status changed from %d to %d \n",gAppStatus,res);
        gAppStatus = res;
        CallHook(WUPS_LOADER_HOOK_APP_STATUS_CHANGED);
    }

    return res;
}

DECL(void, GX2SetTVBuffer, void *buffer, u32 buffer_size, s32 tv_render_mode, s32 format, s32 buffering_mode) {
    tv_store.buffer = buffer;
    tv_store.buffer_size = buffer_size;
    tv_store.mode = tv_render_mode;
    tv_store.surface_format = format;
    tv_store.buffering_mode = buffering_mode;

    return real_GX2SetTVBuffer(buffer,buffer_size,tv_render_mode,format,buffering_mode);
}

DECL(void, GX2SetDRCBuffer, void *buffer, u32 buffer_size, s32 drc_mode, s32 surface_format, s32 buffering_mode) {
    drc_store.buffer = buffer;
    drc_store.buffer_size = buffer_size;
    drc_store.mode = drc_mode;
    drc_store.surface_format = surface_format;
    drc_store.buffering_mode = buffering_mode;

    return real_GX2SetDRCBuffer(buffer,buffer_size,drc_mode,surface_format,buffering_mode);
}

DECL(void, GX2WaitForVsync, void) {
    CallHook(WUPS_LOADER_HOOK_VSYNC);
    real_GX2WaitForVsync();
}

hooks_magic_t method_hooks_hooks[] __attribute__((section(".data"))) = {
    MAKE_MAGIC(__PPCExit,               LIB_CORE_INIT,  STATIC_FUNCTION),
    MAKE_MAGIC(ProcUIProcessMessages,   LIB_PROC_UI,    DYNAMIC_FUNCTION),
    MAKE_MAGIC(GX2SetTVBuffer,          LIB_GX2,        STATIC_FUNCTION),
    MAKE_MAGIC(GX2SetDRCBuffer,         LIB_GX2,        STATIC_FUNCTION),
    MAKE_MAGIC(GX2WaitForVsync,         LIB_GX2,        STATIC_FUNCTION),
};


u32 method_hooks_size_hooks __attribute__((section(".data"))) = sizeof(method_hooks_hooks) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile u32 method_calls_hooks[sizeof(method_hooks_hooks) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));

