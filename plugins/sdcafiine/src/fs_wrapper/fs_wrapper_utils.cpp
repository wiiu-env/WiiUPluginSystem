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

#include "fs_wrapper_utils.h"
#include "fs_async_wrapper.h"

typedef int (*FallbackHelperInternal)(FallbackParamWrapper*, OSAsyncParamWrapper *);

static int fallbackCaller(FallbackHelperInternal _function, void * fallbackparams){
    FallbackParamWrapper * paramsWrapper = (FallbackParamWrapper *)fallbackparams;
    OSAsyncParamWrapper * params = &(paramsWrapper->params);

    if(paramsWrapper->realFunctionAddress == NULL){ return FS_STATUS_FATAL_ERROR; } // OSFatal("paramsWrapper->realFunctionAddress was NULL;");}
    return _function(paramsWrapper,params);
}

static int fallbackFSCloseFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int (*real_FSCloseFileAsyncCallback)(FSClient *, FSCmdBlock *, int, int, FSAsyncParams *);
    real_FSCloseFileAsyncCallback = (int(*)(FSClient *, FSCmdBlock *, int, int, FSAsyncParams *)) paramsWrapper->realFunctionAddress;
    int res = real_FSCloseFileAsyncCallback(params->pClient, params->pCmd, params->handle, params->error,params->asyncParams);
    return res;
}

static int fallbackFSGetPosFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int (*real_FSGetPosFileAsyncFunc)(FSClient *, FSCmdBlock *, int, int *, int, FSAsyncParams *);
    real_FSGetPosFileAsyncFunc = (int(*)(FSClient *, FSCmdBlock *, int, int *, int, FSAsyncParams *)) paramsWrapper->realFunctionAddress;
    int res = real_FSGetPosFileAsyncFunc(params->pClient, params->pCmd, params->handle, params->posPtr, params->error,params->asyncParams);
    return res;
}

static int fallbackFSGetStatAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int (*real_FSGetStatAsyncFunc)(FSClient *, FSCmdBlock *, const char *, FSStat *, int, FSAsyncParams *);
    real_FSGetStatAsyncFunc = (int(*)(FSClient *, FSCmdBlock *, const char *, FSStat *, int, FSAsyncParams *)) paramsWrapper->realFunctionAddress;
    int res = real_FSGetStatAsyncFunc(params->pClient, params->pCmd, params->path, params->stats, params->error,params->asyncParams);
    return res;
}

static int fallbackFSGetStatFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int (*real_FSGetStatFileAsyncFunc)(FSClient *, FSCmdBlock *, int, FSStat *, int, FSAsyncParams *);
    real_FSGetStatFileAsyncFunc = (int(*)(FSClient *, FSCmdBlock *, int, FSStat *, int, FSAsyncParams *)) paramsWrapper->realFunctionAddress;
    int res = real_FSGetStatFileAsyncFunc(params->pClient, params->pCmd, params->handle, params->stats, params->error,params->asyncParams);
    return res;
}

static int fallbackFSIsEofAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int (*real_FSIsEofAsyncFunc)(FSClient *, FSCmdBlock *, int, int, FSAsyncParams *);
    real_FSIsEofAsyncFunc = (int(*)(FSClient *, FSCmdBlock *, int, int, FSAsyncParams *)) paramsWrapper->realFunctionAddress;
    int res = real_FSIsEofAsyncFunc(params->pClient, params->pCmd, params->handle, params->error,params->asyncParams);
    return res;
}

static int fallbackFSOpenFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int (*real_FSOpenFileAsyncCallback)(FSClient *, FSCmdBlock *, const char *, const char *, int *, int, FSAsyncParams *);
    real_FSOpenFileAsyncCallback = (int(*)(FSClient *, FSCmdBlock *, const char *, const char *, int *, int, FSAsyncParams *)) paramsWrapper->realFunctionAddress;
    int res = real_FSOpenFileAsyncCallback(params->pClient, params->pCmd, params->path, params->mode, params->handlePtr, params->error, params->asyncParams);
    return res;
}

static int fallbackFSReadFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int (*real_FSReadFileAsyncFunc)(FSClient *, FSCmdBlock *, void *, int, int, int, int, int, FSAsyncParams *);
    real_FSReadFileAsyncFunc = (int(*)(FSClient *, FSCmdBlock *, void *, int, int, int, int, int, FSAsyncParams *)) paramsWrapper->realFunctionAddress;
    int res = real_FSReadFileAsyncFunc(params->pClient, params->pCmd, params->buffer, params->size, params->count, params->handle, params->flag, params->error, params->asyncParams);
    return res;
}

static int fallbackFSReadFileWithPosAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int (*real_FSReadFileWithPosAsyncFunc)(FSClient *, FSCmdBlock *, void *, int, int, u32, int, int, int, FSAsyncParams *);
    real_FSReadFileWithPosAsyncFunc = (int(*)(FSClient *, FSCmdBlock *, void *, int, int, u32, int, int, int, FSAsyncParams *)) paramsWrapper->realFunctionAddress;
    int res = real_FSReadFileWithPosAsyncFunc(params->pClient, params->pCmd, params->buffer, params->size, params->count, params->pos, params->handle, params->flag, params->error, params->asyncParams);
    return res;
}

static int fallbackFSSetPosFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int (*real_FSSetPosFileAsyncFunc)(FSClient *, FSCmdBlock *, int, u32, int, FSAsyncParams *);
    real_FSSetPosFileAsyncFunc = (int(*)(FSClient *, FSCmdBlock *, int, u32, int, FSAsyncParams *)) paramsWrapper->realFunctionAddress;
    int res = real_FSSetPosFileAsyncFunc(params->pClient, params->pCmd, params->handle, params->pos, params->error, params->asyncParams);
    return res;
}

#define DEFINE_FS_FALLBACK_CALLER(name) \
        int fallback##name(void * fallbackparams){\
            return fallbackCaller(fallback##name##Internal,fallbackparams);\
        }

DEFINE_FS_FALLBACK_CALLER(FSCloseFileAsync)
DEFINE_FS_FALLBACK_CALLER(FSGetStatAsync)
DEFINE_FS_FALLBACK_CALLER(FSGetStatFileAsync)
DEFINE_FS_FALLBACK_CALLER(FSGetPosFileAsync)
DEFINE_FS_FALLBACK_CALLER(FSIsEofAsync)
DEFINE_FS_FALLBACK_CALLER(FSOpenFileAsync)
DEFINE_FS_FALLBACK_CALLER(FSReadFileAsync)
DEFINE_FS_FALLBACK_CALLER(FSReadFileWithPosAsync)
DEFINE_FS_FALLBACK_CALLER(FSSetPosFileAsync)
