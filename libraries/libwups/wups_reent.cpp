#include "wups_reent.h"

#include "wups/hooks.h"
#include "wups/reent_internal.h"
#include "wups/wups_debug.h"
#include <coreinit/debug.h>
#include <coreinit/memdefaultheap.h>
#include <coreinit/thread.h>

struct wups_loader_init_reent_args_t {
    WUPSReent_GetReentContext get_context_ptr       = nullptr;
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
    __internal_functions.add_reent_context_ptr = args.add_reent_context_ptr;
}

// use variable in the .data section as unique plugin id
static const int sReentPluginId = 0;

bool wups_backend_get_context(const void *id, void **outPtr) {
    return __internal_functions.get_context_ptr(id, outPtr);
}

bool wups_backend_register_context(const void *pluginId, void *reentPtr, void (*cleanupFn)(void *)) {
    return __internal_functions.add_reent_context_ptr(pluginId, reentPtr, cleanupFn);
}

static void reclaim_reent_trampoline(void *payload) {
    WUPS_DEBUG_REPORT("reclaim_reent_trampoline: Destroying reent payload: %p\n", payload);

    if (payload) {
        auto *reentPtr = static_cast<_reent *>(payload);
        _reclaim_reent(reentPtr);
        // Make sure to use MEMFreeToDefaultHeap
        MEMFreeToDefaultHeap(reentPtr);
    }
}

struct _reent *__wups_getreent() {
    if (!OSGetCurrentThread()) {
        return _GLOBAL_REENT;
    }
    void *existingCtx = nullptr;
    if (!wups_backend_get_context(&sReentPluginId, &existingCtx)) {
        return _GLOBAL_REENT;
    }
    // if non-null we can use it
    if (existingCtx) {
        return static_cast<_reent *>(existingCtx);
    }

    // Use `MEMAllocFromDefaultHeap` to avoid creating a new reent for allocating the reent
    auto *newReent = static_cast<struct _reent *>(MEMAllocFromDefaultHeap(sizeof(struct _reent)));
    if (!newReent) {
        return _GLOBAL_REENT;
    }

    WUPS_DEBUG_REPORT("Allocated context! for thread %p: %p\n", OSGetCurrentThread(), newReent);

    _REENT_INIT_PTR(newReent);

    const bool result = wups_backend_register_context(
            &sReentPluginId,
            newReent,
            reclaim_reent_trampoline);

    if (!result) {
        WUPS_DEBUG_WARN("Failed to register context for thread %p\n", OSGetCurrentThread());
        _reclaim_reent(newReent);
        // Make sure to call the free function
        MEMFreeToDefaultHeap(newReent);
        return _GLOBAL_REENT;
    }

    return newReent;
}