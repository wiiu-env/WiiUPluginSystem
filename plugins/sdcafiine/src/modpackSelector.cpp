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
#include <dynamic_libs/padscore_functions.h>
#include <utils/logger.h>
#include <utils/StringTools.h>
#include <fs/FSUtils.h>
#include <fs/CFile.hpp>
#include <fs/DirList.h>
#include "common/retain_vars.h"

#define TEXT_SEL(x, text1, text2)           ((x) ? (text1) : (text2))

enum PadButton {
	PAD_BTN_A,
	PAD_BTN_UP,
	PAD_BTN_DOWN,
	PAD_BTN_L,
	PAD_BTN_R
};

VPADData vpad;
s32 vpadError;

KPADData pads[4];
s32 padErrors[4];
u32 padTypes[4];

void initControllers() {
	KPADInit();
}

void releaseControllers() {
	for(int i = 0; i < 4; i++) {
		padErrors[i] = WPADProbe(i, &padTypes[i]);
		if(padErrors[i] == 0) {
			WPADDisconnect(i);
		}
	}

	//Clear buffers so future launches of SDCafiine don't read old data
	memset(&vpad, 0, sizeof(VPADData));
	vpadError = 0;

	memset(pads, 0, sizeof(KPADData) * 4);
	memset(padErrors, 0, sizeof(s32) * 4);
	memset(padTypes, 0, sizeof(u32) * 4);
}

void pollControllers() {
	VPADRead(0, &vpad, 1, &vpadError);

	for(int i = 0; i < 4; i++) {
		padErrors[i] = WPADProbe(i, &padTypes[i]);
		if(padErrors[i] == 0) {
			KPADRead(i, &pads[i], 1);
		}
	}
}

bool isButtonPressed(enum PadButton btn) {
	if(vpadError == 0) {
		switch (btn) {
		case PAD_BTN_A:
			if (vpad.btns_d & VPAD_BUTTON_A) return true;
			break;

		case PAD_BTN_UP:
			if (vpad.btns_d & VPAD_BUTTON_UP) return true;
			break;

		case PAD_BTN_DOWN:
			if (vpad.btns_d & VPAD_BUTTON_DOWN) return true;
			break;

		case PAD_BTN_L:
			if (vpad.btns_d & VPAD_BUTTON_L) return true;
			break;

		case PAD_BTN_R:
			if (vpad.btns_d & VPAD_BUTTON_R) return true;
			break;

		default:
			break;
		}
	}

	for(int i = 0; i < 4; i++) {
		if(padErrors[i] == 0) {
			if(pads[i].device_type < 2) {
				switch(btn) {
				case PAD_BTN_A:
					if (pads[i].btns_d & WPAD_BUTTON_A) return true;
					break;

				case PAD_BTN_UP:
					if (pads[i].btns_d & WPAD_BUTTON_UP) return true;
					break;

				case PAD_BTN_DOWN:
					if (pads[i].btns_d & WPAD_BUTTON_DOWN) return true;
					break;

				case PAD_BTN_L:
					if (pads[i].btns_d & WPAD_BUTTON_1) return true;
					break;

				case PAD_BTN_R:
					if (pads[i].btns_d & WPAD_BUTTON_2) return true;
					break;

				default:
					break;
				}
			}
			else {
				switch(btn) {
				case PAD_BTN_A:
					if (pads[i].classic.btns_d & WPAD_CLASSIC_BUTTON_A) return true;
					break;

				case PAD_BTN_UP:
					if (pads[i].classic.btns_d & WPAD_CLASSIC_BUTTON_UP) return true;
					break;

				case PAD_BTN_DOWN:
					if (pads[i].classic.btns_d & WPAD_CLASSIC_BUTTON_DOWN) return true;
					break;

				case PAD_BTN_L:
					if (pads[i].classic.btns_d & WPAD_CLASSIC_BUTTON_L) return true;
					break;

				case PAD_BTN_R:
					if (pads[i].classic.btns_d & WPAD_CLASSIC_BUTTON_R) return true;
					break;

				default:
					break;
				}
			}
		}
	}

	return false;
}

void HandleMultiModPacks(u64 titleID/*,bool showMenu*/) {
	gModFolder[0] = 0;

    char TitleIDString[FS_MAX_FULLPATH_SIZE];
    snprintf(TitleIDString,FS_MAX_FULLPATH_SIZE,"%016llX",titleID);

    std::map<std::string,std::string> modTitlePath;

    std::map<std::string,std::string> mounting_points;
    if(gSDInitDone){ mounting_points[std::string(SD_PATH)] = std::string(NAME_PREFIX_SD); }
    if(gUSBInitDone){ mounting_points[std::string(USB_PATH)] = std::string(NAME_PREFIX_USB); }
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

    initControllers();

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

        pollControllers();

		if (isButtonPressed(PAD_BTN_A)) {
			wantToExit = 1;
			initScreen = 1;
		}
		else if (isButtonPressed(PAD_BTN_DOWN)) {
			selected++;
			initScreen = 1;
		}
		else if (isButtonPressed(PAD_BTN_UP)) {
			selected--;
			initScreen = 1;
		}
		else if (isButtonPressed(PAD_BTN_L)) {
			selected -= per_page;
			initScreen = 1;
		}
		else if (isButtonPressed(PAD_BTN_R)) {
			selected += per_page;
			initScreen = 1;
		}

		if (selected < 0) selected = 0;
		if (selected >= modPackListSize) selected = modPackListSize - 1;
		page = selected / per_page;

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

    releaseControllers();

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
