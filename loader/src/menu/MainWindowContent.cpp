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
#include "MainWindowContent.h"

MainWindowContent::MainWindowContent(int32_t w, int32_t h)
    : GuiFrame(w, h)
    , width(w)
    , height(h)
    , bgImageColor(w, h, (GX2Color) {
    0, 0, 0, 0
}) {
    bgImageColor.setImageColor((GX2Color) {
        248, 249, 248, 255
    }, 0);
    bgImageColor.setImageColor((GX2Color) {
        248, 249, 248, 255
    }, 1);
    bgImageColor.setImageColor((GX2Color) {
        248, 249, 248, 255
    }, 2);
    bgImageColor.setImageColor((GX2Color) {
        248, 249, 248, 255
    }, 3);

    append(&bgImageColor);
}

ContentTemplate * MainWindowContent::getContent() {
    return content;
}



void MainWindowContent::SetScreen(ContentTemplate * new_content) {
    RemoveScreen();
    if(new_content) {
        //while(content != NULL); //hopefully this shit don't kill us.

        content = new_content;
        append(content);
    }
}

void MainWindowContent::RemoveScreen() {
    remove(content);
    delete content;
    content = NULL;
}

void MainWindowContent::OnCloseEffectFinish(GuiElement *element) {

}

MainWindowContent::~MainWindowContent() {
    remove(&bgImageColor);
    RemoveScreen();
}
