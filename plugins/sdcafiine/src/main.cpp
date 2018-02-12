#include <wups.h>
#include <string.h>
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <dynamic_libs/sys_functions.h>
#include <dynamic_libs/fs_functions.h>
#include <dynamic_libs/proc_ui_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <fs/sd_fat_devoptab.h>
#include <utils/logger.h>
#include "common/retain_vars.h"
#include <iosuhax.h>
#include <fswrapper/fs_retain_vars.h>
#include <fswrapper/FileReplacerUtils.h>
#include "common/common.h"
#include "main.h"
#include "modpackSelector.h"

#define DEBUG_LOG  1

WUPS_MODULE_NAME("SDCaffiine lite");
WUPS_MODULE_VERSION("v1.0");
WUPS_MODULE_AUTHOR("Maschell");
WUPS_MODULE_LICENSE("GPL");

void Init_SD_USB();

INITIALIZE(){
    if(gAppStatus == 2){
        return;
    }
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging

    InitSysFunctionPointers(); // For SYSLaunchMenu()
    InitProcUIFunctionPointers(); // For SYSLaunchMenu()
    InitFSFunctionPointers();
    InitVPadFunctionPointers();

    log_init();

    gSDInitDone = 0;

    DEBUG_FUNCTION_LINE("Mount SD partition\n");
    Init_SD_USB();

    if(FileReplacerUtils::setGroupAndOwnerID()){
        DEBUG_FUNCTION_LINE("SUCCESS\n");
    }

    HandleMultiModPacks(OSGetTitleID());

    log_print("Init of sd_cafiine!\n");
}


void Init_SD_USB() {
    //int res = IOSUHAX_Open(NULL);
    //if(res < 0){
        //ExecuteIOSExploitWithDefaultConfig();
    //    return;
    //}
    deleteDevTabsNames();
    mount_fake();
    gSDInitDone |= SDUSB_MOUNTED_FAKE;

    int res = -1;
    if(res < 0){
        DEBUG_FUNCTION_LINE("IOSUHAX_open failed\n");
        if((res = mount_sd_fat("sd")) >= 0){
            DEBUG_FUNCTION_LINE("mount_sd_fat success\n");
            gSDInitDone |= SDUSB_MOUNTED_OS_SD;
        }else{
            DEBUG_FUNCTION_LINE("mount_sd_fat failed %d\n",res);
        }
    }else{
        DEBUG_FUNCTION_LINE("Using IOSUHAX for SD/USB access\n");
        gSDInitDone |= SDUSB_LIBIOSU_LOADED;
        int ntfs_mounts = 0; //mountAllNTFS();
        if(ntfs_mounts > 0){
            gSDInitDone |= USB_MOUNTED_LIBNTFS;
        }

        /*if(mount_libfatAll() == 0){
            gSDInitDone |= SD_MOUNTED_LIBFAT;
            gSDInitDone |= USB_MOUNTED_LIBFAT;
        }*/
    }
    DEBUG_FUNCTION_LINE("%08X\n",gSDInitDone);
}


void deInit_SD_USB(){
    DEBUG_FUNCTION_LINE("Called this function.\n");

    if(gSDInitDone & SDUSB_MOUNTED_FAKE){
       DEBUG_FUNCTION_LINE("Unmounting fake\n");
       unmount_fake();
       gSDInitDone &= ~SDUSB_MOUNTED_FAKE;
    }
    if(gSDInitDone & SDUSB_MOUNTED_OS_SD){
        DEBUG_FUNCTION_LINE("Unmounting OS SD\n");
        unmount_sd_fat("sd");
        gSDInitDone &= ~SDUSB_MOUNTED_OS_SD;
    }

    /*if(gSDInitDone & SD_MOUNTED_LIBFAT){
        DEBUG_FUNCTION_LINE("Unmounting LIBFAT SD\n");
        unmount_libfat("sd");
        gSDInitDone &= ~SD_MOUNTED_LIBFAT;
    }

    if(gSDInitDone & USB_MOUNTED_LIBFAT){
        DEBUG_FUNCTION_LINE("Unmounting LIBFAT USB\n");
        unmount_libfat("usb");
        gSDInitDone &= ~USB_MOUNTED_LIBFAT;
    }

    if(gSDInitDone & USB_MOUNTED_LIBNTFS){
        DEBUG_FUNCTION_LINE("Unmounting LIBNTFS USB\n");
        unmountAllNTFS();
        gSDInitDone &= ~USB_MOUNTED_LIBNTFS;
    }*/

    if(gSDInitDone & SDUSB_LIBIOSU_LOADED){
        DEBUG_FUNCTION_LINE("Calling IOSUHAX_Close\n");
        IOSUHAX_Close();
        gSDInitDone &= ~SDUSB_LIBIOSU_LOADED;
    }
    deleteDevTabsNames();
    if(gSDInitDone != SDUSB_MOUNTED_NONE){
        DEBUG_FUNCTION_LINE("WARNING. Some devices are still mounted.\n");
    }
    DEBUG_FUNCTION_LINE("Function end.\n");
}

void deInitApplication(){
    //FileReplacerUtils::getInstance()->StopAsyncThread();
    FileReplacerUtils::destroyInstance();
    deInit_SD_USB();
}

