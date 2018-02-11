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

#include <utils/logger.h>

#include "fs_default_os_wrapper.h"

int fs_default_os_wrapper_FSCloseFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int fd, int error, FSAsyncParams * asyncParams, void * realAddress){
    FallbackParamWrapper  paramWrapper;
    memset(&paramWrapper,0,sizeof(FallbackParamWrapper));

    CustomAsyncParam  params;
    memset(&params,0,sizeof(CustomAsyncParam));

    paramWrapper.params.pClient = pClient;
    paramWrapper.params.pCmd = pCmd;
    paramWrapper.params.handle = fd;
    paramWrapper.params.error = error;
    paramWrapper.params.asyncParams = asyncParams;

    paramWrapper.realFunctionAddress = (void*) realAddress;

    params.params = paramWrapper.params;

    return fs_wrapper_FSCloseFileAsyncEx(params,&fallbackFSCloseFileAsync,&paramWrapper);
}

int fs_default_os_wrapper_FSGetPosFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int fd, int *pos, int error, FSAsyncParams * asyncParams, void * realAddress){
    FallbackParamWrapper  paramWrapper;
    memset(&paramWrapper,0,sizeof(FallbackParamWrapper));

    CustomAsyncParam  params;
    memset(&params,0,sizeof(CustomAsyncParam));

    paramWrapper.params.pClient = pClient;
    paramWrapper.params.pCmd = pCmd;
    paramWrapper.params.handle = fd;
    paramWrapper.params.posPtr = pos;
    paramWrapper.params.error = error;
    paramWrapper.params.asyncParams = asyncParams;

    paramWrapper.realFunctionAddress = (void*) realAddress;

    params.params = paramWrapper.params;

    return fs_wrapper_FSGetPosFileAsyncEx(params,fallbackFSGetPosFileAsync,&paramWrapper);
}

int fs_default_os_wrapper_FSGetStatAsync(const char *libPath, FSClient *pClient, FSCmdBlock *pCmd, const char *path, FSStat *stats, int error, FSAsyncParams * asyncParams, void * realAddress){
    FallbackParamWrapper  paramWrapper;
    memset(&paramWrapper,0,sizeof(FallbackParamWrapper));

    CustomAsyncParam  params;
    memset(&params,0,sizeof(CustomAsyncParam));

    paramWrapper.params.pClient = pClient;
    paramWrapper.params.pCmd = pCmd;
    paramWrapper.params.path = (char*)path;
    paramWrapper.params.stats = stats;
    paramWrapper.params.error = error;
    paramWrapper.params.asyncParams = asyncParams;

    paramWrapper.realFunctionAddress = (void*) realAddress;

    params.params = paramWrapper.params;
    params.params.path = libPath; // Use modified path.
    params.needToFreePath = 1;

    return fs_wrapper_FSGetPosFileAsyncEx(params,fallbackFSGetStatAsync,&paramWrapper);
}

int fs_default_os_wrapper_FSGetStatFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int fd, FSStat * stats, int error, FSAsyncParams * asyncParams, void * realAddress){
    FallbackParamWrapper  paramWrapper;
    memset(&paramWrapper,0,sizeof(FallbackParamWrapper));

    CustomAsyncParam  params;
    memset(&params,0,sizeof(CustomAsyncParam));

    paramWrapper.params.pClient = pClient;
    paramWrapper.params.pCmd = pCmd;
    paramWrapper.params.handle = fd;
    paramWrapper.params.stats = stats;
    paramWrapper.params.error = error;
    paramWrapper.params.asyncParams = asyncParams;

    paramWrapper.realFunctionAddress = (void*) realAddress;

    params.params = paramWrapper.params;

    return fs_wrapper_FSGetStatFileAsyncEx(params,fallbackFSGetStatFileAsync,&paramWrapper);
}

int fs_default_os_wrapper_FSIsEofAsync(FSClient *pClient, FSCmdBlock *pCmd, int fd, int error, FSAsyncParams * asyncParams, void * realAddress){
    FallbackParamWrapper  paramWrapper;
    memset(&paramWrapper,0,sizeof(FallbackParamWrapper));

    CustomAsyncParam  params;
    memset(&params,0,sizeof(CustomAsyncParam));

    paramWrapper.params.pClient = pClient;
    paramWrapper.params.pCmd = pCmd;
    paramWrapper.params.handle = fd;
    paramWrapper.params.error = error;
    paramWrapper.params.asyncParams = asyncParams;

    paramWrapper.realFunctionAddress = (void*) realAddress;

    params.params = paramWrapper.params;

    return fs_wrapper_FSIsEofAsyncEx(params,fallbackFSIsEofAsync,&paramWrapper);
}

