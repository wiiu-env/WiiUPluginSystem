/****************************************************************************
 * Copyright (C) 2017,2018 Maschell
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
#include "utils/logger.h"
#include "utils/voice_swapper.h"
#include "common/c_retain_vars.h"
#include "dynamic_libs/proc_ui_functions.h"
#include "dynamic_libs/vpad_functions.h"

DECL_FUNCTION(s32, AXSetVoiceDeviceMixOld, void *v, s32 device, u32 id, void *mix){
    if(gSwap){ device = !device;}
    if(VOICE_SWAP_LOG == 1){log_printf("AXSetVoiceDeviceMixOld voice: %08X device: %d, mix: %08X\n",v,device,mix);}
    VoiceSwapper_setMix(v,device,mix);
    return real_AXSetVoiceDeviceMixOld(v,device,id,mix);
}

DECL_FUNCTION(s32, AXSetVoiceDeviceMix, void *v, s32 device, u32 id, void *mix){
    if(gSwap){ device = !device;}
    if(VOICE_SWAP_LOG == 1){log_printf("AXSetVoiceDeviceMix voice: %08X device: %d, mix: %08X\n",v,device,mix);}
    VoiceSwapper_setMix(v,device,mix);
    return real_AXSetVoiceDeviceMix(v,device,id,mix);
}

DECL_FUNCTION(void *, AXAcquireVoiceExOld, u32 prio, void * callback, u32 arg){
    void * result = real_AXAcquireVoiceExOld(prio,callback,arg);
    if(VOICE_SWAP_LOG == 1){log_printf("AXAcquireVoiceExOld result: %08X \n",result);}
    VoiceSwapper_acquireVoice(result);
    return result;
}

DECL_FUNCTION(void *, AXAcquireVoiceEx, u32 prio, void * callback, u32 arg){
    void * result = real_AXAcquireVoiceEx(prio,callback,arg);
    if(VOICE_SWAP_LOG == 1){log_printf("AXAcquireVoiceEx result: %08X \n",result);}
    VoiceSwapper_acquireVoice(result);
    return result;
}

DECL_FUNCTION(void, AXFreeVoiceOld, void *v){
    if(VOICE_SWAP_LOG == 1){log_printf("AXFreeVoiceOld v: %08X \n",v);}
    VoiceSwapper_freeVoice(v);
    real_AXFreeVoiceOld(v);
}

DECL_FUNCTION(void, AXFreeVoice, void *v){
    if(VOICE_SWAP_LOG == 1){log_printf("AXFreeVoice v: %08X \n",v);}
    VoiceSwapper_freeVoice(v);
    real_AXFreeVoice(v);
}

DECL_FUNCTION(void, GX2CopyColorBufferToScanBuffer, GX2ColorBuffer *colorBuffer, s32 scan_target){
    if(gSwap){
        if(scan_target == 1){
            scan_target = 4;
        }else{
            scan_target = 1;
        }
    }
    real_GX2CopyColorBufferToScanBuffer(colorBuffer,scan_target);
}

/*
DECL(s32, AXSetDefaultMixerSelectOld, u32 s){
    s32 result = real_AXSetDefaultMixerSelectOld(s);
    return result;
}*/


void swapVoices(){
    VoiceSwapper_swapAll();
    for(int i = 0;i<VOICE_INFO_MAX;i++){
        if(gVoiceInfos[i].voice == NULL) continue;

        real_AXSetVoiceDeviceMix(gVoiceInfos[i].voice,0,0,gVoiceInfos[i].mixTV);
        real_AXSetVoiceDeviceMix(gVoiceInfos[i].voice,1,0,gVoiceInfos[i].mixDRC);
        real_AXSetVoiceDeviceMixOld(gVoiceInfos[i].voice,0,0,gVoiceInfos[i].mixTV);
        real_AXSetVoiceDeviceMixOld(gVoiceInfos[i].voice,1,0,gVoiceInfos[i].mixDRC);
    }
}

DECL_FUNCTION(int, VPADRead, int chan, VPADData *buffer, u32 buffer_size, s32 *error) {
    int result = real_VPADRead(chan, buffer, buffer_size, error);

    if(result > 0 && ((buffer[0].btns_h & gButtonCombo) == gButtonCombo) && gCallbackCooldown == 0 ){
        gCallbackCooldown = 0x3C;
        gSwap = !gSwap;
        if(!gAppStatus){
            swapVoices();
        }
    }
    if(gCallbackCooldown > 0) gCallbackCooldown--;

    return result;
}

DECL_FUNCTION(u32, ProcUIProcessMessages, u32 u){
    u32 res = real_ProcUIProcessMessages(u);
    if(res != gAppStatus){
        log_printf("App status changed from %d to %d \n",gAppStatus,res);
        gAppStatus = res;
    }

    return res;
}

WUPS_MUST_REPLACE(ProcUIProcessMessages,            WUPS_LOADER_LIBRARY_PROC_UI,    ProcUIProcessMessages);
WUPS_MUST_REPLACE(GX2CopyColorBufferToScanBuffer,   WUPS_LOADER_LIBRARY_GX2,        GX2CopyColorBufferToScanBuffer);
WUPS_MUST_REPLACE(VPADRead,                         WUPS_LOADER_LIBRARY_VPAD,       VPADRead);
WUPS_MUST_REPLACE(AXAcquireVoiceExOld,              WUPS_LOADER_LIBRARY_SND_CORE,   AXAcquireVoiceEx);
WUPS_MUST_REPLACE(AXFreeVoiceOld,                   WUPS_LOADER_LIBRARY_SND_CORE,   AXFreeVoice);
WUPS_MUST_REPLACE(AXSetVoiceDeviceMixOld,           WUPS_LOADER_LIBRARY_SND_CORE,   AXSetVoiceDeviceMix);
//WUPS_MUST_REPLACE(AXSetDefaultMixerSelectOld, ,   WUPS_LOADER_LIBRARY_SND_CORE,     AXSetDefaultMixerSelect),
WUPS_MUST_REPLACE(AXAcquireVoiceEx,                 WUPS_LOADER_LIBRARY_SNDCORE2,   AXAcquireVoiceEx);
WUPS_MUST_REPLACE(AXFreeVoice,                      WUPS_LOADER_LIBRARY_SNDCORE2,   AXFreeVoice);
WUPS_MUST_REPLACE(AXSetVoiceDeviceMix,              WUPS_LOADER_LIBRARY_SNDCORE2,   AXSetVoiceDeviceMix);
