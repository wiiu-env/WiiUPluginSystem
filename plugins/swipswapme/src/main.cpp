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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <wups.h>

#include <vector>
#include <string>

#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/gx2_functions.h>
#include <dynamic_libs/gx2_types.h>
#include <dynamic_libs/syshid_functions.h>
#include <dynamic_libs/ax_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <dynamic_libs/padscore_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <dynamic_libs/sys_functions.h>
#include <dynamic_libs/proc_ui_functions.h>
#include <utils/logger.h>
#include <common/c_retain_vars.h>

WUPS_PLUGIN_NAME("SwipSwapMe");
WUPS_PLUGIN_DESCRIPTION("Swaps the gamepad and tv screen when pressing a certain button (TV is default)");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");


u32 SplashScreen(s32 time,s32 combotime);

/* Entry point */
ON_APPLICATION_START(args){
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging

    InitSysFunctionPointers(); // For SYSLaunchMenu()

    //For patching
    InitVPadFunctionPointers();
    InitPadScoreFunctionPointers();
    InitAXFunctionPointers();

    memset(gVoiceInfos,0,sizeof(gVoiceInfos));
}

ON_APP_STATUS_CHANGED(status) {
	gAppStatus = status;
}

INITIALIZE_PLUGIN(){
	InitOSFunctionPointers();
	InitVPadFunctionPointers();
	u32 res = SplashScreen(10,2);
	gButtonCombo = res;
}

#define FPS 60
u32 SplashScreen(s32 time,s32 combotime){
    u32 result = VPAD_BUTTON_TV;
    
    // Init screen
    OSScreenInit();
       
    u32 screen_buf0_size = OSScreenGetBufferSizeEx(0);
    u32 screen_buf1_size = OSScreenGetBufferSizeEx(1);
    
    u32 * screenbuffer0 = (u32*)memalign(0x100, screen_buf0_size);
    u32 * screenbuffer1 = (u32*)memalign(0x100, screen_buf1_size);
    
    if(screenbuffer0 == NULL || screenbuffer1 == NULL){
        if(screenbuffer0 != NULL){ free(screenbuffer0); }
        if(screenbuffer1 != NULL){ free(screenbuffer1); }
        return result;
    }    
 
    OSScreenSetBufferEx(0, (void *)screenbuffer0);
    OSScreenSetBufferEx(1, (void *)screenbuffer1);

    OSScreenEnableEx(0, 1);
    OSScreenEnableEx(1, 1);

    // Clear screens
    OSScreenClearBufferEx(0, 0);
    OSScreenClearBufferEx(1, 0);

    // Flip buffers
    OSScreenFlipBuffersEx(0);
    OSScreenFlipBuffersEx(1);
    
    OSScreenClearBufferEx(0, 0);
    OSScreenClearBufferEx(1, 0);

	std::vector<std::string> strings;
    strings.push_back("SwipSwapMe 0.2 - by Maschell.");
    strings.push_back("");
    strings.push_back("");
    strings.push_back("Press the combo you want to use for swapping now for 2 seconds.");
    strings.push_back("Pressing the TV button will return directly.");
    strings.push_back("");
    strings.push_back("Otherwise the default combo (TV button) will be used in 10 seconds.");
    u8 pos = 0;
    for (std::vector<std::string>::iterator it = strings.begin() ; it != strings.end(); ++it){
        OSScreenPutFontEx(0, 0, pos, (*it).c_str());
        OSScreenPutFontEx(1, 0, pos, (*it).c_str());
        pos++;
	}

    OSScreenFlipBuffersEx(0);
    OSScreenFlipBuffersEx(1);

    s32 tickswait = time * FPS * 16;

    s32 sleepingtime = 16;
    s32 times = tickswait/16;
    s32 i=0;

    VPADData vpad_data;
    s32 error;
    u32 last = 0xFFFFFFFF;
    s32 timer = 0;
    while(i<times){
        VPADRead(0, &vpad_data, 1, &error);
        if(vpad_data.btns_h == VPAD_BUTTON_TV) break;
        if(last == vpad_data.btns_h && last != 0){
            timer++;
        }else{
            last = vpad_data.btns_h;
            timer = 0;
        }
        if(timer >= combotime*FPS){
            result = vpad_data.btns_h;
            break;
        }
        i++;
        os_usleep(sleepingtime*1000);
    }
    
    if(screenbuffer0 != NULL){ free(screenbuffer0); }
    if(screenbuffer1 != NULL){ free(screenbuffer1); }
        
    return result;
}
