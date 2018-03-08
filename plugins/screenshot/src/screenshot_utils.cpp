#include "screenshot_utils.h"
#include <fs/FSUtils.h>
#include <malloc.h>

bool UnormR8G8B8A8TogdImage(gdImagePtr *gdImgTmp, void *image_data, u32 width, u32 rows, u32 pitch) {
	*gdImgTmp = gdImageCreateTrueColor(width , rows );
    if(*gdImgTmp != NULL){
        R8G8B8A8_COLOR *buffer = (R8G8B8A8_COLOR *) image_data;
        R8G8B8A8_COLOR val;
        for (u32 row = 0; row < rows; ++row) {
            for (u32 x = 0; x < width; ++x) {
                val = buffer[row * pitch + x];
                gdImageSetPixel(*gdImgTmp, x , row , gdTrueColor(val.R, val.G, val.B));
            }
        }
        return true;
	}
	return false;
}

bool saveAsJPEG(const char * path, u8 * sourceBuffer, u32 width, u32 height, u32 pitch, u32 format) {
    if(path == NULL || sourceBuffer == NULL) {
        DEBUG_FUNCTION_LINE("path or buffer NULL\n");
        return false;
    }

    if((    format != GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_SRGB &&
            format != GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_UNORM)) {
        DEBUG_FUNCTION_LINE("Format not supported\n");
        return false;
    }

    gdImagePtr gdImagePtr = 0;
    bool result = false;

    if(!UnormR8G8B8A8TogdImage(&gdImagePtr, sourceBuffer, width, height, pitch)){
        DEBUG_FUNCTION_LINE("Setting up the GD buffer failed\n");
        if(gdImagePtr != NULL){
            gdImageDestroy(gdImagePtr);
        }
        return false;
    }

    int imd_size = 0;
    void *data = gdImageJpegPtr(gdImagePtr, &imd_size, 95);
    if (data) {
        DEBUG_FUNCTION_LINE("Encoded file as JPEG. size = %d.\n",imd_size);
        //FSUtils::CreateSubfolder("sd:/screenshots);
        result = FSUtils::saveBufferToFile(path,data,imd_size);
        if(!result){
            DEBUG_FUNCTION_LINE("Failed to save buffer to %s \n",path);
        }
    }
    gdFree(data);
    gdImageDestroy(gdImagePtr);
    return result;
}

bool copyBuffer(GX2ColorBuffer * sourceBuffer, GX2ColorBuffer * targetBuffer, u32 targetWidth, u32 targetHeight) {
    // Making sure the buffers are not NULL
    if (sourceBuffer != NULL && targetBuffer != NULL) {
        GX2InitColorBuffer(targetBuffer, GX2_SURFACE_DIM_2D, targetWidth, targetHeight, 1, GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_SRGB, GX2_AA_MODE_1X);

        // We need to override some values.
        targetBuffer->surface.tile =      GX2_TILE_MODE_LINEAR_ALIGNED;
        targetBuffer->surface.use =       GX2_SURFACE_USE_COLOR_BUFFER_TEXTURE;
        targetBuffer->view_mip = 0;
        targetBuffer->view_first_slice = 0;
        targetBuffer->view_slices_count = 1;
        GX2CalcSurfaceSizeAndAlignment(&targetBuffer->surface);
        GX2InitColorBufferRegs(targetBuffer);

        // Let's allocate the memory.
        targetBuffer->surface.image_data = memalign(targetBuffer->surface.align,targetBuffer->surface.image_size);
        if(targetBuffer->surface.image_data == NULL) {
            DEBUG_FUNCTION_LINE("failed to allocate memory.\n");
            return false;
        }
        DEBUG_FUNCTION_LINE("Allocated image data buffer. data %08X  size %08X \n",targetBuffer->surface.image_data,targetBuffer->surface.image_size);

        GX2Invalidate(GX2_INVALIDATE_CPU, targetBuffer->surface.image_data, targetBuffer->surface.image_size);
        if (sourceBuffer->surface.aa == GX2_AA_MODE_1X) {
            // If AA is disabled, we can simply use GX2CopySurface.
            GX2CopySurface(&sourceBuffer->surface,
                           sourceBuffer->view_mip,
                           sourceBuffer->view_first_slice,
                           &targetBuffer->surface, 0, 0);
        } else {
            // If AA is enabled, we need to resolve the AA buffer.
            GX2Surface tempSurface;
            tempSurface = sourceBuffer->surface;
            tempSurface.aa = GX2_AA_MODE_1X;
            GX2CalcSurfaceSizeAndAlignment(&tempSurface);

            tempSurface.image_data = memalign(tempSurface.align,tempSurface.image_size);
            if(tempSurface.image_data == NULL) {
                DEBUG_FUNCTION_LINE("failed to allocate data AA.\n");
                if(targetBuffer->surface.image_data != NULL) {
                    free(targetBuffer->surface.image_data);
                    targetBuffer->surface.image_data = NULL;
                }
                return false;
            }
            GX2ResolveAAColorBuffer(sourceBuffer,&tempSurface, 0, 0);
            GX2CopySurface(&tempSurface, 0, 0,&targetBuffer->surface, 0, 0);

            // Sync CPU and GPU
            GX2DrawDone();

            if(tempSurface.image_data != NULL) {
                free(tempSurface.image_data);
                tempSurface.image_data = NULL;
            }
        }
        return true;
    } else {
        DEBUG_FUNCTION_LINE("Couldn't copy buffer, pointer was NULL\n");
        return false;
    }
}


