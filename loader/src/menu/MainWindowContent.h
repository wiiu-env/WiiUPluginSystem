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
#ifndef _MAIN_WINDOW_CONTENT_H_
#define _MAIN_WINDOW_CONTENT_H_

#include <gui/Gui.h>
#include <language/gettext.h>
#include <system/AsyncDeleter.h>

#include "content/ContentTemplate.h"
#include "content/ContentHome.h"
#include "MainWindowContent.h"

class MainWindowContent : public GuiFrame, public sigslot::has_slots<>{
public:
    enum eContentNames{
        CONTENT_HOME,
    };

    MainWindowContent(s32 w, s32 h);
    virtual ~MainWindowContent();

    void SetScreen(ContentTemplate * new_content);
    void RemoveScreen();
    ContentTemplate * getContent();

     //If you call this function, please also delete the object. This class isn't taking care of it
    static ContentTemplate * createNewContent(eContentNames contentName){
        switch(contentName){
            case CONTENT_HOME: {           return new ContentHome();  break;}
            default: return NULL;
        }
    }

private:
    s32 width, height;
    GuiImage bgImageColor;

    ContentTemplate * content = NULL;

    void OnCloseEffectFinish(GuiElement *element);
};

#endif //_MAIN_WINDOW_CONTENT_H_
