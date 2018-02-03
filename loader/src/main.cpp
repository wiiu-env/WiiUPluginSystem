#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>

#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <utils/logger.h>
#include <system/exception_handler.h>

#include "main.h"


/* Entry point */
extern "C" int Menu_Main(void){
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging

    log_init();

    setup_os_exceptions();

    DEBUG_FUNCTION_LINE("Application started.\n");

    return EXIT_SUCCESS;
}

