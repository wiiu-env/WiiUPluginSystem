#include <string>

#include <map>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>
#include "modpackSelector.h"
#include "common/common.h"

#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/vpad_functions.h>
#include <utils/logger.h>
#include <utils/StringTools.h>
#include <fs/FSUtils.h>
#include <fs/CFile.hpp>
#include <fs/DirList.h>
#include "common/retain_vars.h"

#define TEXT_SEL(x, text1, text2)           ((x) ? (text1) : (text2))

void HandleMultiModPacks(u64 titleID/*,bool showMenu*/) {
	gModFolder[0] = 0;

    char TitleIDString[FS_MAX_FULLPATH_SIZE];
    snprintf(TitleIDString,FS_MAX_FULLPATH_SIZE,"%016llX",titleID);

    std::map<std::string,std::string> modTitlePath;

    std::map<std::string,std::string> mounting_points;
    if(gSDInitDone & SDUSB_MOUNTED_OS_SD){ mounting_points[std::string(SD_PATH)] = std::string(NAME_PREFIX_SD); }
    if(gSDInitDone & SD_MOUNTED_LIBFAT){ mounting_points[std::string(SD_PATH)] = std::string(NAME_PREFIX_SD); }
    if(gSDInitDone & USB_MOUNTED_LIBFAT){ mounting_points[std::string(USB_PATH)] = std::string(NAME_PREFIX_USB); }
    //int i = 0;

    //for (i = 0; i < ntfs_mount_count; i++){
        //mounting_points[std::string(((ntfs_md *)ntfs_mounts)[i].name) + ":"] = StringTools::strfmt("%s:(%s)",((ntfs_md *)ntfs_mounts)[i].name, ntfsGetVolumeName(((ntfs_md *)ntfs_mounts)[i].name));
    //}

   for (std::map<std::string,std::string>::iterator it=mounting_points.begin(); it!=mounting_points.end(); ++it){

        std::string curMount = it->first;
        std::string curMountName = it->second;
        //DEBUG_FUNCTION_LINE("%s %s \n",curMount.c_str(),curMountName.c_str());
        std::string modTitleIDPath = curMount + GAME_MOD_FOLDER + "/" + TitleIDString;
        //DEBUG_FUNCTION_LINE("modTitleIDPath %s \n",modTitleIDPath.c_str());
        DirList modTitleDirList(modTitleIDPath.c_str(), NULL, DirList::Dirs);

        modTitleDirList.SortList();

        for(int i = 0; i < modTitleDirList.GetFilecount(); i++) {
            std::string curFile = modTitleDirList.GetFilename(i);
            //DEBUG_FUNCTION_LINE("curFile %s \n",curFile.c_str());
            if(curFile.compare(".") == 0 || curFile.compare("..") == 0)
                continue;

            if(curFile.compare(CONTENT_FOLDER) == 0 || curFile.compare(AOC_FOLDER) == 0/* || curFile.compare(META_FOLDER) == 0*/) {
                std::string packageName = curMountName + " " + DEFAULT_NAME_PACKAGE;
                modTitlePath[packageName] = modTitleIDPath;
                DEBUG_FUNCTION_LINE("found %s \n",packageName.c_str());
            }else{
                std::string packageName = curMountName + " " + curFile;
                modTitlePath[packageName] = modTitleIDPath + "/" + curFile;
                DEBUG_FUNCTION_LINE("found %s \n",packageName.c_str());
            }
        }
    }

    //DEBUG_FUNCTION_LINE("Iteration done\n");

    int modPackListSize =modTitlePath.size();

    if(modPackListSize == 0) return;
    if(modPackListSize == 1/* || !showMenu*/){
        for (std::map<std::string,std::string>::iterator it=modTitlePath.begin(); it!=modTitlePath.end(); ++it){
            snprintf(gModFolder, FS_MAX_ENTNAME_SIZE, "%s", it->second.c_str());
            break;
        }
        return;
    }

    int selected = 0;
    int initScreen = 1;
    int x_offset = -2;

    VPADData vpad;
    s32 vpadError;

    OSScreenInit();
    u32 screen_buf0_size = OSScreenGetBufferSizeEx(0);
    u32 screen_buf1_size = OSScreenGetBufferSizeEx(1);
    u32 * screenbuffers = (u32*)memalign(0x100, screen_buf0_size + screen_buf1_size);
    OSScreenSetBufferEx(0, (void *)screenbuffers);
    OSScreenSetBufferEx(1, (void *)(screenbuffers + screen_buf0_size));

    OSScreenEnableEx(0, 1);
    OSScreenEnableEx(1, 1);

    // Clear screens
    OSScreenClearBufferEx(0, 0);
    OSScreenClearBufferEx(1, 0);

    // Flip buffers
    OSScreenFlipBuffersEx(0);
    OSScreenFlipBuffersEx(1);

    int wantToExit = 0;
    int page = 0;
    int per_page = 13;
    int max_pages = (modPackListSize / per_page) + 1;

    while(1){

        vpadError = -1;
        VPADRead(0, &vpad, 1, &vpadError);

        if(vpadError == 0) {
            if(vpad.btns_d & VPAD_BUTTON_A) {
                wantToExit = 1;
                initScreen = 1;
            } else if(vpad.btns_d & VPAD_BUTTON_DOWN) {
                selected++;
                initScreen = 1;
            } else if(vpad.btns_d & VPAD_BUTTON_UP) {
                selected--;
                initScreen = 1;
            } else if(vpad.btns_d & VPAD_BUTTON_L) {
                selected -= per_page;
                initScreen = 1;
            } else if(vpad.btns_d & VPAD_BUTTON_R) {
                selected += per_page;
                initScreen = 1;
            }
            if(selected < 0) selected = 0;
            if(selected >= modPackListSize) selected = modPackListSize-1;
            page = selected / per_page;
        }

        if(initScreen) {
            OSScreenClearBufferEx(0, 0);
            OSScreenClearBufferEx(1, 0);
            console_print_pos(x_offset, -1, "                  -- SDCafiine lite by Maschell --");
            console_print_pos(x_offset, 1, "Select your options and press A to launch.");
            int y_offset = 3;
            int cur_ = 0;

            for (std::map<std::string,std::string>::iterator it=modTitlePath.begin(); it!=modTitlePath.end(); ++it){
                std::string key = it->first;
                std::string value = it->second;

                if(wantToExit && cur_ == selected){
                    snprintf(gModFolder, FS_MAX_ENTNAME_SIZE, "%s", value.c_str());
                    break;
                }

                if(cur_ >= (page*per_page) && cur_ < ((page+1)*per_page)){
                     console_print_pos(x_offset, y_offset++, "%s %s",TEXT_SEL((selected == cur_), "--->", "    "), key.c_str());
                }
                cur_++;
            }

            if(wantToExit){ //just in case.
                break;
            }

            if(max_pages > 0) {
                console_print_pos(x_offset, 17, "Page %02d/%02d. Press L/R to change page.", page + 1,max_pages);
            }

            // Flip buffers
            OSScreenFlipBuffersEx(0);
            OSScreenFlipBuffersEx(1);

            initScreen = 0;
        }
        os_usleep(20000);
    }

    OSScreenClearBufferEx(0, 0);
    OSScreenClearBufferEx(1, 0);

    // Flip buffers
    OSScreenFlipBuffersEx(0);
    OSScreenFlipBuffersEx(1);

    free(screenbuffers);

    return;
}

void console_print_pos(int x, int y, const char *format, ...){
    char * tmp = NULL;

    va_list va;
    va_start(va, format);
    if((vasprintf(&tmp, format, va) >= 0) && tmp)
    {
        if(strlen(tmp) > 79)
            tmp[79] = 0;

        OSScreenPutFontEx(0, x, y, tmp);
        OSScreenPutFontEx(1, x, y, tmp);

    }
    va_end(va);

    if(tmp)
        free(tmp);
}
