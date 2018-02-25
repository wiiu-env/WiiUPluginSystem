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

#ifndef WUPS_UTILS_DEF_H_
#define WUPS_UTILS_DEF_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum wups_overlay_options_type_t {
    WUPS_OVERLAY_NONE,
    WUPS_OVERLAY_DRC_ONLY,                          /* Tries to display only on gamepad screen */
    WUPS_OVERLAY_TV_ONLY,                           /* Tries to display only on tv screen */
    WUPS_OVERLAY_DRC_AND_TV,                        /* Tries to display on both screens. Prioritizes the TV screen if memory is low. */
    WUPS_OVERLAY_DRC_AND_TV_WITH_DRC_PRIO           /* Tries to display on both screens. But if memory is low, prioritize the DRC screen.*/
} wups_overlay_options_type_t;

typedef void (*overlay_callback)(wups_overlay_options_type_t);

/*
    Gets called by the framework
*/
void WUPS_InitFS(wups_loader_init_fs_args_t args);
void WUPS_InitOverlay(wups_loader_init_overlay_args_t args);

/*
    Can be called by the user.
*/
void WUPS_Overlay_PrintTextOnScreen(wups_overlay_options_type_t screen, int x,int y, const char * msg, ...);

void WUPS_Overlay_OSScreenClear(wups_overlay_options_type_t screen);

void WUPS_Overlay_FlipBuffers(wups_overlay_options_type_t screen);

void WUPS_OpenOverlay(wups_overlay_options_type_t screen, overlay_callback callback);

#ifdef __cplusplus
}
#endif

#endif /* WUPS_WUPS_H_ */