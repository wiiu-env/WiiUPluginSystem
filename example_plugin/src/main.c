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

int func(void);


static int my_func2(void)
{
   InitOSFunctionPointers();
   InitSocketFunctionPointers();
   
   log_init();

   //log_printf is not working.
   log_print("Logging from custom function.\n");

   return 43;
}


WUPS_MUST_REPLACE(func,WUPS_LOADER_LIBRARY_GX2, my_func2);
