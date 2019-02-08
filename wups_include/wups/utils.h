/****************************************************************************
 * Copyright (C) 2018 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#ifndef WUPS_UTILS_DEF_H_
#define WUPS_UTILS_DEF_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum wups_overlay_options_type_t {
    WUPS_OVERLAY_NONE,
    WUPS_OVERLAY_DRC_ONLY,                          /* Tries to display only on gamepad screen */
    WUPS_OVERLAY_TV_ONLY,                           /* Tries to display only on tv screen */
    WUPS_OVERLAY_DRC_AND_TV,                        /* Tries to display on both screens. Prioritizes the TV screen if memory is low. */
    WUPS_OVERLAY_DRC_AND_TV_WITH_DRC_PRIO           /* Tries to display on both screens. But if memory is low, prioritize the DRC screen.*/
}
wups_overlay_options_type_t;

typedef void (*overlay_callback)(wups_overlay_options_type_t, void*);

typedef void (*OverlayOpenFunction)(wups_overlay_options_type_t screen, overlay_callback callback, void*);
typedef bool (*ConvertTextureFunction)(const uint8_t *img, int32_t imgSize, void * texture);

typedef struct wups_loader_init_overlay_args_t {
    OverlayOpenFunction overlayfunction_ptr;
    ConvertTextureFunction textureconvertfunction_ptr;
} wups_loader_init_overlay_args_t;

typedef int             (*OpenFunction)     (const char *pathname, int flags);
typedef ssize_t         (*WriteFunction)    (int fd, const void *buf, size_t count);
typedef int             (*CloseFunction)    (int fd);
typedef ssize_t         (*ReadFunction)     (int fd, void *buf, size_t count);
typedef off_t           (*LSeekFunction)    (int fd, off_t offset, int whence);
typedef int             (*StatFunction)     (const char *pathname, struct stat *statbuf);
typedef int             (*FStatFunction)    (int fd, struct stat *statbuf);
typedef DIR*            (*OpenDirFunction)  (const char * arg);
typedef int             (*CloseDirFunction) (DIR *dirp);
typedef struct dirent * (*ReadDirFunction)  (DIR *dirp);
typedef int             (*MKDirFunction)    (const char *path, mode_t mode);

typedef struct wups_loader_init_fs_args_t {
    OpenFunction open_repl;
    CloseFunction close_repl;
    WriteFunction write_repl;
    ReadFunction read_repl;
    LSeekFunction lseek_repl;
    StatFunction stat_repl;
    FStatFunction fstat_repl;
    OpenDirFunction opendir_repl;
    CloseDirFunction closedir_repl;
    ReadDirFunction readdir_repl;
    MKDirFunction mkdir_repl;
} wups_loader_init_fs_args_t;

typedef uint32_t (*KernelReadFunction)(const void *addr);
typedef void     (*KernelWriteFunction)(void *addr, uint32_t value);
typedef void     (*KernelCopyDataFunction)(uint32_t addr, uint32_t src, uint32_t len);

typedef struct wups_loader_init_kernel_args_t_ {
    KernelReadFunction kern_read_ptr;
    KernelWriteFunction kern_write_ptr;
    KernelCopyDataFunction kern_copy_data_ptr;
} wups_loader_init_kernel_args_t;

typedef void* (*VideoMemoryAllocFunction)(uint32_t size, int32_t align);
typedef void  (*VideoMemoryFreeFunction)(void *addr);

typedef struct wups_loader_init_vid_mem_args_t_ {
    VideoMemoryAllocFunction vid_mem_alloc_ptr;
    VideoMemoryFreeFunction vid_mem_free_ptr;
} wups_loader_init_vid_mem_args_t;

/*
    Gets called by the framework
*/
/**
    Sets the pointer for wrapping the fs functions.
    If NULL pointers are provided, the original function will be called.
    The whole point of replacing the fs functions is to inherit SD/USB access. 
    The argument of the ON_APPLICATION_START hook provides information on the state of SD or USB access.
**/
void WUPS_InitFS(wups_loader_init_fs_args_t args);

/**
    Sets the function pointer for opening the overlay.
    If none or a NULL pointer is provided, calling "WUPS_OpenOverlay" has no effect.
**/
void WUPS_InitOverlay(wups_loader_init_overlay_args_t args);

/**
    Sets the function pointers for kernel functions.
    If none or NULL pointers is provided, calling the corresponding function has no effect.
**/
void WUPS_InitKernel(wups_loader_init_kernel_args_t args);

/**
    Sets the function pointers for video mem functions.
    If none or NULL pointers is provided, calling the corresponding function has no effect.
**/
void WUPS_InitVidMem(wups_loader_init_vid_mem_args_t args);

/*
    Can be called by the user.
*/
void WUPS_Overlay_PrintTextOnScreen(wups_overlay_options_type_t screen, int x,int y, const char * msg, ...);

void WUPS_Overlay_OSScreenClear(wups_overlay_options_type_t screen);

void WUPS_Overlay_FlipBuffers(wups_overlay_options_type_t screen);

void WUPS_OpenOverlay(wups_overlay_options_type_t screen, overlay_callback callback, void* args);

// texture needs to be a GX2Texture
bool WUPS_ConvertImageToTexture(const uint8_t *img, int32_t imgSize, void * texture);

/**
    Reads a 32bit value from a given address with kernel rights.
    This function only has an effect if the plugin has the "WUPS_ALLOW_KERNEL" hook and the loader is NOT blocking the kernel access.
    The argument of the ON_APPLICATION_START hook provides the information if the plugin has kernel access which should be checked before using/relying on this function.
**/
uint32_t WUPS_KernelRead(const void *addr);

/**
    Write a 32bit value from a given address with kernel rights.
    This function only has an effect if the plugin has the "WUPS_ALLOW_KERNEL" hook and the loader is NOT blocking the kernel access.
    The argument of the ON_APPLICATION_START hook provides the information if the plugin has kernel access which should be checked before using/relying on this function.
**/
void WUPS_KernelWrite(void *addr, uint32_t value);

/**
    Copies data from a source address to a destination address for a given lenght with kernel rights.
    This function only has an effect if the plugin has the "WUPS_ALLOW_KERNEL" hook and the loader is NOT blocking the kernel access.
    The argument of the ON_APPLICATION_START hook provides the information if the plugin has kernel access which should be checked before using/relying on this function.
**/
void WUPS_KernelCopyDataFunction(uint32_t addr, uint32_t src, uint32_t len);

void * WUPS_VideoMemAlloc(uint32_t size);

void * WUPS_VideoMemMemalign(uint32_t size, int32_t align);

void WUPS_VideoMemFree(void *addr);

#ifdef __cplusplus
}
#endif

#endif /* WUPS_WUPS_H_ */
