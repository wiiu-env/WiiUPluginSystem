#include <wups.h>
#include <utils/logger.h>
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/os_types.h>
#include <dynamic_libs/vpad_functions.h>
#include <dynamic_libs/gx2_functions.h>
#include <utils/StringTools.h>
#include <fs/FSUtils.h>
#include "common.h"
#include "function_patcher.h"
#include "screenshot_utils.h"
#include "retain_vars.hpp"

static bool takeScreenshotTV __attribute__((section(".data"))) = false;
static bool takeScreenshotDRC __attribute__((section(".data"))) = false;
static u8 screenshotCooldown __attribute__((section(".data"))) = 0;
static u32 counter __attribute__((section(".data"))) = 0;

DECL_FUNCTION(int, VPADRead, int chan, VPADData *buffer, u32 buffer_size, s32 *error) {
    int result = real_VPADRead(chan, buffer, buffer_size, error);

    if(result > 0 && (buffer[0].btns_h == gButtonCombo) && screenshotCooldown == 0 && OSIsHomeButtonMenuEnabled()) {
        counter++;
        takeScreenshotTV = true;
        takeScreenshotDRC = true;

        screenshotCooldown = 0x3C;
    }
    if(screenshotCooldown > 0) {
        screenshotCooldown--;
    }

    return result;
}

DECL_FUNCTION(void, GX2CopyColorBufferToScanBuffer, const GX2ColorBuffer *colorBuffer, s32 scan_target) {
    if((takeScreenshotTV || takeScreenshotDRC) && gAppStatus == WUPS_APP_STATUS_FOREGROUND) {
        OSCalendarTime output;
        OSTicksToCalendarTime(OSGetTime(), &output);
        char buffer[255] = {0};
        snprintf(buffer,254,"%s%04i-%02i-%02i/",WIIU_SCREENSHOT_PATH,output.year,output.mon,output.mday);

        FSUtils::CreateSubfolder(buffer);

        snprintf(buffer,254,"%s%04i-%02i-%02i/%04i-%02i-%02i_%02i.%02i.%02i_",
                 WIIU_SCREENSHOT_PATH,output.year,output.mon,output.mday,output.year,output.mon,output.mday,output.hour,output.min,output.sec);

        if(scan_target == 1 && colorBuffer != NULL && takeScreenshotTV && gAppStatus == WUPS_APP_STATUS_FOREGROUND) {
            DEBUG_FUNCTION_LINE("Lets take a screenshot from TV. Source format: %d \n",colorBuffer->surface.format);
            takeScreenshot((GX2ColorBuffer *)colorBuffer, StringTools::strfmt("%sTV.jpg",buffer).c_str());
            takeScreenshotTV = false;
        }
        if(scan_target == 4 && colorBuffer != NULL && takeScreenshotDRC && gAppStatus == WUPS_APP_STATUS_FOREGROUND) {
            DEBUG_FUNCTION_LINE("Lets take a screenshot from DRC. Source format: %d \n",colorBuffer->surface.format);
            takeScreenshot((GX2ColorBuffer *)colorBuffer, StringTools::strfmt("%sDRC.jpg",buffer).c_str());
            takeScreenshotDRC = false;
        }
    }
    real_GX2CopyColorBufferToScanBuffer(colorBuffer,scan_target);
}

WUPS_MUST_REPLACE(VPADRead,                         WUPS_LOADER_LIBRARY_VPAD,   VPADRead);
WUPS_MUST_REPLACE(GX2CopyColorBufferToScanBuffer,   WUPS_LOADER_LIBRARY_GX2,    GX2CopyColorBufferToScanBuffer);
