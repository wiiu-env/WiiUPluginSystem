#include <wups.h>
#include <string.h>
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <dynamic_libs/sys_functions.h>
#include <dynamic_libs/proc_ui_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <fswrapper/FileReplacerUtils.h>
#include <fswrapper/fs_default_os_wrapper.h>
#include <myfs/fs_utils.h>
#include <utils/logger.h>
#include "common/retain_vars.h"
#include "common/common.h"
#include "main.h"

#define	DEBUG_LOG		0

DECL_FUNCTION(int, FSCloseFile, FSClient *pClient, FSCmdBlock *pCmd, int fd, int error) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || !(gUSBInitDone || gSDInitDone)){ return real_FSCloseFile(pClient, pCmd, fd, error); }

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSCloseFile(fd)) != USE_OS_FS_FUNCTION){
        return result;
    }

    return real_FSCloseFile(pClient, pCmd, fd, setErrorFlag(error));
}

DECL_FUNCTION(int, FSGetPosFile, FSClient *pClient, FSCmdBlock *pCmd, int fd, int *pos, int error) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || !(gUSBInitDone || gSDInitDone)){ return real_FSGetPosFile(pClient, pCmd, fd, pos, error); }

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSGetPosFile(fd,pos)) != USE_OS_FS_FUNCTION){
        return result;
    }

	return real_FSGetPosFile(pClient, pCmd, fd, pos, setErrorFlag(error));
}

DECL_FUNCTION(int, FSGetStat, FSClient *pClient, FSCmdBlock *pCmd, const char *path, FSStat *stats, int error) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || !(gUSBInitDone || gSDInitDone)){ return real_FSGetStat(pClient, pCmd, path, stats, error); }

    int result = USE_OS_FS_FUNCTION;

    if(DEBUG_LOG){ DEBUG_FUNCTION_LINE("for path %s\n",path); }
    char * newPath = getPathWithNewBase(path,gModFolder);
    if(newPath != NULL){
        if((result = fs_wrapper_FSGetStat(newPath,stats)) != USE_OS_FS_FUNCTION){
            if(newPath){ free(newPath); newPath = NULL;}
            return result;
        }
        if(newPath){ free(newPath); newPath = NULL;} //Should be dead code...
    }

    return real_FSGetStat(pClient, pCmd, path, stats, setErrorFlag(error));
}

DECL_FUNCTION(int, FSGetStatFile, FSClient *pClient, FSCmdBlock *pCmd, int fd, FSStat * stats, int error) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || !(gUSBInitDone || gSDInitDone)){ return real_FSGetStatFile(pClient, pCmd, fd, stats, error); }

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSGetStatFile(fd,stats)) != USE_OS_FS_FUNCTION){
        return result;
    }

    return real_FSGetStatFile(pClient, pCmd, fd, stats, setErrorFlag(error));
}

DECL_FUNCTION(int, FSIsEof, FSClient *pClient, FSCmdBlock *pCmd, int fd, int error) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || !(gUSBInitDone || gSDInitDone)) return real_FSIsEof(pClient, pCmd, fd, error);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSIsEof(fd)) != USE_OS_FS_FUNCTION){
        return result;
    }

    return real_FSIsEof(pClient, pCmd, fd, setErrorFlag(error));
}

DECL_FUNCTION(int, FSOpenFile, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || !(gUSBInitDone || gSDInitDone)){ return real_FSOpenFile(pClient, pCmd, path, mode, handle, error); }

    /*if(endsWith(path,BOOT_TV_TEX_TGA,-1,-1)){ //Mario Party 10 crashes when pressing the home button.
        if(startsWith("/vol/storage_mlc01/usr/title/",path)){
            u64 tID = getTitleIDFromPath(path);
            HandleMultiModPacks(tID,false);
        }else if(strlen(gLastMetaPath) > 0){
            DEBUG_FUNCTION_LINE("gLastMetaPath %s\n",gLastMetaPath);
            if(startsWith("/vol/storage_usb01/usr/title/",gLastMetaPath) ||
               startsWith("/vol/storage_mlc01/usr/title/",gLastMetaPath)
               ){
                u64 tID = getTitleIDFromPath(gLastMetaPath);
                HandleMultiModPacks(tID,false);
            }
        }
    }*/

    if(DEBUG_LOG){ DEBUG_FUNCTION_LINE("for path %s\n",path); }
    char * newPath = getPathWithNewBase(path,gModFolder);
    int result = USE_OS_FS_FUNCTION;

    if(newPath != NULL){
        if((result = fs_wrapper_FSOpenFile(newPath,mode,handle)) != USE_OS_FS_FUNCTION){
        if(newPath){ free(newPath); newPath = NULL;}
        return result;
        }
        if(newPath){ free(newPath); newPath = NULL;} //Should be dead code...
    }

    return real_FSOpenFile(pClient, pCmd, path, mode, handle, setErrorFlag(error));
}

