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

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utils/logger.h>

#include "fs_async_wrapper.h"

//Wii U fails to allocate memory if we do the functions async. =/
#define DO_REAL_ASYNC       0

static int doFallback(CustomAsyncParamWrapper params){
    if(params.fallbackFunction != NULL){
        if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Calling the fallback function %08X\n",params.fallbackFunction); }
        return ((FSAsyncFallback)params.fallbackFunction)(params.fallbackParams);
    }

    if(params.params.needToFreePath){
        free((void*)params.params.params.path);
    }

    DEBUG_FUNCTION_LINE("No fallback function provided\n");
    //OSFatal("No fallback given."); //We don't need to free stuff, because we'll (want to) crash anyway.

    return FS_STATUS_FATAL_ERROR;
}

static int sendAsyncResult(CustomAsyncParamWrapper in, int result){
    //FileReplacerUtils::sendAsyncCommand(in.params.params.pClient, in.params.params.pCmd, in.params.params.asyncParams, result);

    if(in.params.needToFreePath){
        free((void*)in.params.params.path);
    }

    return FS_STATUS_OK;
}

static int fs_wrapper_async_template(CustomAsyncParam params,FSAsyncCustomCallback callback, FSAsyncFallback fallback, void * fallbackParams){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called!\n"); }

    CustomAsyncParamWrapper wrapper;
    memset(&wrapper,0,sizeof(CustomAsyncParamWrapper));

    wrapper.params = params;

    wrapper.fallbackFunction = fallback;
    wrapper.fallbackParams = (void*)fallbackParams;

    if(!DO_REAL_ASYNC){
        return callback(wrapper);
    }

    //TODO:!

    /*OSMessage message;
    message.message = (u32) callback;
    message.data0 = (u32) &wrapper;


    //FileReplacerUtils::addFSQueueMSG(&message);*/

    return FS_STATUS_OK;
}

int fs_wrapper_FSCloseFileAsyncCallback(CustomAsyncParamWrapper params){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! params: %08X \n",params); }
    //if(params == NULL){ DEBUG_FUNCTION_LINE("!!!WARNING: Given parameter was NULL\n"); }

    OSAsyncParamWrapper * p = &(params.params.params);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSCloseFile(p->handle)) != USE_OS_FS_FUNCTION){
        return sendAsyncResult(params,result);
    }

    return doFallback(params);
}

int fs_wrapper_FSGetPosFileAsyncCallback(CustomAsyncParamWrapper params){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! params: %08X \n",params); }
    //if(params == NULL){ DEBUG_FUNCTION_LINE("!!!WARNING: Given parameter was NULL\n"); }

    OSAsyncParamWrapper * p = &(params.params.params);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSGetPosFile(p->handle,p->posPtr)) != USE_OS_FS_FUNCTION){
        return sendAsyncResult(params,result);
    }

    return doFallback(params);
}

int fs_wrapper_FSGetStatAsyncCallback(CustomAsyncParamWrapper params){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! params: %08X \n",params); }
    //if(params == NULL){ DEBUG_FUNCTION_LINE("!!!WARNING: Given parameter was NULL\n"); }

    OSAsyncParamWrapper * p = &(params.params.params);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSGetStat(p->path,p->stats)) != USE_OS_FS_FUNCTION){
        return sendAsyncResult(params,result);
    }

    return doFallback(params);
}

int fs_wrapper_FSGetStatFileAsyncCallback(CustomAsyncParamWrapper params){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! params: %08X \n",params); }
    //if(params == NULL){ DEBUG_FUNCTION_LINE("!!!WARNING: Given parameter was NULL\n"); }

    OSAsyncParamWrapper * p = &(params.params.params);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSGetStatFile(p->handle,p->stats)) != USE_OS_FS_FUNCTION){
        return sendAsyncResult(params,result);
    }

    return doFallback(params);
}

int fs_wrapper_FSIsEofAsyncCallback(CustomAsyncParamWrapper params){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! params: %08X \n",params); }
    //if(params == NULL){ DEBUG_FUNCTION_LINE("!!!WARNING: Given parameter was NULL\n"); }

    OSAsyncParamWrapper * p = &(params.params.params);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSIsEof(p->handle)) != USE_OS_FS_FUNCTION){
       return sendAsyncResult(params,result);
    }

    return doFallback(params);
}

int fs_wrapper_FSOpenFileAsyncCallback(CustomAsyncParamWrapper params){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! params: %08X \n",params); }
    //if(params == NULL){ DEBUG_FUNCTION_LINE("!!!WARNING: Given parameter was NULL\n"); }

    OSAsyncParamWrapper * p = &(params.params.params);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSOpenFile(p->path,p->mode,p->handlePtr)) != USE_OS_FS_FUNCTION){
        return sendAsyncResult(params,result);
    }

    return doFallback(params);
}

int fs_wrapper_FSReadFileAsyncCallback(CustomAsyncParamWrapper params){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! params: %08X \n",params); }
    //if(params == NULL){ DEBUG_FUNCTION_LINE("!!!WARNING: Given parameter was NULL\n"); }

    OSAsyncParamWrapper * p = &(params.params.params);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSReadFile(p->handle,p->buffer,p->size,p->count)) != USE_OS_FS_FUNCTION){
       return sendAsyncResult(params,result);
    }

    return doFallback(params);
}

int fs_wrapper_FSReadFileWithPosAsyncCallback(CustomAsyncParamWrapper params){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! params: %08X \n",params); }
    //if(params == NULL){ DEBUG_FUNCTION_LINE("!!!WARNING: Given parameter was NULL\n"); }

    OSAsyncParamWrapper * p = &(params.params.params);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSReadFileWithPos(p->buffer,p->size,p->count,p->pos,p->handle)) != USE_OS_FS_FUNCTION){
        return sendAsyncResult(params,result);
    }

    return doFallback(params);
}

int fs_wrapper_FSSetPosFileAsyncCallback(CustomAsyncParamWrapper params){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! params: %08X \n",params); }
    //if(params == NULL){ DEBUG_FUNCTION_LINE("!!!WARNING: Given parameter was NULL\n"); }

    OSAsyncParamWrapper * p = &(params.params.params);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSSetPosFile(p->handle,p->pos)) != USE_OS_FS_FUNCTION){
        return sendAsyncResult(params,result);
    }

    return doFallback(params);
}

DEFINE_FS_WRAPPER(FSCloseFileAsync)
DEFINE_FS_WRAPPER(FSGetStatAsync)
DEFINE_FS_WRAPPER(FSGetStatFileAsync)
DEFINE_FS_WRAPPER(FSGetPosFileAsync)
DEFINE_FS_WRAPPER(FSIsEofAsync)
DEFINE_FS_WRAPPER(FSOpenFileAsync)
DEFINE_FS_WRAPPER(FSReadFileAsync)
DEFINE_FS_WRAPPER(FSReadFileWithPosAsync)
DEFINE_FS_WRAPPER(FSSetPosFileAsync)

