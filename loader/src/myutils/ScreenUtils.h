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
#ifndef __SCREEN_UTILS_H_
#define __SCREEN_UTILS_H_

class ScreenUtils {
public:
    /**
        Clears the screen for the given screens
        \param screen defines on which screens should be printed
        \param x defines the x position (character position) where the text should be printed
        \param y defines on which line the text should be printed
        \param msg C string that contains the text to be printed.
               It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
               See printf for more information
        \param ... Depending on the format string, the function may expect a sequence of additional arguments, each containing a value to be used to replace a format specifier in the format string
    **/
    static void printTextOnScreen(wups_overlay_options_type_t screen, int x,int y, const char * msg, ...);

    /**
        Clears the screen for the given screens
        \param screen defines which screens should be cleared
    **/
    static void OSScreenClear(wups_overlay_options_type_t screen);

    /**
        Flips the buffer for the given screens
        \param screen defines which screens should be flipped.
    **/
    static void flipBuffers(wups_overlay_options_type_t screen);
private:
    ScreenUtils() {}
    ~ScreenUtils() {}

};
#endif
