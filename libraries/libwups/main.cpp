extern "C" void OSFatal(const char *msg);

extern "C" void __wups_start() {
    OSFatal("This file needs to be run with the Wii U Plugin System.");
}

extern __attribute__((weak)) void __wut_socket_init_devoptab();

extern __attribute__((weak)) void __wut_socket_fini_devoptab();

static int __wut_socket_devoptab_added = 0;

extern void socket_lib_init();

void __attribute__((weak)) __init_wut_socket() {
    if (!&__wut_socket_init_devoptab) return;
    if (!__wut_socket_devoptab_added) {
        socket_lib_init();
        __wut_socket_init_devoptab();
        __wut_socket_devoptab_added = 1;
    }
}

void __attribute__((weak)) __fini_wut_socket() {
    if (!&__wut_socket_init_devoptab || !&__wut_socket_fini_devoptab) return;
    if (__wut_socket_devoptab_added) {
        __wut_socket_fini_devoptab();
        __wut_socket_devoptab_added = 0;
    }
}