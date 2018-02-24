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

#ifndef _MEMORY_DEFS_H_
#define _MEMORY_DEFS_H_

#include <dynamic_libs/os_types.h>

#define MAX_USER_HEAP_INFOS         50
#define MAX_FRM_HEAP_INFOS          50
#define MAX_EXP_HEAP_INFOS          50
#define MAX_UNIT_HEAP_INFOS         50
#define MAX_ALLOCATOR_EXP_INFOS     50
#define MAX_BASE_HEAP_INFOS         50
#define MAX_BLOCK_HEAP_INFOS        50

typedef struct memHeapInfo_ {
    s32 handle;
    void * address;
    u32 size;
    u32 oldSize;
    u16 flags;
} memHeapInfo;

typedef struct memUnitHeapInfo_ {
    s32 handle;
    void * address;
    u32 heapSize;
    u32 memSize;
    u32 align;
    u16 flags;
} memUnitHeapInfo;

typedef struct baseHeapInfo_ {
    s32 oldHandle;
    s32 newHandle;
    s32 memArena;
} baseHeapInfo;

typedef struct blockHeapInfo_ {
    s32  handle;
    void *  tracking;
    void *  startAddress;
    void *  endAddress;
    void *  initTrackMem;
    u32 initTrackMemBytes;
    u16 flags;
} blockHeapInfo;

#endif
