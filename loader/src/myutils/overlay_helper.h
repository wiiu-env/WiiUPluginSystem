#ifndef __OVERLAY_HELPER_H_
#define __OVERLAY_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <wups.h>

struct buffer_store {
    void * buffer;
    int32_t buffer_size;
    int32_t mode;
    int32_t surface_format;
    volatile int32_t buffering_mode;
};

void overlay_helper(wups_overlay_options_type_t screen, overlay_callback callback);

#ifdef __cplusplus
}
#endif

#endif
