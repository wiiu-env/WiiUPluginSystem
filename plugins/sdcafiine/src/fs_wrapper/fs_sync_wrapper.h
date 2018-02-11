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

#ifndef __FS_SYNC_WRAPPER_H_
#define __FS_SYNC_WRAPPER_H_

#include "FileReplacerUtils.h"

#include <dynamic_libs/fs_defs.h>
#include <dynamic_libs/fs_functions.h>
#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif

int fs_wrapper_FSCloseFile(int handle);

int fs_wrapper_FSGetPosFile(int handle,int * pos);

int fs_wrapper_FSGetStat(const char * path, FSStat * stats);

int fs_wrapper_FSGetStatFile(int handle, FSStat * stats);

int fs_wrapper_FSIsEof(int handle);

int fs_wrapper_FSOpenFile(const char * path, const char * mode, int * handle);

int fs_wrapper_FSReadFile(int handle,void *buffer,size_t size,size_t count);

int fs_wrapper_FSReadFileWithPos(void *buffer, size_t size, size_t count, u32 pos, int handle);

int fs_wrapper_FSSetPosFile(int handle,u32 pos);

#ifdef __cplusplus
}
#endif

#endif // __FS_SYNC_WRAPPER_H_
