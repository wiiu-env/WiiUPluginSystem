#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <dirent.h>

#include <sys/stat.h>
#include <unistd.h>

#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/gx2_functions.h>
#include <dynamic_libs/ax_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <dynamic_libs/sys_functions.h>
#include <dynamic_libs/fs_functions.h>
#include <dynamic_libs/nn_nim_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <dynamic_libs/padscore_functions.h>
#include <dynamic_libs/proc_ui_functions.h>

#include <fs/FSUtils.h>
#include <fs/sd_fat_devoptab.h>
#include <utils/logger.h>
#include <utils/utils.h>
#include <utils/function_patcher.h>
#include <system/exception_handler.h>
#include <system/memory.h>

#include <kernel/kernel_utils.h>
#include <mykernel/kernel_utils.h>

#include "mymemory/memory_mapping.h"

#include "common/retain_vars.h"
#include "common/common.h"
#include "plugin/PluginLoader.h"
#include "plugin/PluginInformation.h"
#include "plugin/DynamicLinkingHelper.h"

#include <wups.h>
#include <iosuhax.h>
#include <fat.h>
#include <ntfs.h>

#include "main.h"
#include "utils.h"
#include "Application.h"
#include "patcher/function_patcher.h"
#include "patcher/hooks_patcher.h"
#include "plugin/dynamic_linking_defines.h"
#include "myutils/mocha.h"
#include "myutils/libntfs.h"
#include "myutils/libfat.h"
#include "myutils/overlay_helper.h"
#include "version.h"
#include "settings/CSettings.h"

static void ApplyPatchesAndCallHookStartingApp();
static void RestorePatches();
int32_t isInMiiMakerHBL();
void readAndPrintSegmentRegister(CThread *thread, void *arg);


