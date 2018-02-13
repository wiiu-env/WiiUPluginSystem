/****************************************************************************
 * Copyright (C) 2017,2018 Maschell
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
#ifndef _VOICE_INFO_H_
#define _VOICE_INFO_H_

#include <dynamic_libs/os_types.h>

#define VOICE_INFO_MAX 100

typedef struct _VoiceInfo {
    void* voice;    /**< Pointer to the voice */
    u32 mixTV[24];  /**< Mix to the TV */
    u32 mixDRC[24]; /**< Mix of the DRC */
} VoiceInfo;

#endif //_VOICE_INFO_H_
