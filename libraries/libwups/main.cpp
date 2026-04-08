#include "wups_reent.h"
#include <coreinit/debug.h>

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <stdint.h>

extern "C" void OSFatal(const char *);

extern "C" void __wups_start() {
    OSFatal("This file needs to be run with the Wii U Plugin System.");
}

extern "C" __attribute__((weak)) void __wut_socket_init_devoptab();

extern "C" __attribute__((weak)) void __wut_socket_fini_devoptab();

static int __wut_socket_devoptab_added = 0;

extern "C" __attribute__((weak)) void socket_lib_init();

extern "C" void __attribute__((weak)) __init_wut_socket();

void __attribute__((weak)) __init_wut_socket() {
    if (!&__wut_socket_init_devoptab || !&socket_lib_init) return;
    if (!__wut_socket_devoptab_added) {
        socket_lib_init();
        __wut_socket_init_devoptab();
        __wut_socket_devoptab_added = 1;
    }
}

extern "C" void __attribute__((weak)) __fini_wut_socket();

void __attribute__((weak)) __fini_wut_socket() {
    if (!&__wut_socket_init_devoptab || !&__wut_socket_fini_devoptab) return;
    if (__wut_socket_devoptab_added) {
        __wut_socket_fini_devoptab();
        __wut_socket_devoptab_added = 0;
    }
}

extern "C" struct _reent *__getreent(void) {
    return __wups_getreent();
}

extern const char wups_meta_info_dump[];

typedef enum __wut_thread_specific_id {
    WUT_THREAD_SPECIFIC_0 = 0,
    WUT_THREAD_SPECIFIC_1 = 1,
} __wut_thread_specific_id;


extern "C" void *__attribute__((weak)) wut_get_thread_specific(__wut_thread_specific_id id);

void *wut_get_thread_specific(__wut_thread_specific_id id) {
    if ((uint32_t) id == 0x13371337) { // Mechanism to detect if the function was overridden properly
        return (void *) 0x42424242;
    }

    OSReport("[%s] wups_get_thread_specific: NOT SUPPORTED\n", wups_meta_info_dump);
    OSFatal("wups_get_thread_specific: NOT SUPPORTED\n");

    return nullptr;
}


extern "C" const char wups_meta_plugin_name[];
extern "C" void __attribute__((weak)) abort(void);
extern "C" void __attribute__((weak)) __assert_func(const char *file, int line, const char *func, const char *failedexpr);
extern "C" void __attribute__((weak)) __assert(const char *file, int line, const char *failedexpr);

void __attribute__((weak))
abort(void) {
    char buffer[512] = {};
    strcat(buffer, "Wii U Plugin System (plugin: \"");
    strcat(buffer, wups_meta_plugin_name);
    strcat(buffer, "\"):\n Abort called. Uncaught exception?");
    OSFatal(buffer);
    /* NOTREACHED */
    while (1)
        ;
}

void __attribute__((weak))
__assert_func(const char *file,
              int line,
              const char *func,
              const char *failedexpr) {
    char tmp[512]    = {};
    char buffer[512] = {};

    snprintf(tmp, sizeof(tmp), "Wii U Plugin System (plugin: \"%s\"):\n\n"
                               "assertion \"%s\" failed:\n\n"
                               "file \"%s\", line %d%s%s",
             wups_meta_plugin_name, failedexpr, file, line, func ? ", function: " : "", func ? func : "");

    // make sure to add a \n every 64 characters to fit on the DRC screen.
    char *target_ptr = buffer;
    int i = 0, j = 0, lineLength = 0;
    while (tmp[i] != '\0' && j < (int) sizeof(buffer) - 2) {
        if (tmp[i] == '\n') {
            lineLength = 0;
        } else if (lineLength >= 64) {
            target_ptr[j++] = '\n';
            lineLength      = 0;
        }
        target_ptr[j++] = tmp[i++];
        lineLength++;
    }

    OSFatal(buffer);
    while (true)
        ;
    /* NOTREACHED */
}

void __attribute__((weak))
__assert(const char *file,
         int line,
         const char *failedexpr) {
    __assert_func(file, line, NULL, failedexpr);
    /* NOTREACHED */
}