extern "C" int32_t Menu_Main(int32_t argc, char **argv) {
    if(gAppStatus == 2) {
        //"No, we don't want to patch stuff again.");
        return EXIT_RELAUNCH_ON_LOAD;
    }

    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging
    InitSysFunctionPointers();
    InitFSFunctionPointers();
    InitGX2FunctionPointers();
    InitSysFunctionPointers();
    InitVPadFunctionPointers();
    InitPadScoreFunctionPointers();
    InitAXFunctionPointers();
    InitProcUIFunctionPointers();

    log_init();

    DEBUG_FUNCTION_LINE("We have %d kb for plugins.\n",(PLUGIN_LOCATION_END_ADDRESS-getApplicationEndAddr())/1024);
    //setup_os_exceptions();

    DEBUG_FUNCTION_LINE("Wii U Plugin System Loader %s\n",APP_VERSION);
    DEBUG_FUNCTION_LINE("Sizeof dyn_linking_relocation_data_t %d\n",sizeof(dyn_linking_relocation_data_t));
    Init();

    init_kernel_syscalls();
    wups_init_kernel_syscalls();

    gGameTitleID = OSGetTitleID();

    int32_t result = 0;

    //Reset everything when were going back to the Mii Maker
    if(isInMiiMakerHBL()) {
        CallHook(WUPS_LOADER_HOOK_DEINIT_PLUGIN);
        // Restore patches as the patched functions could change.
        RestorePatches();

        DynamicLinkingHelper::getInstance()->clearAll();

        //PluginLoader * pluginLoader  = PluginLoader::getInstance();
        //std::vector<PluginInformation *> pluginList = pluginLoader->getPluginInformation("sd:/wiiu/plugins/");
        //pluginLoader->loadAndLinkPlugins(pluginList);
        //pluginLoader->clearPluginInformation(pluginList);

        //!*******************************************************************
        //!                    Initialize heap memory                        *
        //!*******************************************************************
        DEBUG_FUNCTION_LINE("Initialize memory management\n");
        memoryInitialize();

        DEBUG_FUNCTION_LINE("Start main application\n");
        result = Application::instance()->exec();
        DEBUG_FUNCTION_LINE("Main application stopped result: %d\n",result);

        DEBUG_FUNCTION_LINE("Application::destroyInstance\n");
        Application::destroyInstance();

        DEBUG_FUNCTION_LINE("Release memory\n");
        memoryRelease();
        CSettings::destroyInstance();
        PluginLoader::destroyInstance();
    }

    if(result == APPLICATION_CLOSE_APPLY_MEMORY) {
        if(!MemoryMapping::isMemoryMapped()) {
            MemoryMapping::setupMemoryMapping();
        }
    }

    DEBUG_FUNCTION_LINE("Do relocations\n");

    std::vector<dyn_linking_relocation_entry_t *> relocations = DynamicLinkingHelper::getInstance()->getAllValidDynamicLinkingRelocations();
    DEBUG_FUNCTION_LINE("Found relocation information for %d functions\n",relocations.size());

    if(!DynamicLinkingHelper::getInstance()->fillRelocations(relocations)){
        OSFatal("fillRelocations failed.");
    }

    if(!isInMiiMakerHBL()) {
        DEBUG_FUNCTION_LINE("Apply patches.\n");
        ApplyPatchesAndCallHookStartingApp();


        if(MemoryMapping::isMemoryMapped()) {
            DEBUG_FUNCTION_LINE("Mapping was already done. Running %016llX\n",gGameTitleID);
            readAndPrintSegmentRegister(NULL,NULL);
            MemoryMapping::readTestValuesFromMemory();
        } else {
            DEBUG_FUNCTION_LINE("<-----------------------------------------------------> \n");
            DEBUG_FUNCTION_LINE("<---------------- COPY PASTE ME START-----------------> \n");
            DEBUG_FUNCTION_LINE("<-----------------------------------------------------> \n");
            DEBUG_FUNCTION_LINE("Mapping was't done. Running %016llX\n",gGameTitleID);
            readAndPrintSegmentRegister(NULL,NULL);
            DEBUG_FUNCTION_LINE("<-----------------------------------------------------> \n");
            DEBUG_FUNCTION_LINE("<----------------- COPY PASTE ME END -----------------> \n");
            DEBUG_FUNCTION_LINE("<-----------------------------------------------------> \n");
        }
        return EXIT_RELAUNCH_ON_LOAD;
    }

    if(result == APPLICATION_CLOSE_APPLY || result == APPLICATION_CLOSE_APPLY_MEMORY) {
        CallHook(WUPS_LOADER_HOOK_INIT_KERNEL);
        CallHook(WUPS_LOADER_HOOK_INIT_FS);
        CallHook(WUPS_LOADER_HOOK_INIT_OVERLAY);
        CallHook(WUPS_LOADER_HOOK_INIT_PLUGIN);
        DEBUG_FUNCTION_LINE("Loading the system menu.\n");
        DeInit();
        SYSLaunchMenu();
        return EXIT_RELAUNCH_ON_LOAD;
    }

    DEBUG_FUNCTION_LINE("Let's go to back to the Homebrew Launcher\n");
    DEBUG_FUNCTION_LINE("Restoring the patched functions\n");
    RestorePatches();
    DEBUG_FUNCTION_LINE("Calling the plugin deinit hook\n");
    CallHook(WUPS_LOADER_HOOK_DEINIT_PLUGIN);
    DEBUG_FUNCTION_LINE("Unmounting SD/USB devices\n");
    DeInit();
    DEBUG_FUNCTION_LINE("Bye bye!\n");
    return EXIT_SUCCESS;
}

void ApplyPatchesAndCallHookStartingApp() {
    PatchInvidualMethodHooks(method_hooks_hooks, method_hooks_size_hooks, method_calls_hooks);
    for(int32_t plugin_index=0; plugin_index<gbl_replacement_data.number_used_plugins; plugin_index++) {
        CallHookEx(WUPS_LOADER_HOOK_STARTING_APPLICATION,plugin_index);
        new_PatchInvidualMethodHooks(&gbl_replacement_data.plugin_data[plugin_index]);
        CallHookEx(WUPS_LOADER_HOOK_FUNCTIONS_PATCHED,plugin_index);
    }
}

void DeInit() {
    DeInit_SD_USB();
}

void RestorePatches() {
    for(int32_t plugin_index=gbl_replacement_data.number_used_plugins-1; plugin_index>=0; plugin_index--) {
        DEBUG_FUNCTION_LINE("Restoring function for plugin: %d\n",plugin_index);
        new_RestoreInvidualInstructions(&gbl_replacement_data.plugin_data[plugin_index]);
    }
    RestoreInvidualInstructions(method_hooks_hooks, method_hooks_size_hooks);
}

