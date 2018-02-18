#include <wups.h>
#include <string.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "utils/logger.h"

WUPS_PLUGIN_NAME("Padcon");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");


INITIALIZE(args){
   InitOSFunctionPointers();
   InitSocketFunctionPointers();
   InitVPadFunctionPointers();
   
   log_init();

   log_print("Init of padcon!\n");
} 

// Both would be possible.
//WUPS_HOOK_INIT(init);

DECL_FUNCTION(int, VPADRead, int chan, VPADData *buffer, u32 buffer_size, s32 *error) {
    int result = real_VPADRead(chan, buffer, buffer_size, error);
     if(buffer->btns_r&VPAD_BUTTON_STICK_R) {
            int mode;
            VPADGetLcdMode(0, (s32*)&mode);       // Get current display mode
            if(mode != 1) {
                VPADSetLcdMode(0, 1);       // Turn it off
            }
            else {
                VPADSetLcdMode(0, 0xFF);    // Turn it on
            }
    }
    return result;
}

WUPS_MUST_REPLACE(VPADRead ,WUPS_LOADER_LIBRARY_VPAD, VPADRead);
