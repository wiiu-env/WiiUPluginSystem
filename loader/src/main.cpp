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

#include <wups.h>
#include <libelf.h>
#include "version.h"
#include "main.h"
#include "utils.h"
#include "module_parser.h"
#include "link_utils.h"
#include "elf_utils.h"

static void printModuleInfos();
static void printInfos();

/* Entry point */
extern "C" int Menu_Main(int argc, char **argv){
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging
    InitFSFunctionPointers();

    log_init();

    DEBUG_FUNCTION_LINE("Wii U Plugin System Loader %s\n",APP_VERSION);

    setup_os_exceptions();

    DEBUG_FUNCTION_LINE("Mount SD partition\n");

    int res = 0;
    if((res = mount_sd_fat("sd")) >= 0){
        DEBUG_FUNCTION_LINE("Mounting successful\n");
        loadAndProcessElf("sd:/wiiu/plugins/example_plugin.mod");
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

    DEBUG_FUNCTION_LINE("Now try to use brainslug code\n");
    if(Module_CheckFile(elfPath)){
        Module_Load(elfPath);
        DEBUG_FUNCTION_LINE("Found %d modules!\n",module_list_count);
        //printInfos();
        unsigned char * space = (unsigned char*)0x00910000;
        Module_ListLink(&space);

        if(module_relocations_count == 0){
            DEBUG_FUNCTION_LINE("We need no relocations, we can call the functions!!\n");
            DEBUG_FUNCTION_LINE("Calling %d functions!\n",module_entries_count);
            for (unsigned int i = 0; i < module_entries_count; i++) {
                DEBUG_FUNCTION_LINE("--- Function %d ---\n",i);
                if( module_entries[i].type == WUPS_LOADER_ENTRY_FUNCTION ||
                    module_entries[i].type == WUPS_LOADER_ENTRY_FUNCTION_MANDATORY){
                    DEBUG_FUNCTION_LINE("Let's call the function: %s \n",module_entries[i].data._function.name);
                    int ret = ( (int (*)(void))((unsigned int*)module_entries[i].data._function.target) )();
                    DEBUG_FUNCTION_LINE("result:  %d \n",ret);
                }
            }
        }
    }

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
