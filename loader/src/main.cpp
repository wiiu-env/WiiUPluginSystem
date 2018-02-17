#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <dirent.h>

#include <sys/stat.h>
#include <unistd.h>

#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/ax_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/fs_functions.h"
#include "dynamic_libs/nn_nim_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/padscore_functions.h"
#include "dynamic_libs/proc_ui_functions.h"

#include <utils/logger.h>
#include <fs/FSUtils.h>
#include <fs/sd_fat_devoptab.h>
#include <utils/utils.h>
#include <system/exception_handler.h>
#include <system/memory.h>

#include "common/retain_vars.h"
#include "common/common.h"
#include "modules/ModuleData.h"

#include <utils/function_patcher.h>

#include <wups.h>
#include <iosuhax.h>
#include <fat.h>
#include <ntfs.h>

#include "main.h"
#include "utils.h"
#include "Application.h"
#include "patcher/function_patcher.h"
#include "patcher/hooks_patcher.h"
#include "myutils/mocha.h"
#include "myutils/libntfs.h"
#include "myutils/libfat.h"
#include "version.h"
#include "settings/CSettings.h"

static bool loadSamplePlugins();
static void ApplyPatches();
void CallHook(wups_loader_hook_type_t hook_type);
static void RestorePatches();
s32 isInMiiMakerHBL();

static void copyDataIntoGlobalStruct(std::vector<ModuleData *>* modules);
static void loadElf(std::vector<ModuleData *>* modules, const char * elfPath, uint8_t ** space);

u8 isFirstBoot __attribute__((section(".data"))) = 1;

#define PLUGIN_LOCATION_END_ADDRESS 0x01000000

