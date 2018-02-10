#include <wups.h>
#include <malloc.h>
#include <string.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "utils/logger.h"

WUPS_MODULE_NAME("test module");
WUPS_MODULE_VERSION("v1.0");
WUPS_MODULE_AUTHOR("Maschell");
WUPS_MODULE_LICENSE("BSD");

INITIALIZE(){
   InitOSFunctionPointers();
   InitSocketFunctionPointers();
   
   log_init();

   log_print("Init of example_plugin!\n");
} 
DECL_FUNCTION(int,FSInit,void){
   log_print("FSInit called\n");
   return real_FSInit();
}


WUPS_MUST_REPLACE(FSInit,WUPS_LOADER_LIBRARY_COREINIT, FSInit);
