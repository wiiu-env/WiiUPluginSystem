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

#ifndef _MAIN_H_
#define _MAIN_H_

#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/os_types.h>

/* Main */
#ifdef __cplusplus
extern "C" {
#endif

#include <libelf.h>

//! C wrapper for our C++ functions
int Menu_Main(int argc, char **argv);

void Init_SD_USB();

void DeInit_SD_USB();

void Init();
void DeInit();

#ifdef __cplusplus
}
#endif

#endif