DECL_FUNCTION(int, FSReadFile, FSClient *pClient, FSCmdBlock *pCmd, void *buffer, int size, int count, int handle, int flag, int error) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || !(gUSBInitDone || gSDInitDone)){ return real_FSReadFile(pClient, pCmd, buffer, size, count, handle, flag, error); }

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSReadFile(handle,buffer,size,count)) != USE_OS_FS_FUNCTION){
        return result;
    }

    return real_FSReadFile(pClient, pCmd, buffer, size, count, handle, flag, setErrorFlag(error));
}

DECL_FUNCTION(int, FSSetPosFile, FSClient *pClient, FSCmdBlock *pCmd, int fd, u32 pos, int error) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || !(gUSBInitDone || gSDInitDone)) return real_FSSetPosFile(pClient, pCmd, fd, pos, error);

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSSetPosFile(fd,pos)) != USE_OS_FS_FUNCTION){
        return result;
    }

	return real_FSSetPosFile(pClient, pCmd, fd, pos, setErrorFlag(error));
}

DECL_FUNCTION(int, FSReadFileWithPos, FSClient *pClient, FSCmdBlock *pCmd, void *buffer, int size, int count, u32 pos, int fd, int flag, int error) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || !(gUSBInitDone || gSDInitDone)){ return real_FSReadFileWithPos(pClient, pCmd, buffer, size, count, pos, fd, flag, error); }

    int result = USE_OS_FS_FUNCTION;
    if((result = fs_wrapper_FSReadFileWithPos(buffer,size,count,pos,fd)) != USE_OS_FS_FUNCTION){
        return result;
    }

    return real_FSReadFileWithPos(pClient, pCmd, buffer, size, count, pos, fd, flag, setErrorFlag(error));
}


/**
In theory it would be enough just to patch the "async" versions of the function.
The non-async functions internally use the async function anyway.
However this my be a bit faster/robust, when we handle the async functions async.
**/

DECL_FUNCTION(int, FSCloseFileAsync, FSClient *pClient, FSCmdBlock *pCmd, int fd, int error, FSAsyncParams * asyncParams) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || checkErrorFlag(&error) || !(gUSBInitDone || gSDInitDone)){ // Use the real implementation if the app is not in foreground or we already checked it.
        return real_FSCloseFileAsync(pClient, pCmd, fd, error, asyncParams);
    }

    return fs_default_os_wrapper_FSCloseFileAsync(pClient, pCmd, fd, error, asyncParams,(void*) real_FSCloseFileAsync);
}

DECL_FUNCTION(int, FSGetPosFileAsync, FSClient *pClient, FSCmdBlock *pCmd, int fd, int *pos, int error, FSAsyncParams * asyncParams) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || checkErrorFlag(&error) || !(gUSBInitDone || gSDInitDone)){ // Use the real implementation if the app is not in foreground or we already checked it.
        return real_FSGetPosFileAsync(pClient, pCmd, fd, pos, error, asyncParams);
    }

    return fs_default_os_wrapper_FSGetPosFileAsync(pClient, pCmd, fd, pos, error, asyncParams,(void*) real_FSGetPosFileAsync);
}

