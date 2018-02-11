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

#ifndef __FS_ASYNC_WRAPPER_H_
#define __FS_ASYNC_WRAPPER_H_

#include "FileReplacerUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <dynamic_libs/fs_defs.h>
#include <dynamic_libs/fs_functions.h>
#include <unistd.h>

#include "fs_sync_wrapper.h"

typedef int (*FSAsyncFallback)(void * params);

typedef struct OSAsyncParamWrapper_{
    FSClient *pClient;
    FSCmdBlock *pCmd;
    const char *path;
    const char *mode;
    FSStat * stats;
    void * buffer;
    int handle;
    int size;
    int count;
    int pos;
    int flag;
    int *handlePtr;
    int *posPtr;
    int error;
    FSAsyncParams * asyncParams;
} OSAsyncParamWrapper;

typedef struct FallbackParamWrapper_{
    OSAsyncParamWrapper params;
    void * realFunctionAddress;
} FallbackParamWrapper;

typedef struct CustomAsyncParam_{
    OSAsyncParamWrapper params;
    bool needToFreePath;
} CustomAsyncParam;

typedef struct CustomAsyncParamWrapper_{
    CustomAsyncParam params;
    FSAsyncFallback fallbackFunction;
    void * fallbackParams;
} CustomAsyncParamWrapper;

typedef int (*FSAsyncCustomCallback)(CustomAsyncParamWrapper params);


#define DEFINE_FS_WRAPPER(name) \
        int fs_wrapper_##name##Ex(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams){\
            return fs_wrapper_async_template(params,fs_wrapper_##name##Callback,fallback,fallbackParams);\
        }\
        int fs_wrapper_##name(CustomAsyncParam params){ \
            return fs_wrapper_##name##Ex(params,NULL,NULL);\
        }

#define DECLARE_FS_WRAPPER(name) \
        int fs_wrapper_##name##Ex(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams);\
        int fs_wrapper_##name(CustomAsyncParam params);

    int fs_wrapper_FSCloseFileAsync(CustomAsyncParam params);

    int fs_wrapper_FSCloseFileAsyncEx(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams);

    int fs_wrapper_FSGetStatAsync(CustomAsyncParam params);

    int fs_wrapper_FSGetStatAsyncEx(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams);

    int fs_wrapper_FSGetStatFileAsync(CustomAsyncParam params);

    int fs_wrapper_FSGetStatFileAsyncEx(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams);

    int fs_wrapper_FSGetPosFileAsync(CustomAsyncParam params);

    int fs_wrapper_FSGetPosFileAsyncEx(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams);

    int fs_wrapper_FSIsEofAsync(CustomAsyncParam params);

    int fs_wrapper_FSIsEofAsyncEx(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams);

    int fs_wrapper_FSOpenFileAsync(CustomAsyncParam params);

    int fs_wrapper_FSOpenFileAsyncEx(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams);

    int fs_wrapper_FSReadFileAsync(CustomAsyncParam params);

    int fs_wrapper_FSReadFileAsyncEx(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams);

    int fs_wrapper_FSReadFileWithPosAsync(CustomAsyncParam params);

    int fs_wrapper_FSReadFileWithPosAsyncEx(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams);

    int fs_wrapper_FSSetPosFileAsync(CustomAsyncParam params);

    int fs_wrapper_FSSetPosFileAsyncEx(CustomAsyncParam params, FSAsyncFallback fallback, void * fallbackParams);

#ifdef __cplusplus
}
#endif

#endif // __FS_ASYNC_WRAPPER_H_
