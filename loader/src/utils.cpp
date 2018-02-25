#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>

#include <utils/logger.h>
#include <wups.h>

#include "utils.h"
#include "common/common.h"
#include "common/retain_vars.h"
#include "myutils/overlay_helper.h"

// https://gist.github.com/ccbrown/9722406
void dumpHex(const void* data, size_t size) {
    char ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    for (i = 0; i < size; ++i) {
        log_printf("%02X ", ((unsigned char*)data)[i]);
        if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
            ascii[i % 16] = ((unsigned char*)data)[i];
        } else {
            ascii[i % 16] = '.';
        }
        if ((i+1) % 8 == 0 || i+1 == size) {
            log_printf(" ");
            if ((i+1) % 16 == 0) {
                log_printf("|  %s \n", ascii);
            } else if (i+1 == size) {
                ascii[(i+1) % 16] = '\0';
                if ((i+1) % 16 <= 8) {
                    log_printf(" ");
                }
                for (j = (i+1) % 16; j < 16; ++j) {
                    log_printf("   ");
                }
                log_printf("|  %s \n", ascii);
            }
        }
    }
}

void CallHook(wups_loader_hook_type_t hook_type) {
    CallHookEx(hook_type,-1);
}

void CallHookEx(wups_loader_hook_type_t hook_type, s32 plugin_index_needed) {
    for(int plugin_index=0; plugin_index<gbl_replacement_data.number_used_plugins; plugin_index++) {
        replacement_data_plugin_t * plugin_data = &gbl_replacement_data.plugin_data[plugin_index];
        if(plugin_index_needed != -1 && plugin_index_needed != plugin_index) {
            continue;
        }
        //DEBUG_FUNCTION_LINE("Checking hook functions for %s.\n",plugin_data->plugin_name);
        //DEBUG_FUNCTION_LINE("Found hooks: %d\n",plugin_data->number_used_hooks);
        for(int j=0; j<plugin_data->number_used_hooks; j++) {
            replacement_data_hook_t * hook_data = &plugin_data->hooks[j];
            if(hook_data->type == hook_type) {
                DEBUG_FUNCTION_LINE("Calling hook of type %d for plugin %s\n",hook_data->type,plugin_data->plugin_name);
                void * func_ptr = hook_data->func_pointer;
                //TODO: Switch cases depending on arguments etc.
                // Adding arguments!

                if(func_ptr != NULL) {
                    if(hook_type == WUPS_LOADER_HOOK_INIT_PLUGIN) {
                        wups_loader_init_plugin_args_t args;
                        args.fs_wrapper.open_repl = (const void*)&open;
                        args.fs_wrapper.close_repl = (const void*)&close;
                        args.fs_wrapper.write_repl = (const void*)&write;
                        args.fs_wrapper.read_repl = (const void*)&read;
                        args.fs_wrapper.lseek_repl = (const void*)&lseek;
                        args.fs_wrapper.stat_repl = (const void*)&stat;
                        args.fs_wrapper.fstat_repl = (const void*)&fstat;
                        args.fs_wrapper.opendir_repl = (const void*)&opendir;
                        args.fs_wrapper.closedir_repl = (const void*)&closedir;
                        args.fs_wrapper.readdir_repl = (const void*)&readdir;
                        args.overlayfunction_ptr = (const void*)&overlay_helper;

                        ((void (*)(wups_loader_init_plugin_args_t))((unsigned int*)func_ptr) )(args);
                    } else if(hook_type == WUPS_LOADER_HOOK_DEINIT_PLUGIN) {
                        ((void (*)(void))((unsigned int*)func_ptr) )();
                    }  else if(hook_type == WUPS_LOADER_HOOK_STARTING_APPLICATION) {
                        wups_loader_app_started_args_t args;
                        memset(&args,0,sizeof(args));
                        if(gSDInitDone & WUPS_SD_MOUNTED) {
                            args.sd_mounted = true;
                        }
                        if(gSDInitDone & WUPS_USB_MOUNTED) {
                            args.usb_mounted = true;
                        }
                        ((void (*)(wups_loader_app_started_args_t))((unsigned int*)func_ptr) )(args);
                    } else if(hook_type == WUPS_LOADER_HOOK_ENDING_APPLICATION) {
                        ((void (*)(void))((unsigned int*)func_ptr))();
                    } else if(hook_type == WUPS_LOADER_HOOK_VSYNC) {
                        ((void (*)(void))((unsigned int*)func_ptr))();
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
                        ((void (*)(wups_loader_app_status_t))((unsigned int*)func_ptr))(status);
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
