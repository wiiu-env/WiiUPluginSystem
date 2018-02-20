#ifndef __OVERLAY_HELPER_H_
#define __OVERLAY_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <wups.h>

struct buffer_store{
    void * buffer;
    s32 buffer_size;
    s32 mode;
    s32 surface_format;
    vs32 buffering_mode;
};

void overlay_helper(wups_overlay_options_type_t screen, overlay_callback callback);

#ifdef __cplusplus
}
#endif

#endif
