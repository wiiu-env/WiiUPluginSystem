#include <wups.h>
#include <malloc.h>
#include <string.h>
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <dynamic_libs/fs_functions.h>
#include <utils/logger.h>

// Mandatory plugin information.
WUPS_PLUGIN_NAME("Plugin template");
WUPS_PLUGIN_DESCRIPTION("Description");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("USERNAME");
WUPS_PLUGIN_LICENSE("BSD");

// FS Access
WUPS_FS_ACCESS()
// Overlay access
WUPS_ALLOW_OVERLAY()

// Gets called once the loader exists.
INITIALIZE_PLUGIN(){
	InitOSFunctionPointers();
	InitSocketFunctionPointers();

	log_init();
	DEBUG_FUNCTION_LINE("INITIALIZE_PLUGIN\n");
}

// Called whenever an application was started.
ON_APPLICATION_START(){
   InitOSFunctionPointers();
   InitSocketFunctionPointers();

   log_init();

   DEBUG_FUNCTION_LINE("ON_APPLICATION_START\n");
}

// Called whenever an application is ending
ON_APPLICATION_ENDING(){
    DEBUG_FUNCTION_LINE("ON_APPLICATION_ENDING\n");
}

// Lets replace FSOpenFile
DECL_FUNCTION(int, FSOpenFile, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error) {
    int result = real_FSOpenFile(pClient, pCmd, path, mode, handle, error);

    log_printf("FSOpenFile called for folder %s! Result %d\n",path,result);
    return result;
}

// Replace it.
WUPS_MUST_REPLACE(FSOpenFile,                   WUPS_LOADER_LIBRARY_COREINIT,               FSOpenFile);