/* Entry point */
extern "C" int Menu_Main(int argc, char **argv){
    if(gAppStatus == 2){
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
    InitPad ScoreFunctionPointers();
    InitAXFunctionPointers();
    InitProcUIFunctionPointers();

    log_init();

    DEBUG_FUNCTION_LINE("We have %d kb for plugins.\n",(PLUGIN_LOCATION_END_ADDRESS-getApplicationEndAddr())/1024);

    DEBUG_FUNCTION_LINE("Wii U Plugin System Loader %s\n",APP_VERSION);

    //setup_os_exceptions();

    DEBUG_FUNCTION_LINE("Mount SD partition\n");
    Init_SD_USB();

    if(isFirstBoot){
        memset((void*)&gbl_replacement_data,0,sizeof(gbl_replacement_data));
        if(!loadSamplePlugins()){
            return EXIT_SUCCESS;
        }

        //!*******************************************************************
        //!                    Initialize heap memory                        *
        //!*******************************************************************
        DEBUG_FUNCTION_LINE("Initialize memory management\n");
        memoryInitialize();

        DEBUG_FUNCTION_LINE("Start main application\n");
        s32 result = Application::instance()->exec();
        DEBUG_FUNCTION_LINE("Main application stopped result: %d\n",result);
        Application::destroyInstance();

        DEBUG_FUNCTION_LINE("Release memory\n");
        memoryRelease();
        CSettings::destroyInstance();
        if(result == APPLICATION_CLOSE_MIIMAKER){
            DeInit();
            return EXIT_SUCCESS;
        }
    }


    //Reset everything when were going back to the Mii Maker
    if(!isFirstBoot && isInMiiMakerHBL()){
        DEBUG_FUNCTION_LINE("Returing to the Homebrew Launcher!\n");
        isFirstBoot = 0;
        DeInit();
        RestorePatches();
        return EXIT_SUCCESS;
    } else {
        DEBUG_FUNCTION_LINE("Apply patches.\n");
        ApplyPatches();
    }

    if(!isInMiiMakerHBL()){ //Starting the application
        DEBUG_FUNCTION_LINE("Calling init hook.\n");
        CallHook(WUPS_LOADER_HOOK_INIT_FUNCTION);
        return EXIT_RELAUNCH_ON_LOAD;
    }

    if(isFirstBoot){ // First boot back to SysMenu
        DEBUG_FUNCTION_LINE("Loading the System Menu\n");
        isFirstBoot = 0;
        SYSLaunchMenu();
        return EXIT_RELAUNCH_ON_LOAD;
    }

    DEBUG_FUNCTION_LINE("Application is ending now.\n");

    DeInit();
    RestorePatches();

    return EXIT_SUCCESS;
}

void ApplyPatches(){
    PatchInvidualMethodHooks(method_hooks_hooks, method_hooks_size_hooks, method_calls_hooks);
    for(int module_index=0;module_index<gbl_replacement_data.number_used_modules;module_index++){
        new_PatchInvidualMethodHooks(&gbl_replacement_data.module_data[module_index]);
    }
}

void CallHook(wups_loader_hook_type_t hook_type){
    for(int module_index=0;module_index<gbl_replacement_data.number_used_modules;module_index++){
        replacement_data_module_t * module_data = &gbl_replacement_data.module_data[module_index];
        DEBUG_FUNCTION_LINE("Checking hook functions for %s.\n",module_data->module_name);
        DEBUG_FUNCTION_LINE("Found hooks: %d\n",module_data->number_used_hooks);
        for(int j=0;j<module_data->number_used_hooks;j++){
            replacement_data_hook_t * hook_data = &module_data->hooks[j];
            if(hook_data->type == hook_type){
                DEBUG_FUNCTION_LINE("Calling hook of type %d\n",hook_data->type);
                void * func_ptr = hook_data->func_pointer;
                //TODO: Switch cases depending on arguments etc.
                // Adding arguments!

                if(func_ptr != NULL){
                    if(hook_type == WUPS_LOADER_HOOK_INIT_FUNCTION){
                        DEBUG_FUNCTION_LINE("Calling it! %08X\n",func_ptr);
                        wups_loader_init_args_t args;
                        args.device_mounted = gSDInitDone;
                        args.fs_wrapper.open_repl = (const void*)&open;
                        args.fs_wrapper.close_repl = (const void*)&close;
                        args.fs_wrapper.write_repl = (const void*)&write;
                        args.fs_wrapper.read_repl = (const void*)&read;
                        args.fs_wrapper.lseek_repl = (const void*)&lseek;
                        args.fs_wrapper.stat_repl = (const void*)&stat;
                        args.fs_wrapper.fstat_repl = (const void*)&fstat;
                        args.fs_wrapper.opendir_repl = (const void*)&opendir;
                        args.fs_wrapper.closedir_repl = (const void*)&closedir;
                        args.fs_wrapper.readdir_repl = (const void*)&readdir;

                        ( (void (*)(wups_loader_init_args_t *))((unsigned int*)func_ptr) )(&args);
                    }
                }else{
                    DEBUG_FUNCTION_LINE("Was not defined\n");
                }
            }
        }
    }
}

void DeInit(){
    DeInit_SD_USB();
}

void RestorePatches(){
    for(int module_index=gbl_replacement_data.number_used_modules-1;module_index>=0;module_index--){
        DEBUG_FUNCTION_LINE("Restoring function for module: %d\n",module_index);
        new_RestoreInvidualInstructions(&gbl_replacement_data.module_data[module_index]);
    }
    RestoreInvidualInstructions(method_hooks_hooks, method_hooks_size_hooks);
}

s32 isInMiiMakerHBL(){
    if (OSGetTitleID != 0 && (
            OSGetTitleID() == 0x000500101004A200 || // mii maker eur
            OSGetTitleID() == 0x000500101004A100 || // mii maker usa
            OSGetTitleID() == 0x000500101004A000 ||// mii maker jpn
            OSGetTitleID() == 0x0005000013374842))
        {
            return 1;
    }
    return 0;
}

bool loadSamplePlugins(){
    if((gSDInitDone & WUPS_SD_MOUNTED) > 0){
        DEBUG_FUNCTION_LINE("Mounting successful. Loading modules\n");

        std::vector<ModuleData *> modules;

        // Set a pointer to the END of our plugin location.
        // So our plugins will be stored from PLUGIN_LOCATION_END_ADDRESS backward. Maximum is getApplicationEndAddr() (behind this loader).
        // Copied the "reserved" direction from brainslug, not sure why we're doing this.
        unsigned char * space = (unsigned char*)PLUGIN_LOCATION_END_ADDRESS;


        // TODO: Load all files from a certain directory, don't use hardcoded paths. (common.h)
        loadElf(&modules, "sd:/wiiu/plugins/hid_to_vpad.mod",&space);
        loadElf(&modules, "sd:/wiiu/plugins/sdcafiine.mod",&space);
        loadElf(&modules, "sd:/wiiu/plugins/padcon.mod",&space);
        loadElf(&modules, "sd:/wiiu/plugins/swipswapme.mod",&space);

        // Copy the data into the global struct locating in the .data section.
        copyDataIntoGlobalStruct(&modules);

        // Free memory.
        for(size_t i = 0; i< modules.size();i++){
            ModuleData * cur_module = modules[i];
            if(cur_module != NULL){
                free(cur_module);
            }
        }

        DEBUG_FUNCTION_LINE("Flush memory\n");
        DCFlushRange ((void*)getApplicationEndAddr(),PLUGIN_LOCATION_END_ADDRESS-getApplicationEndAddr());
        DCInvalidateRange((void*)getApplicationEndAddr(),PLUGIN_LOCATION_END_ADDRESS-getApplicationEndAddr());

        // TODO: keep it mounted for the plugins. But this would require sharing the read/write/open etc. functions from this loader.
        // Idea: Giving the init hook the pointers. Hiding the __wrap function of the plugin behind the INITIALIZE macro.
        // Needs to be tested if this is working. This would have the advantage of adopting all right/accesses from the loader (libfat, libntfs, iosuhax etc.)
        //unmount_sd_fat("sd");
    }
    return true;
}

static void copyDataIntoGlobalStruct(std::vector<ModuleData *>* modules){
    int module_index = 0;
    // Copy data to global struct.
    for(size_t i = 0; i< modules->size();i++){
        ModuleData * cur_module = modules->at(i);

        std::vector<EntryData *> entry_data_list = cur_module->getEntryDataList();
        std::vector<HookData *> hook_data_list = cur_module->getHookDataList();
        if(module_index >= MAXIMUM_MODULES ){
            DEBUG_FUNCTION_LINE("Maximum of %d modules reached. %s won't be loaded!\n",MAXIMUM_MODULES,cur_module->getName().c_str());
            continue;
        }
        if(entry_data_list.size() > MAXIMUM_FUNCTION_PER_MODULE){
            DEBUG_FUNCTION_LINE("Module %s would replace to many function (%d, maximum is %d). It won't be loaded.\n",cur_module->getName().c_str(),entry_data_list.size(),MAXIMUM_FUNCTION_PER_MODULE);
            continue;
        }
        if(hook_data_list.size() > MAXIMUM_HOOKS_PER_MODULE){
            DEBUG_FUNCTION_LINE("Module %s would set too many hooks (%d, maximum is %d). It won't be loaded.\n",cur_module->getName().c_str(),hook_data_list.size(),MAXIMUM_HOOKS_PER_MODULE);
            continue;
        }

        replacement_data_module_t * module_data = &gbl_replacement_data.module_data[module_index];

        strncpy(module_data->module_name,cur_module->getName().c_str(),MAXIMUM_MODULE_NAME_LENGTH-1);

        for(size_t j = 0; j < entry_data_list.size();j++){
            replacement_data_function_t * function_data = &module_data->functions[j];

            EntryData * cur_entry = entry_data_list[j];
            DEBUG_FUNCTION_LINE("Adding entry \"%s\" for module \"%s\"\n",cur_entry->getName().c_str(),module_data->module_name);

            //TODO: Warning/Error if string is too long.
            strncpy(function_data->function_name,cur_entry->getName().c_str(),MAXIMUM_FUNCTION_NAME_LENGTH-1);

            function_data->library = cur_entry->getLibrary();
            function_data->replaceAddr = (u32) cur_entry->getReplaceAddress();
            function_data->replaceCall = (u32) cur_entry->getReplaceCall();

            module_data->number_used_functions++;
        }

        DEBUG_FUNCTION_LINE("Entries for module \"%s\": %d\n",module_data->module_name,module_data->number_used_functions);

        for(size_t j = 0; j < hook_data_list.size();j++){
            replacement_data_hook_t * hook_data = &module_data->hooks[j];

            HookData * hook_entry = hook_data_list[j];

            DEBUG_FUNCTION_LINE("Set hook for module \"%s\" of type %08X to target %08X\n",module_data->module_name,hook_entry->getType(),(void*) hook_entry->getFunctionPointer());
            hook_data->func_pointer = (void*) hook_entry->getFunctionPointer();
            hook_data->type         = hook_entry->getType();
            module_data->number_used_hooks++;
        }

        DEBUG_FUNCTION_LINE("Hooks for module \"%s\": %d\n",module_data->module_name,module_data->number_used_hooks);

        module_index++;
        gbl_replacement_data.number_used_modules++;
    }
}

static void loadElf(std::vector<ModuleData *>* modules, const char * elfPath, uint8_t ** space){
    DEBUG_FUNCTION_LINE("Try to load %s\n",elfPath);

    ModuleData * module = new ModuleData(elfPath,space);
    if(module->isLoadedSuccessfully()){
        DEBUG_FUNCTION_LINE("%s loading was successful!. \n", elfPath);
        modules->push_back(module);
    } else {
        DEBUG_FUNCTION_LINE("%s loading failed. \n", elfPath);
    }
}

void Init_SD_USB() {
    int res = IOSUHAX_Open(NULL);
    if(res < 0){
        ExecuteIOSExploitWithDefaultConfig();
    }
    deleteDevTabsNames();
    mount_fake();
    gSDInitDone |= WUPS_SDUSB_MOUNTED_FAKE;

    if(res < 0){
        DEBUG_FUNCTION_LINE("IOSUHAX_open failed\n");
        if((res = mount_sd_fat("sd")) >= 0){
            DEBUG_FUNCTION_LINE("mount_sd_fat success\n");
            gSDInitDone |= WUPS_SDUSB_MOUNTED_OS_SD;
        }else{
            DEBUG_FUNCTION_LINE("mount_sd_fat failed %d\n",res);
        }
    }else{
        DEBUG_FUNCTION_LINE("Using IOSUHAX for SD/USB access\n");
        gSDInitDone |= WUPS_SDUSB_LIBIOSU_LOADED;
        int ntfs_mounts = mountAllNTFS();
        if(ntfs_mounts > 0){
            gSDInitDone |= WUPS_USB_MOUNTED_LIBNTFS;
        }

        if(mount_libfatAll() == 0){
            gSDInitDone |= WUPS_SD_MOUNTED_LIBFAT;
            gSDInitDone |= WUPS_USB_MOUNTED_LIBFAT;
        }
    }
    DEBUG_FUNCTION_LINE("%08X\n",gSDInitDone);
}

void DeInit_SD_USB(){
    DEBUG_FUNCTION_LINE("Called this function.\n");

    if(gSDInitDone & WUPS_SDUSB_MOUNTED_FAKE){
       DEBUG_FUNCTION_LINE("Unmounting fake\n");
       unmount_fake();
       gSDInitDone &= ~WUPS_SDUSB_MOUNTED_FAKE;
    }
    if(gSDInitDone & WUPS_SDUSB_MOUNTED_OS_SD){
        DEBUG_FUNCTION_LINE("Unmounting OS SD\n");
        unmount_sd_fat("sd");
        gSDInitDone &= ~WUPS_SDUSB_MOUNTED_OS_SD;
    }

    if(gSDInitDone & WUPS_SD_MOUNTED_LIBFAT){
        DEBUG_FUNCTION_LINE("Unmounting LIBFAT SD\n");
        unmount_libfat("sd");
        gSDInitDone &= ~WUPS_SD_MOUNTED_LIBFAT;
    }

    if(gSDInitDone & WUPS_USB_MOUNTED_LIBFAT){
        DEBUG_FUNCTION_LINE("Unmounting LIBFAT USB\n");
        unmount_libfat("usb");
        gSDInitDone &= ~WUPS_USB_MOUNTED_LIBFAT;
    }

    if(gSDInitDone & WUPS_USB_MOUNTED_LIBNTFS){
        DEBUG_FUNCTION_LINE("Unmounting LIBNTFS USB\n");
        unmountAllNTFS();
        gSDInitDone &= ~WUPS_USB_MOUNTED_LIBNTFS;
    }

    if(gSDInitDone & WUPS_SDUSB_LIBIOSU_LOADED){
        DEBUG_FUNCTION_LINE("Calling IOSUHAX_Close\n");
        IOSUHAX_Close();
        gSDInitDone &= ~WUPS_SDUSB_LIBIOSU_LOADED;

    }
    deleteDevTabsNames();
    if(gSDInitDone != WUPS_SDUSB_MOUNTED_NONE){
        DEBUG_FUNCTION_LINE("WARNING. Some devices are still mounted.\n");
    }
    DEBUG_FUNCTION_LINE("Function end.\n");
}
