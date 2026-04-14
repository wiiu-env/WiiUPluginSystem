#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*WUPSReent_GetReentContext)(const void *pluginId, void **outPtr);
typedef bool (*WUPSReent_AddReentContext)(const void *pluginId, void *reentPtr, void (*cleanupFn)(void *));

typedef uint32_t WUPS_REENT_API_VERSION;

#define WUPS_REENT_CUR_API_VERSION 0x02

typedef struct wups_loader_init_reent_args_t_ {
    WUPS_REENT_API_VERSION version;
    WUPSReent_GetReentContext get_context_ptr;
    WUPSReent_AddReentContext add_reent_context_ptr;
} wups_loader_init_reent_args_t_;

void WUPSReentAPI_InitInternal(wups_loader_init_reent_args_t_ args);

#ifdef __cplusplus
}
#endif