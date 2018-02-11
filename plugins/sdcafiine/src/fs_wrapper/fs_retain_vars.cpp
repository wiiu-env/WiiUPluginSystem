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
#include <dynamic_libs/os_types.h>
#include "fs_retain_vars.h"

OSMessageQueue fsFSQueue __attribute__((section(".data")));
OSMessage fsFSQueueMessages[FS_QUEUE_MESSAGE_COUNT] __attribute__((section(".data")));

FSAsyncResult fsAsyncResultCache[ASYNC_RESULT_CACHE_SIZE] __attribute__((section(".data")));
u8 fsAsyncResultCacheLock __attribute__((section(".data"))) = 0;
u8 fsAsyncResultCacheCur __attribute__((section(".data"))) = 0;

u32 global_owner_id __attribute__((section(".data"))) = 0;
u32 global_group_id __attribute__((section(".data"))) = 0;
