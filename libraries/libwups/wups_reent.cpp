#include "wups_reent.h"

#include "wups/hooks.h"
#include "wups/reent_internal.h"
#include "wups/wups_debug.h"
#include <coreinit/debug.h>
#include <coreinit/thread.h>
#include <stdlib.h>

struct wups_loader_init_reent_args_t {
    WUPSReent_GetReentContext get_context_ptr       = nullptr;
    WUPSReent_SetSentinel set_sentinel_ptr          = nullptr;
    WUPSReent_RestoreHead restore_head_ptr          = nullptr;
    WUPSReent_AddReentContext add_reent_context_ptr = nullptr;
};

static wups_loader_init_reent_args_t __internal_functions = {};

void WUPSReentAPI_InitInternal(wups_loader_init_reent_args_t_ args) {
    if (args.version > WUPS_REENT_CUR_API_VERSION) {
        OSFatal("Incompatible reent api version");
        return;
    }
    WUPS_DEBUG_REPORT("WUPSReentAPI_InitInternal: Initializing reent plugin\n");

    __internal_functions.get_context_ptr       = args.get_context_ptr;
    __internal_functions.set_sentinel_ptr      = args.set_sentinel_ptr;
    __internal_functions.add_reent_context_ptr = args.add_reent_context_ptr;
    __internal_functions.restore_head_ptr      = args.restore_head_ptr;
}

// use variable in the .data section as unique plugin id
static const int sReentPluginId = 0;

void *wups_backend_get_context(const void *id, wups_loader_init_reent_errors_t_ *outError) {
    return __internal_functions.get_context_ptr(id, outError);
}

void *wups_backend_set_sentinel() {
    return __internal_functions.set_sentinel_ptr();
}

void wups_backend_restore_head(void *head) {
    __internal_functions.restore_head_ptr(head);
}

bool wups_backend_register_context(const void *pluginId, void *reentPtr, void (*cleanupFn)(void *), void *oldHead) {
    return __internal_functions.add_reent_context_ptr(pluginId, reentPtr, cleanupFn, oldHead);
}

static void reclaim_reent_trampoline(void *payload) {
    WUPS_DEBUG_REPORT("reclaim_reent_trampoline: Destroying reent payload: %p\n", payload);

    if (payload) {
        auto *reentPtr = static_cast<_reent *>(payload);
        _reclaim_reent(reentPtr);
        free(reentPtr);
    }
}

struct _reent *__wups_getreent() {
    if (!OSGetCurrentThread()) {
        return _GLOBAL_REENT;
    }
    wups_loader_init_reent_errors_t_ error = WUPSReent_ERROR_NONE;
    if (void *existingCtx = wups_backend_get_context(&sReentPluginId, &error)) {
        return static_cast<_reent *>(existingCtx);
    }
    switch (error) {
        case WUPSReent_ERROR_GLOBAL_REENT_REQUESTED:
        case WUPSReent_ERROR_NO_THREAD:
            return _GLOBAL_REENT;
        case WUPSReent_ERROR_NONE:
            break;
    }

    auto *oldHead  = wups_backend_set_sentinel();
    auto *newReent = static_cast<struct _reent *>(malloc(sizeof(struct _reent)));
    if (!newReent) {
        wups_backend_restore_head(oldHead);
        return _GLOBAL_REENT;
    }

    WUPS_DEBUG_REPORT("Allocated context! for thread %p: %p\n", OSGetCurrentThread(), newReent);

    _REENT_INIT_PTR(newReent);

    bool result = wups_backend_register_context(
            &sReentPluginId,
            newReent,
            reclaim_reent_trampoline,
            oldHead);

    if (!result) {
        WUPS_DEBUG_WARN("Failed to register context for thread %p\n", OSGetCurrentThread());
        _reclaim_reent(newReent);
        free(newReent);
        wups_backend_restore_head(oldHead);
        return _GLOBAL_REENT;
    }

    return newReent;
}