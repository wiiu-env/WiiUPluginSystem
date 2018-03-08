#ifndef _SCREENSHOT_UTILS_H_
#define _SCREENSHOT_UTILS_H_

#include <dynamic_libs/gx2_functions.h>
#include <gd.h>
#include <utils/logger.h>

typedef struct _R8G8B8A8_COLOR {
    u8 R, G, B, A;
} R8G8B8A8_COLOR;

bool UnormR8G8B8A8TogdImage(gdImagePtr *gdImgTmp, void *image_data, u32 width, u32 rows, u32 pitch);

bool saveAsJPEG(const char * path, u8 * sourceBuffer, u32 width, u32 height, u32 pitch, u32 format);

bool copyBuffer(GX2ColorBuffer * sourceBuffer, GX2ColorBuffer * targetBuffer, u32 targetWidth, u32 targetHeight);

bool takeScreenshot(GX2ColorBuffer *srcBuffer,const char * path);

#endif