int fs_default_os_wrapper_FSOpenFileAsync(const char *libpath, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error, FSAsyncParams *asyncParams, void * realAddress){

    FallbackParamWrapper  paramWrapper;
    memset(&paramWrapper,0,sizeof(FallbackParamWrapper));

    CustomAsyncParam  params;
    memset(&params,0,sizeof(CustomAsyncParam));

    paramWrapper.params.pClient = pClient;
    paramWrapper.params.pCmd = pCmd;
    paramWrapper.params.path = path;
    paramWrapper.params.mode = mode;
    paramWrapper.params.handlePtr = handle;
    paramWrapper.params.error = error;
    paramWrapper.params.asyncParams = asyncParams;

    paramWrapper.realFunctionAddress = (void*) realAddress;

    params.params = paramWrapper.params;
    params.params.path = libpath; // Use modified path.
    params.needToFreePath = 1;

    return fs_wrapper_FSOpenFileAsyncEx(params,fallbackFSOpenFileAsync,&paramWrapper);
}

int fs_default_os_wrapper_FSReadFileAsync(FSClient *pClient, FSCmdBlock *pCmd, void *buffer, int size, int count, int fd, int flag, int error, FSAsyncParams *asyncParams, void * realAddress){
    FallbackParamWrapper  paramWrapper;
    memset(&paramWrapper,0,sizeof(FallbackParamWrapper));

    CustomAsyncParam  params;
    memset(&params,0,sizeof(CustomAsyncParam));

    paramWrapper.params.pClient = pClient;
    paramWrapper.params.pCmd = pCmd;
    paramWrapper.params.buffer = buffer;
    paramWrapper.params.size = size;
    paramWrapper.params.count = count;
    paramWrapper.params.handle = fd;
    paramWrapper.params.flag = flag;
    paramWrapper.params.error = error;
    paramWrapper.params.asyncParams = asyncParams;

    paramWrapper.realFunctionAddress = (void*) realAddress;

    params.params = paramWrapper.params;

    return fs_wrapper_FSReadFileAsyncEx(params,fallbackFSReadFileAsync,&paramWrapper);
}

int fs_default_os_wrapper_FSReadFileWithPosAsync(FSClient *pClient, FSCmdBlock *pCmd, void *buffer, int size, int count, u32 pos, int fd, int flag, int error, FSAsyncParams *asyncParams, void * realAddress){
    FallbackParamWrapper  paramWrapper;
    memset(&paramWrapper,0,sizeof(FallbackParamWrapper));

    CustomAsyncParam  params;
    memset(&params,0,sizeof(CustomAsyncParam));

    paramWrapper.params.pClient = pClient;
    paramWrapper.params.pCmd = pCmd;
    paramWrapper.params.buffer = buffer;
    paramWrapper.params.size = size;
    paramWrapper.params.count = count;
    paramWrapper.params.pos = pos;
    paramWrapper.params.handle = fd;
    paramWrapper.params.flag = flag;
    paramWrapper.params.error = error;
    paramWrapper.params.asyncParams = asyncParams;

    paramWrapper.realFunctionAddress = (void*) realAddress;

    params.params = paramWrapper.params;

    return fs_wrapper_FSReadFileWithPosAsyncEx(params,fallbackFSReadFileWithPosAsync,&paramWrapper);
}

int fs_default_os_wrapper_FSSetPosFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int handle, u32 pos, int error, FSAsyncParams *asyncParams, void * realAddress){
    FallbackParamWrapper  paramWrapper;
    memset(&paramWrapper,0,sizeof(FallbackParamWrapper));

    CustomAsyncParam  params;
    memset(&params,0,sizeof(CustomAsyncParam));

    paramWrapper.params.pClient = pClient;
    paramWrapper.params.pCmd = pCmd;
    paramWrapper.params.handle = handle;
    paramWrapper.params.pos = pos;
    paramWrapper.params.error = error;
    paramWrapper.params.asyncParams = asyncParams;

    paramWrapper.realFunctionAddress = (void*) realAddress;

    params.params = paramWrapper.params;

    return fs_wrapper_FSSetPosFileAsyncEx(params,fallbackFSSetPosFileAsync,&paramWrapper);
}
