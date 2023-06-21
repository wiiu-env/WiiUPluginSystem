#include "wups_reent.h"
#include "wups_thread_specific.h"
#include <cstring>
#include <stdint.h>
#include <stdlib.h>

#define __WUPS_CONTEXT_THREAD_SPECIFIC_ID WUT_THREAD_SPECIFIC_1

extern "C" __attribute__((weak)) void wut_set_thread_specific(__wut_thread_specific_id id, void *value);
extern "C" __attribute__((weak)) void *wut_get_thread_specific(__wut_thread_specific_id);

typedef uint32_t OSThread;

extern const char wups_meta_info_dump[];

extern "C" void OSFatal(const char *);
extern "C" void OSReport(const char *, ...);

extern "C" OSThread *OSGetCurrentThread();

typedef void (*OSThreadCleanupCallbackFn)(OSThread *thread, void *stack);

extern "C" OSThreadCleanupCallbackFn
OSSetThreadCleanupCallback(OSThread *thread,
                           OSThreadCleanupCallbackFn callback);

struct __wups_thread_context {
    struct _reent reent;
    OSThreadCleanupCallbackFn savedCleanup;
};

static void
__wups_thread_cleanup(OSThread *thread,
                      void *stack) {
    struct __wups_thread_context *context;

    context = (struct __wups_thread_context *) wut_get_thread_specific(__WUPS_CONTEXT_THREAD_SPECIFIC_ID);
    if (!context || &context->reent == _GLOBAL_REENT) {
        OSReport("[%s] __wups_thread_cleanup: Context was NULL or reent was global\n", wups_meta_info_dump);
        OSFatal("__wups_thread_cleanup: Context was NULL or reent was global");
    }

    if (context->savedCleanup) {
        context->savedCleanup(thread, stack);
    }

    _reclaim_reent(&context->reent);

    // Use global reent during free since the current reent is getting freed
    wut_set_thread_specific(__WUPS_CONTEXT_THREAD_SPECIFIC_ID, _GLOBAL_REENT);

    free(context);

    wut_set_thread_specific(__WUPS_CONTEXT_THREAD_SPECIFIC_ID, NULL);
}

struct _reent *
__wups_getreent(void) {
    struct __wups_thread_context *context;

    context = (struct __wups_thread_context *) wut_get_thread_specific(__WUPS_CONTEXT_THREAD_SPECIFIC_ID);
    if (!context) {
        // Temporarily use global reent during context allocation
        wut_set_thread_specific(__WUPS_CONTEXT_THREAD_SPECIFIC_ID, _GLOBAL_REENT);

        context = (struct __wups_thread_context *) malloc(sizeof(*context));
        if (!context) {
            OSReport("[%s] __wups_getreent: Failed to allocate reent context\n", wups_meta_info_dump);
            OSFatal("__wups_getreent: Failed to allocate reent context");
            wut_set_thread_specific(__WUPS_CONTEXT_THREAD_SPECIFIC_ID, NULL);
            return NULL;
        }

        _REENT_INIT_PTR(&context->reent);
        context->savedCleanup = OSSetThreadCleanupCallback(OSGetCurrentThread(), &__wups_thread_cleanup);

        wut_set_thread_specific(__WUPS_CONTEXT_THREAD_SPECIFIC_ID, context);
    }

    return &context->reent;
}
