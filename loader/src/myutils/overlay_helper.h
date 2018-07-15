#ifndef __OVERLAY_HELPER_H_
#define __OVERLAY_HELPER_H_

#include <wups.h>

#ifdef __cplusplus
extern "C" {
#endif

struct buffer_store {
    void * buffer;
    int32_t buffer_size;
    int32_t mode;
    int32_t surface_format;
    volatile int32_t buffering_mode;
};

#ifdef __cplusplus
}
#endif

void overlay_helper(wups_overlay_options_type_t screen, overlay_callback callback, void * args);

#endif
