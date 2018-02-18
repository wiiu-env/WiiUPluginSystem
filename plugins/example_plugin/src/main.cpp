#include <wups.h>
#include <malloc.h>
#include <string.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/fs_functions.h"
#include "utils/logger.h"

WUPS_PLUGIN_NAME("Example plugin");
WUPS_PLUGIN_DESCRIPTION("This is just an example plugin and will log the FSOpenFile function.");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("BSD");

INITIALIZE(args){
   InitOSFunctionPointers();
   InitSocketFunctionPointers();
   
   log_init();

   log_print("Init of example_plugin!\n");
} 

DECL_FUNCTION(int, FSOpenFile, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error) {
    int result = real_FSOpenFile(pClient, pCmd, path, mode, handle, error);
    
    log_printf("FSOpenFile called for folder %s! Result %d\n",path,result);
    return result;
}

WUPS_MUST_REPLACE(FSOpenFile,WUPS_LOADER_LIBRARY_COREINIT, FSOpenFile);
