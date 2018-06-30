/****************************************************************************
 * Copyright (C) 2016-2018 Maschell
 * With code from chadderz and dimok
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

#ifndef _FUNCTION_PATCHER_HOOKS_H_
#define _FUNCTION_PATCHER_HOOKS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <dynamic_libs/os_functions.h>
#include <wups.h>

struct rpl_handling {
    wups_loader_library_type_t library;
    const char rplname[15];
    uint32_t handle;
};
#define STATIC_FUNCTION          0
#define DYNAMIC_FUNCTION         1

#define FUNCTION_PATCHER_METHOD_STORE_SIZE                  13
#define MAXIMUM_PLUGIN_PATH_NAME_LENGTH                     256
#define MAXIMUM_PLUGIN_NAME_LENGTH                          51
#define MAXIMUM_FUNCTION_NAME_LENGTH                        61

struct replacement_data_function_t {
    uint32_t replaceAddr;                                                   /* [needs to be filled] Address of our replacement function */
    uint32_t replaceCall;                                                   /* [needs to be filled] Address to access the real_function */
    wups_loader_library_type_t library;                                     /* [needs to be filled] rpl where the function we want to replace is. */
    char function_name[MAXIMUM_FUNCTION_NAME_LENGTH];                       /* [needs to be filled] name of the function we want to replace */
    uint32_t realAddr;                                                      /* [will be filled] Address of the real function we want to replace. */
    volatile uint32_t replace_data [FUNCTION_PATCHER_METHOD_STORE_SIZE];    /* [will be filled] Space for us to store some jump instructions */
    uint32_t restoreInstruction;                                            /* [will be filled] Copy of the instruction we replaced to jump to our code. */
    uint8_t functionType;                                                   /* [will be filled] */
    uint8_t alreadyPatched;                                                 /* [will be filled] */
};

struct replacement_data_hook_t {
    void * func_pointer = NULL;                                     /* [will be filled] */
    wups_loader_hook_type_t type;                                   /* [will be filled] */
};

#define MAXIMUM_HOOKS_PER_PLUGIN                        10
#define MAXIMUM_FUNCTION_PER_PLUGIN                     100

struct replacement_data_plugin_t {
    char                        path[MAXIMUM_PLUGIN_PATH_NAME_LENGTH] = "";     // Path where the plugin is stored
    char                        plugin_name[MAXIMUM_PLUGIN_NAME_LENGTH] = "";   // Name of this plugin
    int32_t                     priority;                                       // Priority of this plugin
    int32_t                     number_used_functions;                          // Number of used function. Maximum is MAXIMUM_FUNCTION_PER_PLUGIN
    replacement_data_function_t functions[MAXIMUM_FUNCTION_PER_PLUGIN];         // Replacement information for each function.

    int32_t                     number_used_hooks;                              // Number of used hooks. Maximum is MAXIMUM_HOOKS_PER_PLUGIN
    replacement_data_hook_t     hooks[MAXIMUM_HOOKS_PER_PLUGIN];                // Replacement information for each function.
};

#define MAXIMUM_PLUGINS                                 32

struct replacement_data_t {
    int32_t                     number_used_plugins = 0;                        // Number of used function. Maximum is MAXIMUM_FUNCTION_PER_PLUGIN
    replacement_data_plugin_t   plugin_data[MAXIMUM_PLUGINS];
};

void new_PatchInvidualMethodHooks(replacement_data_plugin_t * data);
void new_RestoreInvidualInstructions(replacement_data_plugin_t * plugin_data);
uint32_t new_GetAddressOfFunction(const char * functionName,wups_loader_library_type_t library);
int32_t new_isDynamicFunction(uint32_t physicalAddress);
void new_resetLibs();

#ifdef __cplusplus
}
#endif

#endif /* _FUNCTION_PATCHER_HOOKS_H_ */
