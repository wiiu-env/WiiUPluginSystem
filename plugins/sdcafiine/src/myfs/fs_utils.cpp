#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <utils/logger.h>
#include <utils/StringTools.h>

#include "fs_utils.h"
#include "common/common.h"
#include "common/retain_vars.h"

int is_gamefile(const char *path) {
    if(path == NULL) return 0;
    if(strncmp(path,"CAFE/",5) == 0) return GAME_PATH_TYPE_STRIPPED_CONTENT;    //Workaround for NSMBU

    // In case the path starts by "//" and not "/" (some games do that ... ...)
    if (path[0] == '/' && path[1] == '/')
        path = &path[1];

    // In case the path does not start with "/" (some games do that too ...)
    int len = 0;
    char new_path[16];
    if(path[0] != '/') {
        new_path[0] = '/';
        len++;
    }

    while(*path && len < (int)(sizeof(new_path) - 1)) {
        new_path[len++] = *path++;
    }
    new_path[len++] = 0;

    if(strncmp(new_path,"/vol/content",12) == 0) return GAME_PATH_TYPE_CONTENT;
    //dlc support
    if(strncmp(new_path,"/vol/aoc",8) == 0) return GAME_PATH_TYPE_AOC;

    return 0;
}
char * getPathWithNewBase(const char * inPath, const char * newBase){
    if(inPath == NULL || *inPath == 0 || newBase == NULL || *newBase == 0) return NULL;
    char * relativePath = getRelativePath(inPath);
    char * result = NULL;

    if(relativePath == NULL){
        /*
        if(strlen(gModFolder) > 0){
            int file = 0;
            if(endsWith(inPath,BOOT_TV_TEX_TGA,-1,-1)){ file = 1;}
            else if(endsWith(inPath,BOOT_DRC_TEX_TGA,-1,-1)){ file = 2;}
            else if(endsWith(inPath,BOOT_SOUND_BTSND,-1,-1)){ file = 3;}
            if(file == 0){ return NULL;}

            result = (char*)malloc((sizeof(char) * (strlen(newBase) +1 + strlen(META_FOLDER) + 1 + 20) + 1));
            if(result == NULL){
                DEBUG_FUNCTION_LINE("malloc for result failed.\n");
                return NULL;
            }
            if(file == 1){sprintf(result,"%s/%s/%s",newBase,META_FOLDER,BOOT_TV_TEX_TGA);}
            if(file == 2){sprintf(result,"%s/%s/%s",newBase,META_FOLDER,BOOT_DRC_TEX_TGA);}
            if(file == 3){sprintf(result,"%s/%s/%s",newBase,META_FOLDER,BOOT_SOUND_BTSND);}
            return result;
        }*/

        return NULL;
    }

    result = (char*)malloc((sizeof(char) * (strlen(newBase) +1+ strlen(relativePath))) + 1);
    if(result == NULL){
        DEBUG_FUNCTION_LINE("malloc for result failed.\n");
        OSFATAL_FUNCTION_LINE("malloc for result failed.\n");
        return NULL;
    }
    sprintf(result,"%s/%s",newBase,relativePath);

    free(relativePath);
    relativePath = NULL;

    return result;
}

char * getRelativePath(const char *path){
    if(path == NULL) return NULL;
    char * pathForCheck = NULL;

    int gameFile = is_gamefile(path);
    if(gameFile > 0) {
        //if(DEBUG_LOG) log_printf("getNewPath %s\n", path);
        int path_offset = 0;

        // In case the path starts by "//" and not "/" (some games do that ... ...)
        if (path[0] == '/' && path[1] == '/')
            path = &path[1];

        // In case the path does not start with "/" set an offset for all the accesses
        if(path[0] != '/')
            path_offset = -1;


        if(gameFile == GAME_PATH_TYPE_CONTENT) { // is content
            //content

            // some games are doing /vol/content/./....
            if(path[13 + path_offset] == '.' && path[14 + path_offset] == '/') {
                path_offset += 2;
            }
            char * pathForCheckInternal = (char*)path + 13 + path_offset;
            if(pathForCheckInternal[0] == '/') pathForCheckInternal++; //Skip double slash

            pathForCheck = (char*)malloc(sizeof(CONTENT_FOLDER) + 1 + (sizeof(char) * (strlen(pathForCheckInternal) + 1)));
            if(pathForCheck == NULL){
                DEBUG_FUNCTION_LINE("malloc failed\n");
                OSFATAL_FUNCTION_LINE("malloc failed\n");
                return NULL;
            }
            sprintf(pathForCheck,"%s/%s",CONTENT_FOLDER,pathForCheckInternal);
        } else if (gameFile == GAME_PATH_TYPE_AOC) { // is aoc
            //aoc
            int aocFolderLength = 1;
            char * aocFolderLengthCheck = (char*)path + 5 + path_offset;
            while (aocFolderLengthCheck[0] != '/') {
                aocFolderLength++;
                aocFolderLengthCheck++;
            }
            char * pathForCheckInternal = (char*)path + 5 + aocFolderLength + path_offset;
            if(pathForCheckInternal[0] == '/') pathForCheckInternal++; //Skip double slash

            pathForCheck = (char*)malloc(sizeof(AOC_FOLDER) + 1 + (sizeof(char) * (strlen(pathForCheckInternal) + 1)));
            if(pathForCheck == NULL){
                DEBUG_FUNCTION_LINE("malloc failed\n");
                OSFATAL_FUNCTION_LINE("malloc failed\n");
                return NULL;
            }
            sprintf(pathForCheck,"%s/%s",AOC_FOLDER,pathForCheckInternal);
        } else if (gameFile == GAME_PATH_TYPE_STRIPPED_CONTENT) { // is stripped content
            pathForCheck = (char*)malloc(sizeof(CONTENT_FOLDER) + 1 + (sizeof(char) * (strlen(path) + 1)));
            if(pathForCheck == NULL){
                DEBUG_FUNCTION_LINE("malloc failed\n");
                OSFATAL_FUNCTION_LINE("malloc failed\n");
                return NULL;
            }
            sprintf(pathForCheck,"%s/%s",CONTENT_FOLDER,path);
        }
    }

    return pathForCheck;
}
