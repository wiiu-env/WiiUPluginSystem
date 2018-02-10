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

DECL_FUNCTION(void,OSFatal,char * msg){
   InitOSFunctionPointers();
   InitSocketFunctionPointers();
   
   log_init();

   //log_printf is not working.
   log_print(msg);
   log_print("\n^--- Someone called OSFatal with this string. Lets modify it. Bye bye =(\n");
   real_OSFatal("Does the replacement work?");
   return;
}


WUPS_MUST_REPLACE(OSFatal,WUPS_LOADER_LIBRARY_COREINIT, OSFatal);
