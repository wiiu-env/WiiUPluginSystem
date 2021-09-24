/****************************************************************************
 * Copyright (C) 2021 Maschell
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

#pragma once

#include <stdint.h>

#define WUPS_CONFIG_BUTTON_NONE     0
#define WUPS_CONFIG_BUTTON_LEFT     (1<<0)
#define WUPS_CONFIG_BUTTON_RIGHT    (1<<1)
#define WUPS_CONFIG_BUTTON_UP       (1<<2)
#define WUPS_CONFIG_BUTTON_DOWN     (1<<3)
#define WUPS_CONFIG_BUTTON_A        (1<<4)
#define WUPS_CONFIG_BUTTON_B        (1<<5)
#define WUPS_CONFIG_BUTTON_ZL       (1<<6)
#define WUPS_CONFIG_BUTTON_ZR       (1<<7)
#define WUPS_CONFIG_BUTTON_L        (1<<8)
#define WUPS_CONFIG_BUTTON_R        (1<<9)
typedef int32_t WUPSConfigButtons;

typedef struct {
    int32_t (*getCurrentValueDisplay)(void *context, char *out_buf, int32_t out_size);

    int32_t (*getCurrentValueSelectedDisplay)(void *context, char *out_buf, int32_t out_size);

    void (*onSelected)(void *context, bool isSelected);

    void (*restoreDefault)(void *context);

    bool (*isMovementAllowed)(void *context);

    bool (*callCallback)(void *context);

    void (*onButtonPressed)(void *context, WUPSConfigButtons button);

    void (*onDelete)(void *context);
} WUPSConfigCallbacks_t;

typedef uint32_t WUPSConfigItemHandle;
typedef uint32_t WUPSConfigHandle;
typedef uint32_t WUPSConfigCategoryHandle;