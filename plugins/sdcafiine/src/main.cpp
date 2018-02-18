#include <wups.h>
#include <string.h>
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <dynamic_libs/sys_functions.h>
#include <dynamic_libs/fs_functions.h>
#include <dynamic_libs/proc_ui_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <utils/logger.h>
#include "common/retain_vars.h"
#include <fswrapper/fs_retain_vars.h>
#include <fswrapper/FileReplacerUtils.h>
#include "common/common.h"
#include "main.h"
#include "modpackSelector.h"

WUPS_PLUGIN_NAME("SDCaffiine lite");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

INITIALIZE(args){
    WUPS_InitFS(args);
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging
    InitVPadFunctionPointers(); //For logging

    log_init();

    if(FileReplacerUtils::setGroupAndOwnerID()){
        DEBUG_FUNCTION_LINE("SUCCESS\n");
    }

    if(args != NULL){
        gSDInitDone = args->device_mounted;
        /*if((args->device_mounted & WUPS_SD_MOUNTED) > 0){
            DEBUG_FUNCTION_LINE("Yay, SD was mounted!\n");
            gSDInitDone = WUPS_SD_MOUNTED;

            DEBUG_FUNCTION_LINE("opendir result %08X!\n",opendir("sd:/"));

        } else {
            DEBUG_FUNCTION_LINE("SD wasn't mounted...!\n");
        }*/
    }else {
        DEBUG_FUNCTION_LINE("args were NULL!\n");
    }

    HandleMultiModPacks(OSGetTitleID());

    log_print("Init of SDCafiine!\n");
}

void deInitApplication(){
    //FileReplacerUtils::getInstance()->StopAsyncThread();
    FileReplacerUtils::destroyInstance();
}
