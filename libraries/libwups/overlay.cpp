
#define _GNU_SOURCE
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

#ifdef __cplusplus
extern "C" {
#endif


void OSScreenInit(void);
void OSScreenShutdown(void);
uint32_t OSScreenGetBufferSizeEx(uint32_t bufferNum);
void OSScreenSetBufferEx(uint32_t bufferNum, void * addr);
void OSScreenClearBufferEx(uint32_t bufferNum, uint32_t temp);
void OSScreenFlipBuffersEx(uint32_t bufferNum);
void OSScreenPutFontEx(uint32_t bufferNum, uint32_t posX, uint32_t posY, const char * buffer);
void OSScreenEnableEx(uint32_t bufferNum, int32_t enable);
void OSScreenPutPixelEx(uint32_t bufferNum, uint32_t posX, uint32_t posY, uint32_t color);

static OverlayOpenFunction overlayfunction_ptr __attribute__((section(".data"))) = NULL;
static ConvertTextureFunction textureconvertfunction_ptr __attribute__((section(".data"))) = NULL;
static DrawTextureFunction drawtexturefunction_ptr __attribute__((section(".data"))) = NULL;

void WUPS_InitOverlay(wups_loader_init_overlay_args_t args) {
    overlayfunction_ptr = args.overlayfunction_ptr;
    textureconvertfunction_ptr = args.textureconvertfunction_ptr;
    drawtexturefunction_ptr = args.drawtexturefunction_ptr;
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

void WUPS_OpenOverlay(wups_overlay_options_type_t screen, overlay_callback callback, void* arg) {
    if(overlayfunction_ptr != NULL) {
        overlayfunction_ptr(screen, callback, arg);
    }
}

bool WUPS_ConvertImageToTexture(const uint8_t *img, int32_t imgSize, void * texture) {
    if(textureconvertfunction_ptr != NULL) {
        return textureconvertfunction_ptr(img, imgSize, texture);
    }
    return false;
}

void WUPS_DrawTexture(void * texture, void * sampler, float x, float y, int32_t width, int32_t height, float alpha = 1.0f) {
    if(drawtexturefunction_ptr != NULL) {
        drawtexturefunction_ptr(texture, sampler, x, y, width, height, alpha);
    }
}

#ifdef __cplusplus
}
#endif
