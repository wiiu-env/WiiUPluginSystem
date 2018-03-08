#include <wups.h>
#include <malloc.h>
#include <vector>
#include <string>
#include <string.h>
#include <dirent.h>
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <dynamic_libs/fs_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <dynamic_libs/gx2_functions.h>
#include <utils/logger.h>
#include "retain_vars.hpp"

// Mandatory plugin information.
WUPS_PLUGIN_NAME("Screenshot tool");
WUPS_PLUGIN_DESCRIPTION("This plugin allows you to make screenshots that will be saved to the sd card");
WUPS_PLUGIN_VERSION("v0.1");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

// FS Access
WUPS_FS_ACCESS()

u32 SplashScreen(s32 time,s32 combotime);

// Gets called once the loader exists.
INITIALIZE_PLUGIN() {
    InitOSFunctionPointers();
    InitSocketFunctionPointers();
    InitVPadFunctionPointers();
    InitGX2FunctionPointers();

    log_init();

    InitOSFunctionPointers();
    InitVPadFunctionPointers();
    u32 res = SplashScreen(10,2);
    gButtonCombo = res;

}

// Called whenever an application was started.
ON_APPLICATION_START(my_args) {
    InitOSFunctionPointers();
    InitSocketFunctionPointers();
    InitFSFunctionPointers();

    log_init();
}

ON_APP_STATUS_CHANGED(status) {
    gAppStatus = status;
}


#define FPS 60
u32 SplashScreen(s32 time,s32 combotime) {
    u32 result = VPAD_BUTTON_R | VPAD_BUTTON_L | VPAD_BUTTON_ZR | VPAD_BUTTON_ZL;

    // Init screen
    OSScreenInit();

    u32 screen_buf0_size = OSScreenGetBufferSizeEx(0);
    u32 screen_buf1_size = OSScreenGetBufferSizeEx(1);

    u32 * screenbuffer0 = (u32*)memalign(0x100, screen_buf0_size);
    u32 * screenbuffer1 = (u32*)memalign(0x100, screen_buf1_size);

    if(screenbuffer0 == NULL || screenbuffer1 == NULL) {
        if(screenbuffer0 != NULL) {
            free(screenbuffer0);
        }
        if(screenbuffer1 != NULL) {
            free(screenbuffer1);
        }
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
    strings.push_back("Screenshot tool 0.1 - by Maschell.");
    strings.push_back("");
    strings.push_back("");
    strings.push_back("Press the combo you want to use for making screenshots now");
    strings.push_back("for 2 seconds.");
    strings.push_back(" ");
    strings.push_back("Otherwise the default combo (L+R+ZR+ZL button) will be used");
    strings.push_back("in 10 seconds.");
    strings.push_back(" ");
    strings.push_back("Press the TV buttons to exit with the default combo.");

    u8 pos = 0;
    for (std::vector<std::string>::iterator it = strings.begin() ; it != strings.end(); ++it) {
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
    while(i<times) {
        VPADRead(0, &vpad_data, 1, &error);
        if(vpad_data.btns_h == VPAD_BUTTON_TV) {
            break;
        }
        if(last == vpad_data.btns_h && last != 0) {
            timer++;
        } else {
            last = vpad_data.btns_h;
            timer = 0;
        }
        if(timer >= combotime*FPS) {
            result = vpad_data.btns_h;
            break;
        }
        i++;
        os_usleep(sleepingtime*1000);
    }

    if(screenbuffer0 != NULL) {
        free(screenbuffer0);
    }
    if(screenbuffer1 != NULL) {
        free(screenbuffer1);
    }

    return result;
}
