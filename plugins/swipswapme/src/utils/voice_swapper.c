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

#include "voice_info.h"
#include "voice_swapper.h"

void VoiceSwapper_acquireVoice(void * voice){
    for(int i = 0;i<VOICE_INFO_MAX;i++){
        if(gVoiceInfos[i].voice == NULL){
            if(VOICE_SWAP_LOG == 1){log_printf("[VoiceSwapper] acquireVoice in slot %d for %08X!\n",i,voice);}
            gVoiceInfos[i].voice = voice;
            break;
        }
    }
}

void VoiceSwapper_freeVoice(void * voice){
    for(int i = 0;i<VOICE_INFO_MAX;i++){
        if(gVoiceInfos[i].voice == voice){
            if(VOICE_SWAP_LOG == 1){log_printf("[VoiceSwapper] freeVoice in slot %d for %08X!\n",i,voice);}
            gVoiceInfos[i].voice = NULL;
            break;
        }
    }
}

void VoiceSwapper_setMix(void * voice,u32 device, void* mix){
   for(int i = 0;i<VOICE_INFO_MAX;i++){
        if(gVoiceInfos[i].voice == voice){
            if(VOICE_SWAP_LOG == 1){log_printf("[VoiceSwapper] setMix in slot %d for %08X!\n",i,voice);}
            if(device == 0){
                memcpy(gVoiceInfos[i].mixTV,mix,sizeof(gVoiceInfos[i].mixTV));
            }else if(device == 1){
                memcpy(gVoiceInfos[i].mixDRC,mix,sizeof(gVoiceInfos[i].mixDRC));
            }
            break;
        }
    }
}

void VoiceSwapper_swapAll(){
    for(int i = 0;i<VOICE_INFO_MAX;i++){
        if(gVoiceInfos[i].voice == NULL) continue;
        if(VOICE_SWAP_LOG == 1){log_printf("[VoiceSwapper] swapping slot %d, voice %08X!\n",i,gVoiceInfos[i].voice);}
        u32 buffer[24];
        memcpy(buffer,gVoiceInfos[i].mixTV,sizeof(gVoiceInfos[i].mixTV));
        memcpy(gVoiceInfos[i].mixTV,gVoiceInfos[i].mixDRC,sizeof(gVoiceInfos[i].mixTV));
        memcpy(gVoiceInfos[i].mixDRC,buffer,sizeof(gVoiceInfos[i].mixTV));
    }
}
