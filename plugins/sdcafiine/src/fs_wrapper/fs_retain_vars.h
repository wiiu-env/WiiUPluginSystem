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

#ifndef FS_RETAINS_VARS_H_
#define FS_RETAINS_VARS_H_

#include <dynamic_libs/fs_defs.h>
#include <dynamic_libs/os_types.h>

#define ASYNC_RESULT_CACHE_SIZE     50
#define FS_QUEUE_MESSAGE_COUNT      5

extern OSMessageQueue fsFSQueue __attribute__((section(".data")));
extern OSMessage fsFSQueueMessages[FS_QUEUE_MESSAGE_COUNT] __attribute__((section(".data")));

extern FSAsyncResult fsAsyncResultCache[ASYNC_RESULT_CACHE_SIZE];

extern u8 fsAsyncResultCacheLock;
extern u8 fsAsyncResultCacheCur;

extern u32 global_owner_id;
extern u32 global_group_id;

#endif // FS_RETAINS_VARS_H_
