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
static void printInfos();
static void printReplacementInfos();

/* Entry point */
extern "C" int Menu_Main(int argc, char **argv){
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging
    InitFSFunctionPointers();

    log_init();

    DEBUG_FUNCTION_LINE("Wii U Plugin System Loader %s\n",APP_VERSION);

    setup_os_exceptions();

    DEBUG_FUNCTION_LINE("End address of loader %08X\n",getApplicationEndAddr());

    memset((void*)&gbl_replacement_data,0,sizeof(gbl_replacement_data));

    DEBUG_FUNCTION_LINE("Mount SD partition\n");

    int res = 0;
    if((res = mount_sd_fat("sd")) >= 0){
        DEBUG_FUNCTION_LINE("Mounting successful\n");
        loadElf("sd:/wiiu/plugins/example_plugin.mod");

        if(module_list_count == 0){
            DEBUG_FUNCTION_LINE("Found no valid modules! =( Exiting\n");
            return EXIT_SUCCESS;
        }

        DEBUG_FUNCTION_LINE("Found %d modules!\n",module_list_count);

        printInfos();

        DEBUG_FUNCTION_LINE("Relocating them now\n");
        unsigned char * space = (unsigned char*)0x01000000;

        Module_ListLink(&space);
        Module_ListLinkFinal();

        DEBUG_FUNCTION_LINE("Flush memory\n");

        DCFlushRange ((void*)getApplicationEndAddr(),0x01000000-getApplicationEndAddr());
        DCInvalidateRange((void*)getApplicationEndAddr(),0x01000000-getApplicationEndAddr());

        if(module_relocations_count != 0){
            DEBUG_FUNCTION_LINE("We still have undefined symbol. Make sure to link them in =/ Exiting\n");
        }

        DEBUG_FUNCTION_LINE("Printing some information before replacing the functions\n");

        printReplacementInfos();

        for(int module_index=0;module_index<gbl_replacement_data.number_used_modules;module_index++){
            new_PatchInvidualMethodHooks(&gbl_replacement_data.module_data[module_index]);
        }

        // The example plugin should override the message.
        OSFatal("We should never see this message.");
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

    }

    DEBUG_FUNCTION_LINE("Application is ending now.\n");

    return EXIT_SUCCESS;
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
