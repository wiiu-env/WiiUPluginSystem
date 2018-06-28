#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <wups.h>

void OSScreenInit(void);
void OSScreenShutdown(void);
uint32_t OSScreenGetBufferSizeEx(uint32_t bufferNum);
void OSScreenSetBufferEx(uint32_t bufferNum, void * addr);
void OSScreenClearBufferEx(uint32_t bufferNum, uint32_t temp);
void OSScreenFlipBuffersEx(uint32_t bufferNum);
void OSScreenPutFontEx(uint32_t bufferNum, uint32_t posX, uint32_t posY, const char * buffer);
void OSScreenEnableEx(uint32_t bufferNum, int32_t enable);
void OSScreenPutPixelEx(uint32_t bufferNum, uint32_t posX, uint32_t posY, uint32_t color);

static void * overlayfunction_ptr __attribute__((section(".data"))) = NULL;

#ifdef __cplusplus
extern "C" {
#endif
void WUPS_InitOverlay(wups_loader_init_overlay_args_t args) {
    overlayfunction_ptr = (void*) args.overlayfunction_ptr;
}

void WUPS_Overlay_PrintTextOnScreen(wups_overlay_options_type_t screen, int x,int y, const char * msg, ...) {
    if(screen == WUPS_OVERLAY_NONE) {
        return;
    }

    char * tmp = NULL;

    va_list va;
    va_start(va, msg);
    if((vasprintf(&tmp, msg, va) >= 0) && tmp) {
        if(screen != WUPS_OVERLAY_DRC_ONLY) { // Draw TV if it's not DRC exclusive.
            OSScreenPutFontEx(0, x, y, tmp);
        }
        if(screen != WUPS_OVERLAY_TV_ONLY) { // Draw DRC if it's not TV exclusive.
            OSScreenPutFontEx(1, x, y, tmp);
        }
    }
    va_end(va);

    if(tmp) {
        free(tmp);
    }
}

void WUPS_Overlay_OSScreenClear(wups_overlay_options_type_t screen) {
    if(screen == WUPS_OVERLAY_NONE) {
        return;
    }
    if(screen != WUPS_OVERLAY_DRC_ONLY) { // Clear TV if it's not DRC exclusive.
        OSScreenClearBufferEx(0, 0);
    }
    if(screen != WUPS_OVERLAY_TV_ONLY) { // Clear DRC if it's not TV exclusive.
        OSScreenClearBufferEx(1, 0);
    }
}

void WUPS_Overlay_FlipBuffers(wups_overlay_options_type_t screen) {
    if(screen == WUPS_OVERLAY_NONE) {
        return;
    }
    if(screen != WUPS_OVERLAY_DRC_ONLY) { // Flip TV buffer if it's not DRC exclusive.
        OSScreenFlipBuffersEx(0);
    }
    if(screen != WUPS_OVERLAY_TV_ONLY) { // Flip DRC buffer if it's not TV exclusive.
        OSScreenFlipBuffersEx(1);
    }
}

void WUPS_OpenOverlay(wups_overlay_options_type_t screen, overlay_callback callback) {
    if(overlayfunction_ptr != NULL) {
        ( (void (*)(wups_overlay_options_type_t, overlay_callback))((unsigned int*)overlayfunction_ptr) )(screen,callback);
    }
}

#ifdef __cplusplus
}
#endif
