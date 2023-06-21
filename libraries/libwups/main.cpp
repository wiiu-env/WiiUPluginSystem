#include "wups_reent.h"
#include "wups_thread_specific.h"
#include <cstddef>
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

extern "C" void __attribute__((weak)) wut_set_thread_specific(__wut_thread_specific_id id, void *value);

void wut_set_thread_specific(__wut_thread_specific_id id, void *value) {
    return wups_set_thread_specific(id, value);
}

extern "C" void *__attribute__((weak)) wut_get_thread_specific(__wut_thread_specific_id id);

void *wut_get_thread_specific(__wut_thread_specific_id id) {
    return wups_get_thread_specific(id);
}