int32_t isInMiiMakerHBL() {
    if (OSGetTitleID != 0 && (
                OSGetTitleID() == 0x000500101004A200 || // mii maker eur
                OSGetTitleID() == 0x000500101004A100 || // mii maker usa
                OSGetTitleID() == 0x000500101004A000 ||// mii maker jpn
                OSGetTitleID() == 0x0005000013374842)) {
        return 1;
    }
    return 0;
}

void Init() {
    memset(&tv_store,0,sizeof(tv_store));
    memset(&drc_store,0,sizeof(drc_store));
    DEBUG_FUNCTION_LINE("Mount SD partition\n");
    Init_SD_USB();
}

void Init_SD_USB() {
    int32_t res = IOSUHAX_Open(NULL);
    if(res < 0) {
        ExecuteIOSExploitWithDefaultConfig();
    }
    deleteDevTabsNames();
    mount_fake();
    gSDInitDone |= WUPS_SDUSB_MOUNTED_FAKE;

    if(res < 0) {
        DEBUG_FUNCTION_LINE("IOSUHAX_open failed\n");
        if((res = mount_sd_fat("sd")) >= 0) {
            DEBUG_FUNCTION_LINE("mount_sd_fat success\n");
            gSDInitDone |= WUPS_SDUSB_MOUNTED_OS_SD;
        } else {
            DEBUG_FUNCTION_LINE("mount_sd_fat failed %d\n",res);
        }
    } else {
        DEBUG_FUNCTION_LINE("Using IOSUHAX for SD/USB access\n");
        gSDInitDone |= WUPS_SDUSB_LIBIOSU_LOADED;
        int32_t ntfs_mounts = mountAllNTFS();
        if(ntfs_mounts > 0) {
            gSDInitDone |= WUPS_USB_MOUNTED_LIBNTFS;
        }

        if(mount_libfatAll() == 0) {
            gSDInitDone |= WUPS_SD_MOUNTED_LIBFAT;
            gSDInitDone |= WUPS_USB_MOUNTED_LIBFAT;
        }
    }
}

void DeInit_SD_USB() {
    DEBUG_FUNCTION_LINE("Called this function.\n");

    if(gSDInitDone & WUPS_SDUSB_MOUNTED_FAKE) {
        DEBUG_FUNCTION_LINE("Unmounting fake\n");
        unmount_fake();
        gSDInitDone &= ~WUPS_SDUSB_MOUNTED_FAKE;
    }
    if(gSDInitDone & WUPS_SDUSB_MOUNTED_OS_SD) {
        DEBUG_FUNCTION_LINE("Unmounting OS SD\n");
        unmount_sd_fat("sd");
        gSDInitDone &= ~WUPS_SDUSB_MOUNTED_OS_SD;
    }

    if(gSDInitDone & WUPS_SD_MOUNTED_LIBFAT) {
        DEBUG_FUNCTION_LINE("Unmounting LIBFAT SD\n");
        unmount_libfat("sd");
        gSDInitDone &= ~WUPS_SD_MOUNTED_LIBFAT;
    }

    if(gSDInitDone & WUPS_USB_MOUNTED_LIBFAT) {
        DEBUG_FUNCTION_LINE("Unmounting LIBFAT USB\n");
        unmount_libfat("usb");
        gSDInitDone &= ~WUPS_USB_MOUNTED_LIBFAT;
    }

    if(gSDInitDone & WUPS_USB_MOUNTED_LIBNTFS) {
        DEBUG_FUNCTION_LINE("Unmounting LIBNTFS USB\n");
        unmountAllNTFS();
        gSDInitDone &= ~WUPS_USB_MOUNTED_LIBNTFS;
    }

    if(gSDInitDone & WUPS_SDUSB_LIBIOSU_LOADED) {
        DEBUG_FUNCTION_LINE("Calling IOSUHAX_Close\n");
        IOSUHAX_Close();
        gSDInitDone &= ~WUPS_SDUSB_LIBIOSU_LOADED;

    }
    deleteDevTabsNames();
    if(gSDInitDone != WUPS_SDUSB_MOUNTED_NONE) {
        DEBUG_FUNCTION_LINE("WARNING. Some devices are still mounted.\n");
    }
    DEBUG_FUNCTION_LINE("Function end.\n");
}
