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
#ifndef C_RETAINS_VARS_H_
#define C_RETAINS_VARS_H_
#include "dynamic_libs/gx2_functions.h"
#include "utils/voice_info.h"
extern u8 gSwap;
extern u8 gCallbackCooldown;
extern u8 gAppStatus;
extern u32 gButtonCombo;
extern VoiceInfo gVoiceInfos[VOICE_INFO_MAX];

#endif // C_RETAINS_VARS_H_
