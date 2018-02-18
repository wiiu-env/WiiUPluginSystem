/****************************************************************************
 * Copyright (C) 2015 Dimok
 * Modified by Maschell, 2018 for Wii U Plugin System loader
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
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <dynamic_libs/padscore_functions.h>
#include <utils/StringTools.h>
#include <utils/logger.h>
#include "MainWindow.h"
#include "MainWindowGUI.h"
#include "Application.h"

MainWindow * MainWindow::instance = NULL;

MainWindow::MainWindow(s32 w, s32 h)
    : width(w)
    , height(h)
{
    for(s32 i = 0; i < 4; i++) {
        std::string filename = StringTools::strfmt("player%i_point.png", i+1);
        pointerImgData[i] = Resources::GetImageData(filename.c_str());
        pointerImg[i] = new GuiImage(pointerImgData[i]);
        pointerImg[i]->setScale(1.5f);
        pointerValid[i] = false;
    }

    SetupMainView();
}

MainWindow::~MainWindow()
{

    while(!tvElements.empty())
    {
        delete tvElements[0];
        remove(tvElements[0]);
    }
    while(!drcElements.empty())
    {
        delete drcElements[0];
        remove(drcElements[0]);
    }
    for(s32 i = 0; i < 4; i++)
    {
        delete pointerImg[i];
        Resources::RemoveImageData(pointerImgData[i]);
    }
}

void MainWindow::updateEffects()
{
    //! dont read behind the initial elements in case one was added
    u32 tvSize = tvElements.size();
    u32 drcSize = drcElements.size();

    for(u32 i = 0; (i < drcSize) && (i < drcElements.size()); ++i)
    {
        drcElements[i]->updateEffects();
    }

    //! only update TV elements that are not updated yet because they are on DRC
    for(u32 i = 0; (i < tvSize) && (i < tvElements.size()); ++i)
    {
        u32 n;
        for(n = 0; (n < drcSize) && (n < drcElements.size()); n++)
        {
            if(tvElements[i] == drcElements[n])
                break;
        }
        if(n == drcElements.size())
        {
            tvElements[i]->updateEffects();
        }
    }
}

void MainWindow::process()
{
    //! dont read behind the initial elements in case one was added
    u32 tvSize = tvElements.size();
    u32 drcSize = drcElements.size();

    for(u32 i = 0; (i < drcSize) && (i < drcElements.size()); ++i)
    {
        drcElements[i]->process();
    }

    //! only update TV elements that are not updated yet because they are on DRC
    for(u32 i = 0; (i < tvSize) && (i < tvElements.size()); ++i)
    {
        u32 n;
        for(n = 0; (n < drcSize) && (n < drcElements.size()); n++)
        {
            if(tvElements[i] == drcElements[n])
                break;
        }
        if(n == drcElements.size())
        {
            tvElements[i]->process();
        }
    }
}

void MainWindow::update(GuiController *controller)
{
    //! dont read behind the initial elements in case one was added
    //u32 tvSize = tvElements.size();

    if(controller->chan & GuiTrigger::CHANNEL_1)
    {
        u32 drcSize = drcElements.size();

        for(u32 i = 0; (i < drcSize) && (i < drcElements.size()); ++i)
        {
            drcElements[i]->update(controller);
        }
    }
    else
    {
        u32 tvSize = tvElements.size();

        for(u32 i = 0; (i < tvSize) && (i < tvElements.size()); ++i)
        {
            tvElements[i]->update(controller);
        }
    }

//    //! only update TV elements that are not updated yet because they are on DRC
//    for(u32 i = 0; (i < tvSize) && (i < tvElements.size()); ++i)
//    {
//        u32 n;
//        for(n = 0; (n < drcSize) && (n < drcElements.size()); n++)
//        {
//            if(tvElements[i] == drcElements[n])
//                break;
//        }
//        if(n == drcElements.size())
//        {
//            tvElements[i]->update(controller);
//        }
//    }

    if(controller->chanIdx >= 1 && controller->chanIdx <= 4 && controller->data.validPointer)
    {
        s32 wpadIdx = controller->chanIdx - 1;
        f32 posX = controller->data.x;
        f32 posY = controller->data.y;
        pointerImg[wpadIdx]->setPosition(posX, posY);
        pointerImg[wpadIdx]->setAngle(controller->data.pointerAngle);
        pointerValid[wpadIdx] = true;
    }
}

void MainWindow::drawDrc(CVideo *video)
{
    for(u32 i = 0; i < drcElements.size(); ++i)
    {
        drcElements[i]->draw(video);
    }

    for(s32 i = 0; i < 4; i++)
    {
        if(pointerValid[i])
        {
            pointerImg[i]->setAlpha(0.5f);
            pointerImg[i]->draw(video);
            pointerImg[i]->setAlpha(1.0f);
        }
    }
}

void MainWindow::drawTv(CVideo *video)
{
    for(u32 i = 0; i < tvElements.size(); ++i)
    {
        tvElements[i]->draw(video);
    }

    for(s32 i = 0; i < 4; i++)
    {
        if(pointerValid[i])
        {
            pointerImg[i]->draw(video);
            pointerValid[i] = false;
        }
    }
}

void MainWindow::SetupMainView(){
    DrcFrame =  new MainWindowGUI(width,height);
    TvFrame =  DrcFrame;
    appendTv(TvFrame);
    appendDrc(DrcFrame);
}

void MainWindow::OnOpenEffectFinish(GuiElement *element)
{
    //! once the menu is open reset its state and allow it to be "clicked/hold"
    element->effectFinished.disconnect(this);
    element->clearState(GuiElement::STATE_DISABLED);
}

void MainWindow::appendToAllElements(GuiElement * element){
    u32 drcSize = drcElements.size();
    for(u32 i = 0; (i < drcSize) && (i < drcElements.size()); ++i)
    {
        GuiFrame * realElement = dynamic_cast<GuiFrame*>(drcElements[i]);
        if(realElement != NULL){
            realElement->append(element);
        }
    }

    u32 tvSize = tvElements.size();
    for(u32 i = 0; (i < tvSize) && (i < tvElements.size()); ++i)
    {
        GuiFrame * realElement = dynamic_cast<GuiFrame*>(tvElements[i]);
        if(realElement != NULL){
            realElement->append(element);
        }
    }
}

void MainWindow::removeFromAllElements(GuiElement * element){
    u32 drcSize = drcElements.size();
    for(u32 i = 0; (i < drcSize) && (i < drcElements.size()); ++i)
    {
        GuiFrame * realElement = dynamic_cast<GuiFrame*>(drcElements[i]);
        if(realElement != NULL){
            realElement->remove(element);
        }
    }

    u32 tvSize = tvElements.size();
    for(u32 i = 0; (i < tvSize) && (i < tvElements.size()); ++i)
    {
        GuiFrame * realElement = dynamic_cast<GuiFrame*>(tvElements[i]);
        if(realElement != NULL){
            realElement->remove(element);
        }
    }
}


void MainWindow::setState(s32 val, s32 c){
    u32 drcSize = drcElements.size();
    for(u32 i = 0; (i < drcSize) && (i < drcElements.size()); ++i)
    {
        drcElements[i]->setState(val,c);
    }

    u32 tvSize = tvElements.size();
    for(u32 i = 0; (i < tvSize) && (i < tvElements.size()); ++i)
    {
        tvElements[i]->setState(val,c);
    }
}

void MainWindow::clearState(s32 val, s32 c){
    u32 drcSize = drcElements.size();
    for(u32 i = 0; (i < drcSize) && (i < drcElements.size()); ++i)
    {
        drcElements[i]->clearState(val,c);
    }

    u32 tvSize = tvElements.size();
    for(u32 i = 0; (i < tvSize) && (i < tvElements.size()); ++i)
    {
        tvElements[i]->clearState(val,c);
    }
}

void MainWindow::OnCloseEffectFinish(GuiElement *element){
    //! remove element from draw list and push to delete queue
    remove(element);

    AsyncDeleter::pushForDelete(element);

    clearState(GuiElement::STATE_DISABLE_INPUT);
}
