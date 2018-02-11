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
#include "utils/fs_utils.h"
#include "common/retain_vars.h"
#include <iosuhax.h>
#include "common/retain_vars.h"
#include "fs_wrapper/fs_retain_vars.h"
#include "common/common.h"
#include "main.h"

#define DEBUG_LOG  1

WUPS_MODULE_NAME("SDCaffiine");
WUPS_MODULE_VERSION("v1.0");
WUPS_MODULE_AUTHOR("Maschell");
WUPS_MODULE_LICENSE("GPL");

static bool setGroupAndOwnerID();
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
    snprintf(gModFolder, FS_MAX_ENTNAME_SIZE, "sd:/sdcafiine/%016llX",OSGetTitleID());

    DEBUG_FUNCTION_LINE("Folder: %s\n",gModFolder);
    DEBUG_FUNCTION_LINE("Mount SD partition\n");
    Init_SD_USB();

    setGroupAndOwnerID();

    g_dirhandles = vc_vector_create(0, sizeof(int), NULL);
    g_filehandles = vc_vector_create(0, sizeof(int), NULL);

    log_print("Init of sd_cafiine!\n");
}


static bool setGroupAndOwnerID(){
    int mcpHandle = MCP_Open();
    if(mcpHandle != 0)
    {
        unsigned char titleInfo[0x80];
        memset(titleInfo, 0, sizeof(titleInfo));

        MCP_GetOwnTitleInfo(mcpHandle, titleInfo);
        MCP_Close(mcpHandle);
        u32 * test = (u32*)titleInfo;
        global_owner_id = test[1];
        global_group_id = test[2];
        DEBUG_FUNCTION_LINE("Set group_id to %08X and owner_id to %08X\n",global_group_id,global_owner_id);
        return true;
    }
    return false;
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
