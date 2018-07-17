/****************************************************************************
 * Copyright (C) 2018 Maschell
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
#include <wups.h>
#include <stdarg.h>
#include "dynamic_libs/os_functions.h"
#include "ScreenUtils.h"

void ScreenUtils::printTextOnScreen(wups_overlay_options_type_t screen, int x,int y, const char * msg, ...) {
    if(screen == WUPS_OVERLAY_NONE) {
        return;
    }

    char * tmp = NULL;

    va_list va;
    va_start(va, msg);
    if((vasprintf(&tmp, msg, va) >= 0) && tmp) {
        if(screen != WUPS_OVERLAY_DRC_ONLY) { // Draw TV if it's not DRC exclusive.
            OSScreenPutFontEx(0, x, y, tmp);
        }
        if(screen != WUPS_OVERLAY_TV_ONLY) { // Draw DRC if it's not TV exclusive.
            OSScreenPutFontEx(1, x, y, tmp);
        }
    }
    va_end(va);

    if(tmp) {
        free(tmp);
    }
}

void ScreenUtils::OSScreenClear(wups_overlay_options_type_t screen) {
    if(screen == WUPS_OVERLAY_NONE) {
        return;
    }
    if(screen != WUPS_OVERLAY_DRC_ONLY) { // Clear TV if it's not DRC exclusive.
        OSScreenClearBufferEx(0, 0);
    }
    if(screen != WUPS_OVERLAY_TV_ONLY) { // Clear DRC if it's not TV exclusive.
        OSScreenClearBufferEx(1, 0);
    }
}

void ScreenUtils::flipBuffers(wups_overlay_options_type_t screen) {
    if(screen == WUPS_OVERLAY_NONE) {
        return;
    }
    if(screen != WUPS_OVERLAY_DRC_ONLY) { // Flip TV buffer if it's not DRC exclusive.
        OSScreenFlipBuffersEx(0);
    }
    if(screen != WUPS_OVERLAY_TV_ONLY) { // Flip DRC buffer if it's not TV exclusive.
        OSScreenFlipBuffersEx(1);
    }
}