bool takeScreenshot(GX2ColorBuffer *srcBuffer,const char * path) {
    if(srcBuffer == NULL) {
        return false;
    }
    DEBUG_FUNCTION_LINE("Taking screenshot. %s\n",path);

    GX2ColorBuffer colorBuffer;
    GX2ColorBuffer * saveBuffer = NULL;

    // keep dimensions
    u32 width = srcBuffer->surface.width;
    u32 height = srcBuffer->surface.height;

    bool valid = false;
    bool cancel = false;
    bool low_memory = false;
    do {
        // At first we need to copy the buffer to fit our resolution.
        if(saveBuffer == NULL) {
            do {
                valid = copyBuffer(srcBuffer,&colorBuffer,width,height);
                // If the copying failed, we don't have enough memory. Let's decrease the resolution.
                if(!valid) {
                    low_memory = true;

                    if(height >= 1080) {
                        width = 1280;
                        height = 720;
                        DEBUG_FUNCTION_LINE("Switching to 720p.\n");
                    } else if(height >= 720) {
                        width = 854;
                        height = 480;
                        DEBUG_FUNCTION_LINE("Switching to 480p.\n");
                    } else if(height >= 480) {
                        width = 640;
                        height = 360;
                        DEBUG_FUNCTION_LINE("Switching to 360p.\n");
                    } else {
                        // Cancel the screenshot if the resolution would be too low.
                        cancel = true;
                        break;
                    }
                } else {
                    // On success save the pointer.
                    saveBuffer = &colorBuffer;
                }
            } while(!valid);
        }

        // Check if we should proceed
        if(cancel) {
            // Free the memory on error.
            if(colorBuffer.surface.image_data != NULL) {
                free(colorBuffer.surface.image_data);
                colorBuffer.surface.image_data = NULL;
            }
            return false;
        }

        // Flush out destinations caches
        GX2Invalidate(GX2_INVALIDATE_COLOR_BUFFER, colorBuffer.surface.image_data,colorBuffer.surface.image_size);

        // Wait for GPU to finish
        GX2DrawDone();

        DEBUG_FUNCTION_LINE("Trying to save.\n");

        // Trying to save as JPEG.
        valid = saveAsJPEG(path,(u8*) saveBuffer->surface.image_data,width,height,saveBuffer->surface.pitch,saveBuffer->surface.format);

        // Free the colorbuffer copy.
        if(colorBuffer.surface.image_data != NULL) {
            free(colorBuffer.surface.image_data);
            colorBuffer.surface.image_data = NULL;
            saveBuffer = NULL;
        }

        // When taking the screenshot failed, decrease the resolution again ~.
        if(!valid) {
            low_memory = true;
            if(height >= 1080) {
                width = 1280;
                height = 720;
                DEBUG_FUNCTION_LINE("Switching to 720p.\n");
            } else if(height >= 720) {
                width = 854;
                height = 480;
                DEBUG_FUNCTION_LINE("Switching to 480p.\n");
            } else if(height >= 480) {
                width = 640;
                height = 360;
                DEBUG_FUNCTION_LINE("Switching to 360p.\n");
            } else {
                return false;
            }
        }
    } while(!valid);

    return true;
}
