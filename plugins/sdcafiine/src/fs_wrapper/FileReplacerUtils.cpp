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
#include "FileReplacerUtils.h"
#include "fs_async_wrapper.h"
#include "common/retain_vars.h"

OSMessageQueue lfFSQueue __attribute__((section(".data")));
OSMessage lfFSQueueMessages[FS_QUEUE_MESSAGE_COUNT] __attribute__((section(".data")));

FSAsyncResult lfAsyncResultCache[ASYNC_RESULT_CACHE_SIZE] __attribute__((section(".data")));
u8 lfAsyncResultCacheLock __attribute__((section(".data"))) = 0;
u8 lfAsyncResultCacheCur __attribute__((section(".data"))) = 0;

int setErrorFlag(int error){
    int result = error;
    if(error == -1){
        result = CHECKED_WITH_ALL_ERRORS;
    }else{
        result |= CHECKED_MASK;
    }
    return result;
}

int checkErrorFlag(int * error){
    if(*error == CHECKED_WITH_ALL_ERRORS){
        *error = -1;
        return true;
    }else if ((*error & CHECKED_MASK) == CHECKED_MASK){
        *error &= ~CHECKED_MASK;
        return true;
    }
    return false;
}


void addFileHandleInternal(int handle){
    vc_vector_push_back(g_filehandles, &handle);
}

void removeFileHandleInternal(int handle){
    int index = 0;
    for (void* i = vc_vector_begin(g_filehandles);i != vc_vector_end(g_filehandles); i = vc_vector_next(g_filehandles, i)) {
        int val = *(int*)i;
        if(val == handle){
            vc_vector_erase(g_filehandles,index);
            return;
        }
        index++;
    }
}

bool hasFileHandleInternal(int handle){
    for (void* i = vc_vector_begin(g_filehandles);i != vc_vector_end(g_filehandles); i = vc_vector_next(g_filehandles, i)) {
        if(*(int*)i == handle){
            return true;
        }
    }
    return false;
}

void addDirHandleInternal(int handle){
    vc_vector_push_back(g_dirhandles, &handle);
}

void removeDirHandleInternal(int handle){
    int index = 0;
    for (void* i = vc_vector_begin(g_dirhandles);i != vc_vector_end(g_dirhandles); i = vc_vector_next(g_dirhandles, i)) {
        int val = *(int*)i;
        if(val == handle){
            vc_vector_erase(g_dirhandles,index);
            return;
        }
        index++;
    }
}

bool hasDirHandleInternal(int handle){
    for (void* i = vc_vector_begin(g_dirhandles);i != vc_vector_end(g_dirhandles); i = vc_vector_next(g_dirhandles, i)) {
        if(*(int*)i == handle){
            return true;
        }
    }
    return false;
}
