#include <utils/logger.h>
#include <utils/function_patcher.h>
#include "common/retain_vars.h"
#include "hooks_patcher.h"
#include "main.h"

DECL(void, __PPCExit, void){
    DEBUG_FUNCTION_LINE("__PPCExit\n");

    DeInit();

    real___PPCExit();
}

DECL(u32, ProcUIProcessMessages, u32 u){
    u32 res = real_ProcUIProcessMessages(u);
    if(res != gAppStatus){
        DEBUG_FUNCTION_LINE("App status changed from %d to %d \n",gAppStatus,res);
        gAppStatus = res;
    }

    return res;
}

hooks_magic_t method_hooks_hooks[] __attribute__((section(".data"))) = {
    MAKE_MAGIC(__PPCExit,               LIB_CORE_INIT,  STATIC_FUNCTION),
    MAKE_MAGIC(ProcUIProcessMessages,   LIB_PROC_UI,    DYNAMIC_FUNCTION),
};

u32 method_hooks_size_hooks __attribute__((section(".data"))) = sizeof(method_hooks_hooks) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile u32 method_calls_hooks[sizeof(method_hooks_hooks) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));

