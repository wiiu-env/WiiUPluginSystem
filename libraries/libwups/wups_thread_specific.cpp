#include "wups_thread_specific.h"
#include <stddef.h>
#include <stdint.h>

extern "C" void OSFatal(const char *);
extern "C" void OSReport(const char *, ...);

typedef struct OSThread {
    uint8_t other[0x68c];
    void *reserved[5];
} OSThread;

static_assert(offsetof(OSThread, reserved) == 0x68c, "OSThread: \"reserved\" at wrong offset");
static_assert(sizeof(OSThread) == 0x6a0, "OSThread: wrong size");

extern "C" const char wups_meta_info_dump[];

void wups_set_thread_specific(__wut_thread_specific_id id, void *value) {
    OSThread *thread;
    asm volatile("lwz %0, -0x20(0)"
                 : "=r"(thread)); // OSGetCurrentThread()
    if (thread != nullptr) {
        if (id == WUT_THREAD_SPECIFIC_0) {
            thread->reserved[3] = value;
        } else if (id == WUT_THREAD_SPECIFIC_1) {
            thread->reserved[4] = value;
        } else {
            OSReport("[%s] wups_set_thread_specific: invalid id\n", wups_meta_info_dump);
            OSFatal("wups_set_thread_specific: invalid id");
        }
    } else {
        OSReport("[%s] wups_set_thread_specific: invalid thread\n", wups_meta_info_dump);
        OSFatal("wups_set_thread_specific: invalid thread");
    }
}

void *wups_get_thread_specific(__wut_thread_specific_id id) {
    OSThread *thread;
    asm volatile("lwz %0, -0x20(0)"
                 : "=r"(thread)); // OSGetCurrentThread()
    if (thread != nullptr) {
        if (id == WUT_THREAD_SPECIFIC_0) {
            return thread->reserved[3];
        } else if (id == WUT_THREAD_SPECIFIC_1) {
            return thread->reserved[4];
        } else if ((uint32_t) id == 0x13371337) { // Mechanism to detect if the function was overridden properly
            return (void *) 0x42424242;
        } else {
            OSReport("[%s] wups_get_thread_specific: invalid id\n", wups_meta_info_dump);
            OSFatal("wups_get_thread_specific: invalid id");
        }
    } else {
        OSReport("[%s] wups_get_thread_specific: invalid thread\n", wups_meta_info_dump);
        OSFatal("wups_get_thread_specific: invalid thread\n");
    }
    return nullptr;
}