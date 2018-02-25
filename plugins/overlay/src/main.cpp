/****************************************************************************
 * Copyright (C) 2018 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <wups.h>
#include <stdio.h>

#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <dynamic_libs/fs_functions.h>
#include <dynamic_libs/gx2_functions.h>
#include <dynamic_libs/gx2_types.h>
#include <dynamic_libs/socket_functions.h>
#include <utils/logger.h>
#include "main.h"

WUPS_PLUGIN_NAME("Overlay test");
WUPS_PLUGIN_DESCRIPTION("Just an overlay test. Press A,B,X and Y and a text should appear.");
WUPS_PLUGIN_VERSION("v0.1");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

INITIALIZE_PLUGIN(){
    // To init the overlay and FS
}

/* Entry point */
ON_APPLICATION_START(args){
    InitOSFunctionPointers();
    InitSocketFunctionPointers(); //For logging
    InitVPadFunctionPointers(); 
    InitFSFunctionPointers(); 

    InitGX2FunctionPointers();
    
    log_init();
    
    DEBUG_FUNCTION_LINE("OVERLAY TEST INIT DONE.\n");
}