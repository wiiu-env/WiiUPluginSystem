/****************************************************************************
 * Copyright (C) 2017 Maschell
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

#ifndef __FS_DEFAULT_OS_WRAPPER_H_
#define __FS_DEFAULT_OS_WRAPPER_H_

#include "fs_async_wrapper.h"
#include "fs_sync_wrapper.h"
#include "fs_wrapper_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

int fs_default_os_wrapper_FSCloseFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int fd, int error, FSAsyncParams * asyncParams, void * realAddress);
int fs_default_os_wrapper_FSGetPosFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int fd, int *pos, int error, FSAsyncParams * asyncParams, void * realAddress);
int fs_default_os_wrapper_FSGetStatAsync(const char *libpath, FSClient *pClient, FSCmdBlock *pCmd, const char *path, FSStat *stats, int error, FSAsyncParams * asyncParams, void * realAddress);
int fs_default_os_wrapper_FSGetStatFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int fd, FSStat * stats, int error, FSAsyncParams * asyncParams, void * realAddress);
int fs_default_os_wrapper_FSIsEofAsync(FSClient *pClient, FSCmdBlock *pCmd, int fd, int error, FSAsyncParams * asyncParams, void * realAddress);
int fs_default_os_wrapper_FSOpenFileAsync(const char *libpath, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error, FSAsyncParams *asyncParams, void * realAddress);
int fs_default_os_wrapper_FSReadFileAsync(FSClient *pClient, FSCmdBlock *pCmd, void *buffer, int size, int count, int fd, int flag, int error, FSAsyncParams *asyncParams, void * realAddress);
int fs_default_os_wrapper_FSReadFileWithPosAsync(FSClient *pClient, FSCmdBlock *pCmd, void *buffer, int size, int count, u32 pos, int fd, int flag, int error, FSAsyncParams *asyncParams, void * realAddress);
int fs_default_os_wrapper_FSSetPosFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int handle, u32 pos, int error, FSAsyncParams *asyncParams, void * realAddress);

#ifdef __cplusplus
}
#endif

#endif // __FS_DEFAULT_OS_WRAPPER_H_
