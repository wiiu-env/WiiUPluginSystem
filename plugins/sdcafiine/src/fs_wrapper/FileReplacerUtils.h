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

#ifndef __FILE_REPLACER_UTILS_H_
#define __FILE_REPLACER_UTILS_H_

#include <dynamic_libs/fs_defs.h>
#include <dynamic_libs/os_types.h>
#include <system/CThread.h>
#include "utils/vc_vector.h"

#include "fs_retain_vars.h"

#define CHECKED_WITH_ALL_ERRORS     0x10000
#define CHECKED_MASK                0x01000

#define FS_WRAPPER_DEBUG_LOG        0

#define USE_OS_FS_FUNCTION          -1337


/**
Returns a modified error flag.
This will be used to save the information if a file/handle was already
tried to be patched.
The non-async function internally call the async functions, and this way
we avoid testing it twice.
If the result contains our mask, we just straight to the OS functions.
**/
int setErrorFlag(int error);

/**
Check if we already checked the file/handle.
Returns true if it was already checked (+ revert the error)
Return false if it should be (tried) to be patched.
**/
int checkErrorFlag(int * error);

void addFileHandleInternal(int handle);

void removeFileHandleInternal(int handle);

bool hasFileHandleInternal(int handle);

void addDirHandleInternal(int handle);

void removeDirHandleInternal(int handle);

bool hasDirHandleInternal(int handle);
#endif // __FILE_REPLACER_UTILS_H_
