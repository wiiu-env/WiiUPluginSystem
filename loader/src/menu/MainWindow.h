/****************************************************************************
 * Copyright (C) 2015 Dimok
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
#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <vector>
#include <queue>
#include <gui/Gui.h>
#include <system/CMutex.h>

class CVideo;

class MainWindow : public sigslot::has_slots<> {
public:
    virtual ~MainWindow();

    static MainWindow *instance;

    static MainWindow *getInstance(int32_t w,int32_t h) {
        if(!instance) {
            instance = new MainWindow(w, h);
        }
        return instance;
    }

    static MainWindow *getInstance() {
        return instance;
    }

    static void destroyInstance() {
        if(instance) {
            delete instance;
            instance = NULL;
        }
    }

    void appendTv(GuiElement *e) {
        if(!e)
            return;

        removeTv(e);
        tvElements.push_back(e);
    }
    void appendDrc(GuiElement *e) {
        if(!e)
            return;

        removeDrc(e);
        drcElements.push_back(e);
    }

    void append(GuiElement *e) {
        appendTv(e);
        appendDrc(e);
    }

    void insertTv(uint32_t pos, GuiElement *e) {
        if(!e)
            return;

        removeTv(e);
        tvElements.insert(tvElements.begin() + pos, e);
    }
    void insertDrc(uint32_t pos, GuiElement *e) {
        if(!e)
            return;

        removeDrc(e);
        drcElements.insert(drcElements.begin() + pos, e);
    }

    void insert(uint32_t pos, GuiElement *e) {
        insertTv(pos, e);
        insertDrc(pos, e);
    }

    void removeTv(GuiElement *e) {
        for(uint32_t i = 0; i < tvElements.size(); ++i) {
            if(e == tvElements[i]) {
                tvElements.erase(tvElements.begin() + i);
                break;
            }
        }
    }
    void removeDrc(GuiElement *e) {
        for(uint32_t i = 0; i < drcElements.size(); ++i) {
            if(e == drcElements[i]) {
                drcElements.erase(drcElements.begin() + i);
                break;
            }
        }
    }

    void remove(GuiElement *e) {
        removeTv(e);
        removeDrc(e);
    }
    void removeAll() {
        tvElements.clear();
        drcElements.clear();
    }

    void drawDrc(CVideo *video);
    void drawTv(CVideo *video);
    void update(GuiController *controller);
    void updateEffects();
    void process();

    void appendToAllElements(GuiElement * element);
    void removeFromAllElements(GuiElement * element);
    void setState(int32_t i, int32_t c = -1 );
    void clearState(int32_t i, int32_t c = -1);

    void lockGUI() {
        guiMutex.lock();
    }
    void unlockGUI() {
        guiMutex.unlock();
    }

private:
    MainWindow(int32_t w, int32_t h);
    void SetupMainView(void);

    void OnOpenEffectFinish(GuiElement *element);
    void OnCloseEffectFinish(GuiElement *element);

    int32_t width, height;
    std::vector<GuiElement *> drcElements;
    std::vector<GuiElement *> tvElements;

    GuiSound *ClickSound;

    GuiFrame * TvFrame;
    GuiFrame * DrcFrame;

    GuiImageData *pointerImgData[4];
    GuiImage *pointerImg[4];
    bool pointerValid[4];

    CMutex guiMutex;
};

#endif //_MAIN_WINDOW_H_