DECL_FUNCTION(int, FSGetStatAsync, FSClient *pClient, FSCmdBlock *pCmd, const char *path, FSStat *stats, int error, FSAsyncParams * asyncParams) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || checkErrorFlag(&error) || !(gUSBInitDone || gSDInitDone)){ // Use the real implementation if the app is not in foreground or we already checked it.
        return real_FSGetStatAsync(pClient, pCmd, path, stats, error, asyncParams);
    }

    return fs_default_os_wrapper_FSGetStatAsync(getPathWithNewBase((char*)path,gModFolder),pClient, pCmd, path, stats, error, asyncParams,(void*) real_FSGetStatAsync);
}

DECL_FUNCTION(int, FSGetStatFileAsync, FSClient *pClient, FSCmdBlock *pCmd, int fd, FSStat * stats, int error, FSAsyncParams * asyncParams) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || checkErrorFlag(&error) || !(gUSBInitDone || gSDInitDone)){ // Use the real implementation if the app is not in foreground or we already checked it.
        return real_FSGetStatFileAsync(pClient, pCmd, fd, stats, error, asyncParams);
    }

    return fs_default_os_wrapper_FSGetStatFileAsync(pClient, pCmd, fd, stats, error, asyncParams,(void*) real_FSGetStatFileAsync);
}

DECL_FUNCTION(int, FSIsEofAsync, FSClient *pClient, FSCmdBlock *pCmd, int fd, int error, FSAsyncParams *asyncParams) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || checkErrorFlag(&error) || !(gUSBInitDone || gSDInitDone)){ // Use the real implementation if the app is not in foreground or we already checked it.
        return real_FSIsEofAsync(pClient, pCmd, fd, error,asyncParams);
    }

    return fs_default_os_wrapper_FSIsEofAsync(pClient, pCmd, fd, error,asyncParams,(void*) real_FSIsEofAsync);
}
/*
u64 getTitleIDFromPath(const char * path){
    if(path == NULL || strlen(path) < 46) return 0;
    char titleID[0x11];
    char titleIDHigh[0x09];
    char titleIDLow[0x09];
    char * test = (char * )&path[29];
    snprintf(titleIDHigh,0x09,"%s",test);
    test = (char * ) &path[38];
    snprintf(titleIDLow,0x09,"%s",test);
    snprintf(titleID,0x11,"%s%s",titleIDHigh,titleIDLow);
    u64 tID = strtoll(titleID, NULL, 16);
    tID &= ~ 0x0000000E00000000; // remove update flag
    return tID;
}*/

DECL_FUNCTION(int, FSOpenFileAsync, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error, FSAsyncParams *asyncParams) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || checkErrorFlag(&error) || !(gUSBInitDone || gSDInitDone)){ // Use the real implementation if the app is not in foreground or we already checked it.
        return real_FSOpenFileAsync(pClient, pCmd, path, mode, handle,error, asyncParams);
    }

    return fs_default_os_wrapper_FSOpenFileAsync(getPathWithNewBase((char*)path,gModFolder), pClient, pCmd, path, mode, handle,error, asyncParams,(void*) real_FSOpenFileAsync);
}

DECL_FUNCTION(int, FSReadFileAsync, FSClient *pClient, FSCmdBlock *pCmd, void *buffer, int size, int count, int fd, int flag, int error, FSAsyncParams *asyncParams) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || checkErrorFlag(&error) || !(gUSBInitDone || gSDInitDone)){ // Use the real implementation if the app is not in foreground or we already checked it.
        return real_FSReadFileAsync(pClient, pCmd, buffer, size, count, fd, flag, error, asyncParams);
    }

    return fs_default_os_wrapper_FSReadFileAsync(pClient, pCmd, buffer, size, count, fd, flag, error, asyncParams, (void*) real_FSReadFileAsync);
}

DECL_FUNCTION(int, FSReadFileWithPosAsync, FSClient *pClient, FSCmdBlock *pCmd, void *buffer, int size, int count, u32 pos, int fd, int flag, int error, FSAsyncParams *asyncParams) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || checkErrorFlag(&error) || !(gUSBInitDone || gSDInitDone)){ // Use the real implementation if the app is not in foreground or we already checked it.
        return real_FSReadFileWithPosAsync(pClient, pCmd, buffer, size, count, pos, fd, flag, error, asyncParams);
    }

    return fs_default_os_wrapper_FSReadFileWithPosAsync(pClient, pCmd, buffer, size, count, pos, fd, flag, error, asyncParams, (void*) real_FSReadFileWithPosAsync);
}

