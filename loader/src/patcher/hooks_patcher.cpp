#include <utils/logger.h>
#include <utils/function_patcher.h>
#include <dynamic_libs/vpad_functions.h>
#include "common/retain_vars.h"
#include "hooks_patcher.h"
#include "myutils/overlay_helper.h"
#include "myutils/ConfigUtils.h"
#include "main.h"
#include "utils.h"
#include "mymemory/memory_mapping.h"

DECL(void, __PPCExit, void) {
    // Only continue if we are in the "right" application.
    //if(OSGetTitleID() == gGameTitleID) {
    //DEBUG_FUNCTION_LINE("__PPCExit\n");
    //CallHook(WUPS_LOADER_HOOK_ENDING_APPLICATION);
    //DeInit();
    //}

    real___PPCExit();
}

DECL(uint32_t, ProcUIProcessMessages, uint32_t u) {
    uint32_t res = real_ProcUIProcessMessages(u);
    // Only continue if we are in the "right" application.
    if(res != gAppStatus && OSGetTitleID() == gGameTitleID) {
        DEBUG_FUNCTION_LINE("App status changed from %d to %d \n",gAppStatus,res);
        gAppStatus = res;
        CallHook(WUPS_LOADER_HOOK_APP_STATUS_CHANGED);
        if(gAppStatus == WUPS_APP_STATUS_CLOSED) {
            CallHook(WUPS_LOADER_HOOK_ENDING_APPLICATION);
            DeInit();
        }
    }

    return res;
}

DECL(void, GX2SetTVBuffer, void *buffer, uint32_t buffer_size, int32_t tv_render_mode, int32_t format, int32_t buffering_mode) {
    tv_store.buffer = buffer;
    tv_store.buffer_size = buffer_size;
    tv_store.mode = tv_render_mode;
    tv_store.surface_format = format;
    tv_store.buffering_mode = buffering_mode;

    return real_GX2SetTVBuffer(buffer,buffer_size,tv_render_mode,format,buffering_mode);
}

DECL(void, GX2SetDRCBuffer, void *buffer, uint32_t buffer_size, int32_t drc_mode, int32_t surface_format, int32_t buffering_mode) {
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

uint8_t vpadPressCooldown = 0xFF;

uint8_t angleX_counter = 0;
float angleX_delta = 0.0f;
float angleX_last = 0.0f;
uint8_t angleX_frameCounter = 0;

bool checkMagic(VPADData *buffer) {
    // buffer->angle stores the rotations per axis since the app started.
    // Each full rotation add/subtracts 1.0f (depending on the direction).

    // Check for rotation every only 5 frames.
    angleX_frameCounter++;
    if(angleX_frameCounter >= 5) {
        // Get how much the gamepad rotated within the last 5 frames.
        float diff_angle = -(buffer->angle.x - angleX_last);
        // We want the gamepad to make (on average) at least 0.16% (1/6) of a full rotation per 5 frames (for 6 times in a row).
        float target_diff = (0.16f);
        // Calculate if rotated enough in this step (including the delta from the last step).
        float total_diff = (diff_angle + angleX_delta) - target_diff;
        if(total_diff > 0.0f) {
            // The rotation in this step was enough.
            angleX_counter++;
            // When the gamepad rotated ~0.16% for 6 times in a row we made a full rotation!
            if(angleX_counter > 5) {
                ConfigUtils::openConfigMenu();
                // reset stuff.
                angleX_counter = 0;
                angleX_delta = 0.0f;
            } else {
                // Save difference as it will be added on the next check.
                angleX_delta = total_diff;
            }
        } else {
            // reset counter if it stopped rotating.
            angleX_counter = 0;
        }
        angleX_frameCounter = 0;
        angleX_last = buffer->angle.x;
    }

}

DECL(int32_t, VPADRead, int32_t chan, VPADData *buffer, uint32_t buffer_size, int32_t *error) {
    int32_t result = real_VPADRead(chan, buffer, buffer_size, error);

    if(result > 0 && (buffer[0].btns_h == (VPAD_BUTTON_PLUS | VPAD_BUTTON_R | VPAD_BUTTON_L)) && vpadPressCooldown == 0 && OSIsHomeButtonMenuEnabled()) {
        if(MemoryMapping::isMemoryMapped()) {
            MemoryMapping::readTestValuesFromMemory();
        } else {
            DEBUG_FUNCTION_LINE("Memory was not mapped. To test the memory please exit the plugin loader by pressing MINUS\n");
        }
        vpadPressCooldown = 0x3C;
    }

    if(result > 0 && (buffer[0].btns_h == (VPAD_BUTTON_L | VPAD_BUTTON_DOWN | VPAD_BUTTON_MINUS)) && vpadPressCooldown == 0 && OSIsHomeButtonMenuEnabled()) {
        ConfigUtils::openConfigMenu();
        vpadPressCooldown = 0x3C;
    } else if(result > 0 && OSIsHomeButtonMenuEnabled()) {
        checkMagic(buffer);
    }

    if(vpadPressCooldown > 0) {
        vpadPressCooldown--;
    }
    return result;
}


hooks_magic_t method_hooks_hooks[] __attribute__((section(".data"))) = {
    MAKE_MAGIC(__PPCExit,               LIB_CORE_INIT,  STATIC_FUNCTION),
    MAKE_MAGIC(ProcUIProcessMessages,   LIB_PROC_UI,    DYNAMIC_FUNCTION),
    MAKE_MAGIC(GX2SetTVBuffer,          LIB_GX2,        STATIC_FUNCTION),
    MAKE_MAGIC(GX2SetDRCBuffer,         LIB_GX2,        STATIC_FUNCTION),
    MAKE_MAGIC(GX2WaitForVsync,         LIB_GX2,        STATIC_FUNCTION),
    MAKE_MAGIC(VPADRead,                LIB_VPAD,       STATIC_FUNCTION),
};


uint32_t method_hooks_size_hooks __attribute__((section(".data"))) = sizeof(method_hooks_hooks) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile uint32_t method_calls_hooks[sizeof(method_hooks_hooks) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));

