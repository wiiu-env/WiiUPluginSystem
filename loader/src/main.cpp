#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>

#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <dynamic_libs/sys_functions.h>
#include <dynamic_libs/fs_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <utils/logger.h>
#include <fs/FSUtils.h>
#include <fs/sd_fat_devoptab.h>
#include <utils/utils.h>
#include <system/exception_handler.h>
#include "common/retain_vars.h"
#include "common/common.h"

#include <wups.h>
#include <libelf.h>
#include "version.h"
#include "main.h"
#include "utils.h"
#include "module_parser.h"
#include "link_utils.h"
#include "elf_utils.h"
#include "patcher/function_patcher.h"

static void printModuleInfos();
static bool loadSamplePlugin();
static void printInfos();
static void printReplacementInfos();
static void ApplyPatches();
void CallHook(wups_loader_hook_type_t hook_type);
static void RestorePatches();
s32 isInMiiMakerHBL();


u8 isFirstBoot __attribute__((section(".data"))) = 1;


/* Entry point */
extern "C" int Menu_Main(int argc, char **argv){
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging
    InitSysFunctionPointers();
    InitFSFunctionPointers();

    log_init();

    DEBUG_FUNCTION_LINE("Wii U Plugin System Loader %s\n",APP_VERSION);

    setup_os_exceptions();

    if(isFirstBoot){
        memset((void*)&gbl_replacement_data,0,sizeof(gbl_replacement_data));
        if(!loadSamplePlugin()){
            return    EXIT_SUCCESS;
        }

    }

    if(module_relocations_count != 0){
        DEBUG_FUNCTION_LINE("We still have undefined symbol. Make sure to link them in =/ Exiting\n");
        return EXIT_SUCCESS;
    }

    //Reset everything when were going back to the Mii Maker
    if(!isFirstBoot && isInMiiMakerHBL()){
        DEBUG_FUNCTION_LINE("Returing to the Homebrew Launcher!\n");
        isFirstBoot = 0;
        RestorePatches();
        return EXIT_SUCCESS;
    } else {
        DEBUG_FUNCTION_LINE("Apply patches.\n");
        ApplyPatches();
    }

    if(!isInMiiMakerHBL()){ //Starting the application
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

    RestorePatches();

    return EXIT_SUCCESS;
}

void ApplyPatches(){
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
                if(func_ptr != NULL){
                    DEBUG_FUNCTION_LINE("Calling it! %08X\n",func_ptr);
                    ( (void (*)(void))((unsigned int*)func_ptr) )();
                }else{
                    DEBUG_FUNCTION_LINE("Was not defined\n");
                }
            }

        }
    }
}

