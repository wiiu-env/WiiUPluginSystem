#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>

#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <dynamic_libs/fs_functions.h>
#include <utils/logger.h>
#include <fs/FSUtils.h>
#include <fs/sd_fat_devoptab.h>
#include <system/exception_handler.h>
#include "common/common.h"

#include "main.h"
#include "utils.h"

/* Entry point */
extern "C" int Menu_Main(int argc, char **argv){
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging
    InitFSFunctionPointers();

    log_init();

    setup_os_exceptions();

    DEBUG_FUNCTION_LINE("Mount SD partition\n");

    int res = 0;
    if((res = mount_sd_fat("sd")) >= 0){
        DEBUG_FUNCTION_LINE("Mounting successful\n");
        loadAndProcessElf("sd:/example_plugin.mod");
    }

    DEBUG_FUNCTION_LINE("Application is ending now.\n");

    return EXIT_SUCCESS;
}

void loadAndProcessElf(const char * elfPath){
    u8 * elfData = NULL;
    u32 elfDataSize = 0;
    DEBUG_FUNCTION_LINE("Reading elf from path: %s\n",elfPath);

    int result = FSUtils::LoadFileToMem(elfPath, &elfData, &elfDataSize);
    if(result < 0){
        DEBUG_FUNCTION_LINE("Loading failed. Make sure the file is on the SDCard.\n");
        return;
    }

    DEBUG_FUNCTION_LINE("Loaded file. Position: %08X Size: %d\n",elfData,elfDataSize);

    u32 dump_size = 0x80;
    if(elfDataSize >= dump_size){
        DEBUG_FUNCTION_LINE("Hexdump of the first %08X bytes: \n",dump_size);
        dumpHex(elfData,dump_size);
    }

}
