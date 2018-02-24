/****************************************************************************
 * Copyright (C) 2016-2018 Maschell
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
#ifndef _CONTENT_TEMPLATE_H_
#define _CONTENT_TEMPLATE_H_

#include "gui/Gui.h"
#include "menu/MenuCommon.h"

class ContentTemplate : public GuiFrame, public sigslot::has_slots<> {

public:
    ContentTemplate() : GuiFrame(SCREEN_WIDTH, SCREEN_HEIGHT)
        , bgImageColor(SCREEN_WIDTH, SCREEN_HEIGHT, (GX2Color) {
        0, 0, 0, 0
    }) {
        bgImageColor.setImageColor((GX2Color) {
            42, 42, 42, 255
        }, 0);
        bgImageColor.setImageColor((GX2Color) {
            42, 42, 42, 255
        }, 1);
        bgImageColor.setImageColor((GX2Color) {
            42, 42, 42, 255
        }, 2);
        bgImageColor.setImageColor((GX2Color) {
            42, 42, 42, 255
        }, 3);
        append(&bgImageColor);
    }

    virtual ~ContentTemplate() {}
protected:
    GuiImage bgImageColor;
};

#endif /* CONTENT_TEMPLATE_H_ */