void RestorePatches(){
    for(int module_index=gbl_replacement_data.number_used_modules-1;module_index>=0;module_index--){
        DEBUG_FUNCTION_LINE("Restoring function for module: %d\n",module_index);
        new_RestoreInvidualInstructions(&gbl_replacement_data.module_data[module_index]);
    }
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

bool loadSamplePlugin(){
    DEBUG_FUNCTION_LINE("Mount SD partition\n");

    int res = 0;
    if((res = mount_sd_fat("sd")) >= 0){
        DEBUG_FUNCTION_LINE("Mounting successful\n");
        loadElf("sd:/wiiu/plugins/example_plugin.mod");
        loadElf("sd:/wiiu/plugins/sdcafiine.mod");
        loadElf("sd:/wiiu/plugins/padcon.mod");
        loadElf("sd:/wiiu/plugins/swapdrc.mod");
        if(module_list_count == 0){
            DEBUG_FUNCTION_LINE("Found no valid modules! =( Exiting\n");
            return false;
        }

        DEBUG_FUNCTION_LINE("Found %d modules!\n",module_list_count);

        printInfos();

        DEBUG_FUNCTION_LINE("Relocating them now\n");
        unsigned char * space = (unsigned char*)0x01000000;

        if(!Module_ListLink(&space)){
            return false;
        }
        Module_ListLinkFinal();

        DEBUG_FUNCTION_LINE("Flush memory\n");

        DCFlushRange ((void*)getApplicationEndAddr(),0x01000000-getApplicationEndAddr());
        DCInvalidateRange((void*)getApplicationEndAddr(),0x01000000-getApplicationEndAddr());

        if(module_relocations_count != 0){
            DEBUG_FUNCTION_LINE("We still have undefined symbol. Make sure to link them in =/ Exiting\n");
            return false;
        }

        DEBUG_FUNCTION_LINE("Printing some information before replacing the functions\n");

        printReplacementInfos();

        /*
        test code for calling the loaded functions.

        DEBUG_FUNCTION_LINE("We need no more relocations, we can call the functions!!\n");
        DEBUG_FUNCTION_LINE("Calling %d functions!\n",module_entries_count);
        for (unsigned int i = 0; i < module_entries_count; i++) {
            DEBUG_FUNCTION_LINE("--- Function %d ---\n",i);
            if( module_entries[i].type == WUPS_LOADER_ENTRY_FUNCTION ||
                module_entries[i].type == WUPS_LOADER_ENTRY_FUNCTION_MANDATORY){
                DEBUG_FUNCTION_LINE("Let's call the function: %08X \n",module_entries[i].data._function.name);
                DEBUG_FUNCTION_LINE("Let's call the function: %s \n",module_entries[i].data._function.name);
                dumpHex(module_entries[i].data._function.target,0x80);
                //int ret = ( (int (*)(void))((unsigned int*)module_entries[i].data._function.target) )();
                //DEBUG_FUNCTION_LINE("result:  %08X \n",ret);
            }
        }*/

        unmount_sd_fat("sd");
    }
    return true;
}

void loadElf(const char * elfPath){
    DEBUG_FUNCTION_LINE("Reading elf from path: %s\n",elfPath);

    DEBUG_FUNCTION_LINE("Try to load %s\n",elfPath);
    if(Module_CheckFile(elfPath)){
        Module_Load(elfPath);
    }
}

static void printReplacementInfos(){
    DEBUG_FUNCTION_LINE("Found modules: %d\n",gbl_replacement_data.number_used_modules);
    for(int module_index=0;module_index<gbl_replacement_data.number_used_modules;module_index++){
        replacement_data_module_t * module_data = &gbl_replacement_data.module_data[module_index];
        DEBUG_FUNCTION_LINE("Module name: %s\n",module_data->module_name);
        DEBUG_FUNCTION_LINE("Functions that will be replaced: %d\n",module_data->number_used_functions);
        for(int j=0;j<module_data->number_used_functions;j++){
            replacement_data_function_t * function_data = &module_data->functions[j];
            DEBUG_FUNCTION_LINE("[%d] function: %s from lib %d\n",j,function_data->function_name, function_data->library);
        }
        DEBUG_FUNCTION_LINE("--- function list end ---\n");
    }
    DEBUG_FUNCTION_LINE("--- module list end ---\n");
}

static void printInfos(){
        for (unsigned int i = 0; i < module_list_count; i++) {
            DEBUG_FUNCTION_LINE("--- Module %d ---\n",i);
            DEBUG_FUNCTION_LINE("name: %s\n",module_list[i]->name);
            DEBUG_FUNCTION_LINE("path: %s\n",module_list[i]->path);
            DEBUG_FUNCTION_LINE("author: %s\n",module_list[i]->author);
        }
        DEBUG_FUNCTION_LINE("--- Module list end ---\n");
        for (unsigned int i = 0; i < module_entries_count; i++) {
            DEBUG_FUNCTION_LINE("--- Entry %d ---\n",i);
            if( module_entries[i].type == WUPS_LOADER_ENTRY_FUNCTION ||
                module_entries[i].type == WUPS_LOADER_ENTRY_FUNCTION_MANDATORY){
                DEBUG_FUNCTION_LINE("library:  %d \n",module_entries[i].data._function.library);
                DEBUG_FUNCTION_LINE("function: %s \n",module_entries[i].data._function.name);
                DEBUG_FUNCTION_LINE("pointer:  %08X \n",module_entries[i].data._function.target);
            }
        }
        DEBUG_FUNCTION_LINE("--- Entry list end ---\n");
        for (unsigned int i = 0; i < module_relocations_count; i++) {
            DEBUG_FUNCTION_LINE("--- Relocation %d ---\n",i);
            DEBUG_FUNCTION_LINE("name: %s\n",module_relocations[i].name);
        }
        DEBUG_FUNCTION_LINE("--- Relocation list end ---\n");
}
