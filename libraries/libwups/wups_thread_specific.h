#pragma once

typedef enum __wut_thread_specific_id {
    WUT_THREAD_SPECIFIC_0 = 0,
    WUT_THREAD_SPECIFIC_1 = 1,
} __wut_thread_specific_id;

#ifdef __cplusplus
extern "C" {
#endif

void wups_set_thread_specific(__wut_thread_specific_id id, void *value);

void *wups_get_thread_specific(__wut_thread_specific_id id);

#ifdef __cplusplus
}
#endif