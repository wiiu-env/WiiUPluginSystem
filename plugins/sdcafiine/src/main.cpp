#include <wups.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
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

WUPS_MODULE_NAME("SDCaffiine lite");
WUPS_MODULE_VERSION("v1.0");
WUPS_MODULE_AUTHOR("Maschell");
WUPS_MODULE_LICENSE("GPL");

INITIALIZE(){
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging

    log_init();

    if(FileReplacerUtils::setGroupAndOwnerID()){
        DEBUG_FUNCTION_LINE("SUCCESS\n");
    }

    HandleMultiModPacks(OSGetTitleID());

    log_print("Init of SDCafiine!\n");
}

void deInitApplication(){
    //FileReplacerUtils::getInstance()->StopAsyncThread();
    FileReplacerUtils::destroyInstance();
}