DECL_FUNCTION(int, FSSetPosFileAsync, FSClient *pClient, FSCmdBlock *pCmd, int handle, u32 pos, int error, FSAsyncParams *asyncParams) {
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND || checkErrorFlag(&error) || !(gUSBInitDone || gSDInitDone)){ // Use the real implementation if the app is not in foreground or we already checked it.
        return real_FSSetPosFileAsync(pClient, pCmd, handle, pos, error,asyncParams);
    }

    return fs_default_os_wrapper_FSSetPosFileAsync(pClient, pCmd, handle, pos, error,asyncParams, (void*) real_FSSetPosFileAsync);
}

/*
DECL_FUNCTION(int, FSBindMount, void *pClient, void *pCmd, char *source, char *target, int error){
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND) return real_FSBindMount(pClient,pCmd,source,target,error);
    memcpy(gLastMetaPath,source,strlen(source) + 1);
    return real_FSBindMount(pClient,pCmd,source,target,error);
}
DECL_FUNCTION(int, FSBindUnmount, void *pClient, void *pCmd, char *target, int error){
    if(gAppStatus == WUPS_APP_STATUS_BACKGROUND) real_FSBindUnmount(pClient,pCmd,target,error);
    gLastMetaPath[0] = 0;
    return real_FSBindUnmount(pClient,pCmd,target,error);
}*/


WUPS_MUST_REPLACE(FSCloseFile,                 WUPS_LOADER_LIBRARY_COREINIT,  FSCloseFile);
WUPS_MUST_REPLACE(FSGetPosFile,                WUPS_LOADER_LIBRARY_COREINIT,  FSGetPosFile);
WUPS_MUST_REPLACE(FSGetStat,                   WUPS_LOADER_LIBRARY_COREINIT,  FSGetStat);
WUPS_MUST_REPLACE(FSGetStatFile,               WUPS_LOADER_LIBRARY_COREINIT,  FSGetStatFile);
WUPS_MUST_REPLACE(FSIsEof,                     WUPS_LOADER_LIBRARY_COREINIT,  FSIsEof);
WUPS_MUST_REPLACE(FSOpenFile,                  WUPS_LOADER_LIBRARY_COREINIT,  FSOpenFile);
WUPS_MUST_REPLACE(FSReadFile,                  WUPS_LOADER_LIBRARY_COREINIT,  FSReadFile);
WUPS_MUST_REPLACE(FSReadFileWithPos,           WUPS_LOADER_LIBRARY_COREINIT,  FSReadFileWithPos);
WUPS_MUST_REPLACE(FSSetPosFile,                WUPS_LOADER_LIBRARY_COREINIT,  FSSetPosFile);

WUPS_MUST_REPLACE(FSCloseFileAsync,            WUPS_LOADER_LIBRARY_COREINIT,  FSCloseFileAsync);
WUPS_MUST_REPLACE(FSGetPosFileAsync,           WUPS_LOADER_LIBRARY_COREINIT,  FSGetPosFileAsync);
WUPS_MUST_REPLACE(FSGetStatAsync,              WUPS_LOADER_LIBRARY_COREINIT,  FSGetStatAsync);
WUPS_MUST_REPLACE(FSGetStatFileAsync,          WUPS_LOADER_LIBRARY_COREINIT,  FSGetStatFileAsync);
WUPS_MUST_REPLACE(FSIsEofAsync,                WUPS_LOADER_LIBRARY_COREINIT,  FSIsEofAsync);
WUPS_MUST_REPLACE(FSOpenFileAsync,             WUPS_LOADER_LIBRARY_COREINIT,  FSOpenFileAsync);
WUPS_MUST_REPLACE(FSReadFileAsync,             WUPS_LOADER_LIBRARY_COREINIT,  FSReadFileAsync);
WUPS_MUST_REPLACE(FSReadFileWithPosAsync,      WUPS_LOADER_LIBRARY_COREINIT,  FSReadFileWithPosAsync);
WUPS_MUST_REPLACE(FSSetPosFileAsync,           WUPS_LOADER_LIBRARY_COREINIT,  FSSetPosFileAsync);