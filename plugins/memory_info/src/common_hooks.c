#include <wups.h>
#include <string.h>
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <utils/logger.h>
#include "main.h"

DECL_FUNCTION(void, __PPCExit, void) {
    // We reset them at ende, in case at application even uses the functions
    // BEFORE with have our init hook.
    memset(expHeapPointer,0,sizeof(expHeapPointer));
    memset(unitHeapPointer,0,sizeof(unitHeapPointer));
    memset(frmHeapPointer,0,sizeof(frmHeapPointer));
    memset(baseHeaphandles,0,sizeof(baseHeaphandles));
    memset(userHeapInfos,0,sizeof(userHeapInfos));
    real___PPCExit();
}

DECL_FUNCTION(int, VPADRead, int chan, VPADData *buffer, u32 buffer_size, s32 *error) {
    int result = real_VPADRead(chan, buffer, buffer_size, error);

    int btns = VPAD_BUTTON_R | VPAD_BUTTON_L | VPAD_BUTTON_A | VPAD_BUTTON_B | VPAD_BUTTON_X | VPAD_BUTTON_Y;
    if(result > 0 && ((buffer[0].btns_h & (btns)) == btns) && gButtonComboCooldown == 0 && OSIsHomeButtonMenuEnabled()) {
        print_memory_info();
        gButtonComboCooldown = 0x3C;
    }
    if(gButtonComboCooldown > 0) {
        gButtonComboCooldown--;
    }
    return result;
}

WUPS_MUST_REPLACE(__PPCExit,    WUPS_LOADER_LIBRARY_COREINIT,   __PPCExit);
WUPS_MUST_REPLACE(VPADRead,     WUPS_LOADER_LIBRARY_VPAD,       VPADRead);
