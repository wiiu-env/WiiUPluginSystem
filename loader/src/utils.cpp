#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>

#include <utils/logger.h>
#include <wups.h>

#include "utils.h"
#include <utils/logger.h>
#include "common/common.h"
#include "common/retain_vars.h"
#include "myutils/overlay_helper.h"
#include "kernel/syscalls.h"

void CallHook(wups_loader_hook_type_t hook_type) {
    CallHookEx(hook_type,-1);
}

#ifdef __cplusplus
extern "C" {
#endif
    // Part of libutils
    extern uint32_t kern_read(const void *addr);
    extern void kern_write(void *addr, uint32_t value);
#ifdef __cplusplus
}
#endif

void CallHookEx(wups_loader_hook_type_t hook_type, int32_t plugin_index_needed) {
    for(int32_t plugin_index=0; plugin_index<gbl_replacement_data.number_used_plugins; plugin_index++) {
        replacement_data_plugin_t * plugin_data = &gbl_replacement_data.plugin_data[plugin_index];
        if(plugin_index_needed != -1 && plugin_index_needed != plugin_index) {
            continue;
        }

        #warning TODO: change the order of the wups_loader_hook_type_t enum before an offical release.
        //DEBUG_FUNCTION_LINE("Checking hook functions for %s.\n",plugin_data->plugin_name);
        //DEBUG_FUNCTION_LINE("Found hooks: %d\n",plugin_data->number_used_hooks);
        for(int32_t j=0; j<plugin_data->number_used_hooks; j++) {
            replacement_data_hook_t * hook_data = &plugin_data->hooks[j];
            if(hook_data->type == hook_type) {
                DEBUG_FUNCTION_LINE("Calling hook of type %d for plugin %s\n",hook_data->type,plugin_data->plugin_name);
                void * func_ptr = hook_data->func_pointer;
                //TODO: Switch cases depending on arguments etc.
                // Adding arguments!
                if(func_ptr != NULL) {
                    if(hook_type == WUPS_LOADER_HOOK_INIT_FS) {
                        wups_loader_init_fs_args_t args;
                        // open is defined as "extern int open (const char *, int, ...);", we are ignoring the varargs part
                        args.open_repl = (OpenFunction) &open;
                        args.close_repl = &close;
                        args.write_repl = &write;
                        args.read_repl = &read;
                        args.lseek_repl = &lseek;
                        args.stat_repl = &stat;
                        args.fstat_repl = &fstat;
                        args.opendir_repl = &opendir;
                        args.closedir_repl = &closedir;
                        args.readdir_repl = &readdir;
                        args.mkdir_repl = &mkdir;
                        ((void (*)(wups_loader_init_fs_args_t))((uint32_t*)func_ptr) )(args);
                    } else if(hook_type == WUPS_LOADER_HOOK_INIT_OVERLAY) {
                        wups_loader_init_overlay_args_t args;
                        args.overlayfunction_ptr = &overlay_helper;
                        ((void (*)(wups_loader_init_overlay_args_t))((uint32_t*)func_ptr) )(args);
                    } else if(hook_type == WUPS_LOADER_HOOK_INIT_PLUGIN) {
                        ((void (*)(void))((uint32_t*)func_ptr) )();
                    } else if(hook_type == WUPS_LOADER_HOOK_DEINIT_PLUGIN) {
                        ((void (*)(void))((uint32_t*)func_ptr) )();
                    } else if(hook_type == WUPS_LOADER_HOOK_STARTING_APPLICATION) {
                        wups_loader_app_started_args_t args;
                        memset(&args,0,sizeof(args));
                        if(gSDInitDone & WUPS_SD_MOUNTED) {
                            args.sd_mounted = true;
                        }
                        if(gSDInitDone & WUPS_USB_MOUNTED) {
                            args.usb_mounted = true;
                        }
                        if(plugin_data->kernel_allowed && plugin_data->kernel_init_done){
                            args.kernel_access = true;
                        }
                        ((void (*)(wups_loader_app_started_args_t))((uint32_t*)func_ptr) )(args);
                    } else if(hook_type == WUPS_LOADER_HOOK_FUNCTIONS_PATCHED) {
                        ((void (*)(void))((uint32_t*)func_ptr))();
                    } else if(hook_type == WUPS_LOADER_HOOK_ENDING_APPLICATION) {
                        ((void (*)(void))((uint32_t*)func_ptr))();
                    } else if(hook_type == WUPS_LOADER_HOOK_VSYNC) {
                        ((void (*)(void))((uint32_t*)func_ptr))();
                    } else if(hook_type == WUPS_LOADER_HOOK_APP_STATUS_CHANGED) {
                        wups_loader_app_status_t status;
                        if(gAppStatus == 0) {
                            status = WUPS_APP_STATUS_FOREGROUND;
                        } else if(gAppStatus == 2) {
                            status = WUPS_APP_STATUS_BACKGROUND;
                        } else if(gAppStatus == 3) {
                            status = WUPS_APP_STATUS_CLOSED;
                        } else {
                            status = WUPS_APP_STATUS_UNKNOWN;
                        }
                        ((void (*)(wups_loader_app_status_t))((uint32_t*)func_ptr))(status);
                    } else if(hook_type == WUPS_LOADER_HOOK_INIT_KERNEL){
                        // Only call the hook if kernel is allowed.
                        if(plugin_data->kernel_allowed){
                            wups_loader_init_kernel_args_t args;
                            args.kern_read_ptr = &kern_read;
                            args.kern_write_ptr = &kern_write;
                            args.kern_copy_data_ptr = &SC0x25_KernelCopyData;
                            ((void (*)(wups_loader_init_kernel_args_t))((uint32_t*)func_ptr) )(args);
                            plugin_data->kernel_init_done = true;
                        }
                    } else {
                        DEBUG_FUNCTION_LINE("ERROR: HOOK TYPE WAS NOT IMPLEMENTED %08X \n",hook_type);
                    }
                } else {
                    DEBUG_FUNCTION_LINE("Failed to call hook. It was not defined\n");
                }
            }
        }
    }
}
