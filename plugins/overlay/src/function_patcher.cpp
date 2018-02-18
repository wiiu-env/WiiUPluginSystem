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
#include <wups.h>
#include "main.h"
#include <utils/logger.h>
#include <dynamic_libs/vpad_functions.h>
#include <dynamic_libs/os_functions.h>

u8 logVSync __attribute__((section(".data"))) = 0;
u8 gCallbackCooldown __attribute__((section(".data"))) = 0;

struct buffer_store{
    void * buffer;
    s32 buffer_size;
    s32 mode;
    s32 surface_format;
    vs32 buffering_mode;
};

struct buffer_store drc_store __attribute__((section(".data")));
struct buffer_store tv_store __attribute__((section(".data")));


DECL_FUNCTION(void, GX2SetTVBuffer, void *buffer, u32 buffer_size, s32 tv_render_mode, s32 format, s32 buffering_mode){
    tv_store.buffer = buffer;
    tv_store.buffer_size = buffer_size;
    tv_store.mode = tv_render_mode;
    tv_store.surface_format = format;
    tv_store.buffering_mode = buffering_mode;
    
    return real_GX2SetTVBuffer(buffer,buffer_size,tv_render_mode,format,buffering_mode);
}

DECL_FUNCTION(void, GX2SetDRCBuffer, void *buffer, u32 buffer_size, s32 drc_mode, s32 surface_format, s32 buffering_mode){
    drc_store.buffer = buffer;
    drc_store.buffer_size = buffer_size;
    drc_store.mode = drc_mode;
    drc_store.surface_format = surface_format;
    drc_store.buffering_mode = buffering_mode;
    
    return real_GX2SetDRCBuffer(buffer,buffer_size,drc_mode,surface_format,buffering_mode);
}

DECL_FUNCTION(int, VPADRead, int chan, VPADData *buffer, u32 buffer_size, s32 *error) {
    int result = real_VPADRead(chan, buffer, buffer_size, error);
    int btns = VPAD_BUTTON_Y | VPAD_BUTTON_X | VPAD_BUTTON_A | VPAD_BUTTON_B;
    if(result > 0 && ((buffer[0].btns_h & (btns)) == btns) && gCallbackCooldown == 0 ){
        gCallbackCooldown = 0x3C;
        logVSync = 1;
        DCFlushRange(&logVSync,sizeof(logVSync));
        DCInvalidateRange(&logVSync,sizeof(logVSync));
    }
    if(gCallbackCooldown > 0){
        gCallbackCooldown--;
    }

    return result;
}

DECL_FUNCTION(void, GX2WaitForVsync, void){
    real_GX2WaitForVsync();
    if(logVSync){
        if(OSIsHomeButtonMenuEnabled()){
            SplashScreen(2);
            
            // Restore the original buffer.
            real_GX2SetTVBuffer(tv_store.buffer,tv_store.buffer_size,tv_store.mode,tv_store.surface_format,tv_store.buffering_mode);
            real_GX2SetDRCBuffer(drc_store.buffer,drc_store.buffer_size,drc_store.mode,drc_store.surface_format,drc_store.buffering_mode);
        }
        logVSync = 0;
    }
}

WUPS_MUST_REPLACE(GX2SetTVBuffer,    WUPS_LOADER_LIBRARY_GX2,        GX2SetTVBuffer);
WUPS_MUST_REPLACE(GX2SetDRCBuffer,   WUPS_LOADER_LIBRARY_GX2,        GX2SetDRCBuffer);
WUPS_MUST_REPLACE(GX2WaitForVsync,   WUPS_LOADER_LIBRARY_GX2,        GX2WaitForVsync);
WUPS_MUST_REPLACE(VPADRead,          WUPS_LOADER_LIBRARY_VPAD,       VPADRead);