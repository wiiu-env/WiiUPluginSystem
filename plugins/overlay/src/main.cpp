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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <wups.h>
#include <stdio.h>

#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <dynamic_libs/fs_functions.h>
#include <dynamic_libs/gx2_functions.h>
#include <dynamic_libs/gx2_types.h>
#include <dynamic_libs/socket_functions.h>
#include <utils/logger.h>
#include <utils/StringTools.h>
#include "main.h"

WUPS_PLUGIN_NAME("Overlay test");
WUPS_PLUGIN_DESCRIPTION("Just an overlay test. Press A,B,X and Y and a text should appear.");
WUPS_PLUGIN_VERSION("v0.1");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

/* Entry point */
INITIALIZE(args){
    WUPS_InitFS(args);
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging
    InitVPadFunctionPointers(); 
    InitFSFunctionPointers(); 

    InitGX2FunctionPointers();
    
    log_init();
}

void printTextOnScreen(int x,int y, const char * msg){
    OSScreenPutFontEx(0, x, y, msg);
    OSScreenPutFontEx(1, x, y, msg);
}

#define FPS 60
u32 SplashScreen(s32 time){
    u32 result = 0;

    OSScreenInit();
    u32 screen_buf0_size = OSScreenGetBufferSizeEx(0);
    u32 screen_buf1_size = OSScreenGetBufferSizeEx(1);
    u32 * screenbuffer0 = (u32*)memalign(0x100, screen_buf0_size);
    u32 * screenbuffer1 = (u32*)memalign(0x100, screen_buf1_size);
    if(screenbuffer0 == NULL){
        return 0;
    }
    if(screenbuffer1 == NULL){
        free(screenbuffer0);
        return 0;
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

    s32 tickswait = time * FPS * 16;

    s32 sleepingtime = 16;
    s32 times = tickswait/16;
    s32 i=0;

    while(i<times){
        OSScreenClearBufferEx(0, 0);
        OSScreenClearBufferEx(1, 0);
        
        printTextOnScreen(0,0,"This could be something cool.");
        
        printTextOnScreen(0,5,StringTools::strfmt("Testing changing text: %d",i).c_str());

        // Flip buffers
        OSScreenFlipBuffersEx(0);
        OSScreenFlipBuffersEx(1);

        i++;
        os_usleep(sleepingtime*1000);
    }
    
    free(screenbuffer0);
    free(screenbuffer1);
    return result;
}

