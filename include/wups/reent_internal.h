#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum wups_loader_init_reent_errors_t_ {
    WUPSReent_ERROR_NONE                   = 0,
    WUPSReent_ERROR_GLOBAL_REENT_REQUESTED = 1,
    WUPSReent_ERROR_NO_THREAD              = 2,
} wups_loader_init_reent_errors_t_;

typedef void *(*WUPSReent_GetReentContext)(const void *pluginId, wups_loader_init_reent_errors_t_ *outError);
typedef void *(*WUPSReent_SetSentinel)();
typedef void (*WUPSReent_RestoreHead)(void *oldHead);
typedef bool (*WUPSReent_AddReentContext)(const void *pluginId, void *reentPtr, void (*cleanupFn)(void *), void *oldHead);

typedef uint32_t WUPS_REENT_API_VERSION;

#define WUPS_REENT_CUR_API_VERSION 0x01

typedef struct wups_loader_init_reent_args_t_ {
    WUPS_REENT_API_VERSION version;
    WUPSReent_GetReentContext get_context_ptr;
    WUPSReent_SetSentinel set_sentinel_ptr;
    WUPSReent_RestoreHead restore_head_ptr;
    WUPSReent_AddReentContext add_reent_context_ptr;
} wups_loader_init_reent_args_t_;

void WUPSReentAPI_InitInternal(wups_loader_init_reent_args_t_ args);

#ifdef __cplusplus
}
#endif