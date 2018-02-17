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
#include "MainWindowGUI.h"
#include "MenuCommon.h"

MainWindowGUI::MainWindowGUI(s32 w, s32 h)
    : GuiFrame(w, h)
    , width(w)
    , height(h)
    , content(SCREEN_WIDTH,h){
    ContentTemplate * curTemplate = content.createNewContent(MainWindowContent::CONTENT_HOME);
    content.SetScreen(curTemplate);
    append(&content);
}

void MainWindowGUI::setState(s32 i, s32 c){
    GuiFrame::setState(i,c);
}

void MainWindowGUI::process(){
    GuiFrame::process();
}

MainWindowGUI::~MainWindowGUI(){
    content.RemoveScreen();
    remove(&content);
}

