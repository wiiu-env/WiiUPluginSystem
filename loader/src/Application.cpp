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
#include "Application.h"
#include "common/common.h"
#include <dynamic_libs/os_functions.h>
#include <gui/FreeTypeGX.h>
#include <gui/VPadController.h>
#include <gui/WPadController.h>
#include "resources/Resources.h"
#include <sounds/SoundHandler.hpp>
#include <utils/logger.h>
#include "settings/CSettings.h"
#include "myutils/TcpReceiver.h"

Application *Application::applicationInstance = NULL;
bool Application::exitApplication = false;

Application::Application()
    : CThread(CThread::eAttributeAffCore1 | CThread::eAttributePinnedAff, 0, 0x20000)
    , bgMusic(NULL)
    , video(NULL)
    , mainWindow(NULL)
    , exitCode(EXIT_RELAUNCH_ON_LOAD) {
    controller[0] = new VPadController(GuiTrigger::CHANNEL_1);
    controller[1] = new WPadController(GuiTrigger::CHANNEL_2);
    controller[2] = new WPadController(GuiTrigger::CHANNEL_3);
    controller[3] = new WPadController(GuiTrigger::CHANNEL_4);
    controller[4] = new WPadController(GuiTrigger::CHANNEL_5);

    CSettings::instance()->Load();

    //! create bgMusic
    bgMusic = new GuiSound(Resources::GetFile("bgMusic.mp3"), Resources::GetFileSize("bgMusic.mp3"));

    //! load language
    loadLanguageFromConfig();

    exitApplication = false;
}

Application::~Application() {
    DEBUG_FUNCTION_LINE("Destroy music\n");
    delete bgMusic;

    DEBUG_FUNCTION_LINE("Destroy controller\n");

    for(s32 i = 0; i < 5; i++)
        delete controller[i];

    //We may have to handle Asyncdelete in the Destructors.
    DEBUG_FUNCTION_LINE("Destroy async deleter\n");
    do {
        DEBUG_FUNCTION_LINE("Triggering AsyncDeleter\n");
        AsyncDeleter::triggerDeleteProcess();
        while(!AsyncDeleter::realListEmpty()) {
            os_usleep(1000);
        }
    } while(!AsyncDeleter::deleteListEmpty());
    AsyncDeleter::destroyInstance();

    DEBUG_FUNCTION_LINE("Clear resources\n");
    Resources::Clear();

    DEBUG_FUNCTION_LINE("Stop sound handler\n");
    SoundHandler::DestroyInstance();

}

s32 Application::exec() {
    //! start main GX2 thread
    resumeThread();
    //! now wait for thread to finish
    shutdownThread();

    return exitCode;
}

void Application::reloadUI() {
    reloadUIflag = true;
}
void Application::fadeOut() {
    GuiImage fadeOut(video->getTvWidth(), video->getTvHeight(), (GX2Color) {
        0, 0, 0, 255
    });

    for(s32 i = 0; i < 255; i += 10) {
        if(i > 255)
            i = 255;

        fadeOut.setAlpha(i / 255.0f);

        //! start rendering DRC
        video->prepareDrcRendering();
        mainWindow->drawDrc(video);

        GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_ALWAYS);
        fadeOut.draw(video);
        GX2SetDepthOnlyControl(GX2_ENABLE, GX2_ENABLE, GX2_COMPARE_LEQUAL);

        video->drcDrawDone();

        //! start rendering TV
        video->prepareTvRendering();

        mainWindow->drawTv(video);

        GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_ALWAYS);
        fadeOut.draw(video);
        GX2SetDepthOnlyControl(GX2_ENABLE, GX2_ENABLE, GX2_COMPARE_LEQUAL);

        video->tvDrawDone();

        //! as last point update the effects as it can drop elements
        mainWindow->updateEffects();

        video->waitForVSync();
    }

    //! one last cleared black screen
    video->prepareDrcRendering();
    video->drcDrawDone();
    video->prepareTvRendering();
    video->tvDrawDone();
    video->waitForVSync();
    video->tvEnable(false);
    video->drcEnable(false);
}

void Application::executeThread(void) {
    DEBUG_FUNCTION_LINE("Initialize video\n");
    video = new CVideo(GX2_TV_SCAN_MODE_720P, GX2_DRC_SINGLE);

    DEBUG_FUNCTION_LINE("Video size %i x %i\n", video->getTvWidth(), video->getTvHeight());

    //! setup default Font
    DEBUG_FUNCTION_LINE("Initialize main font system\n");
    FreeTypeGX *fontSystem = new FreeTypeGX(Resources::GetFile("font.ttf"), Resources::GetFileSize("font.ttf"), true);
    GuiText::setPresetFont(fontSystem);

    reloadUIflag = true;
    if(bgMusic != NULL) {
        bgMusic->SetLoop(true);
        bgMusic->SetVolume(50);
        bgMusic->Stop(); //CHANG MEEEEEEEEEEEEEEEEEEE
    }

    while(reloadUIflag) {
        reloadUIflag = false;
        exitCode = EXIT_RELAUNCH_ON_LOAD;
        DEBUG_FUNCTION_LINE("Initialize the language\n");
        loadLanguageFromConfig();
        DEBUG_FUNCTION_LINE("Initialize main window\n");
        mainWindow = MainWindow::getInstance(video->getTvWidth(), video->getTvHeight());

        DEBUG_FUNCTION_LINE("Entering main loop\n");
        exitApplication = false;
        //! main GX2 loop (60 Hz cycle with max priority on core 1)

        TcpReceiver pluginReceiver(4299);
        while(!exitApplication && !reloadUIflag) {
            //! Read out inputs
            for(s32 i = 0; i < 5; i++) {
                if(controller[i]->update(video->getTvWidth(), video->getTvHeight()) == false)
                    continue;

                if(controller[i]->data.buttons_d & VPAD_BUTTON_PLUS) {
                    exitCode = APPLICATION_CLOSE_APPLY;
                    exitApplication = true;
                }

                if(controller[i]->data.buttons_d & VPAD_BUTTON_HOME) {
                    exitCode = APPLICATION_CLOSE_MIIMAKER;
                    exitApplication = true;
                }

                //! update controller states
                mainWindow->update(controller[i]);
            }
            mainWindow->process();

            //! start rendering DRC
            video->prepareDrcRendering();
            mainWindow->drawDrc(video);
            video->drcDrawDone();

            //! start rendering TV
            video->prepareTvRendering();
            mainWindow->drawTv(video);
            video->tvDrawDone();

            //! enable screen after first frame render
            if(video->getFrameCount() == 0) {
                video->tvEnable(true);
                video->drcEnable(true);
            }

            //! as last point update the effects as it can drop elements
            mainWindow->updateEffects();

            video->waitForVSync();

            //! transfer elements to real delete list here after all processes are finished
            //! the elements are transfered to another list to delete the elements in a separate thread
            //! and avoid blocking the GUI thread
            AsyncDeleter::triggerDeleteProcess();
        }
        DEBUG_FUNCTION_LINE("fadeOut\n");
        fadeOut();
        DEBUG_FUNCTION_LINE("MainWindow::destroyInstance();\n");
        MainWindow::destroyInstance();
    }
    DEBUG_FUNCTION_LINE("Delete fontSystem\n");
    delete fontSystem;
    DEBUG_FUNCTION_LINE("Delete video\n");
    delete video;
}

void Application::loadLanguageFromConfig() {
    if(!CSettings::getValueAsString(CSettings::AppLanguage).empty()) {
        std::string languagePath = std::string(DEFAULT_LANG_PATH) + "/" + CSettings::getValueAsString(CSettings::AppLanguage) + std::string(LANGUAGE_FILE_EXT);
        gettextLoadLanguage(languagePath.c_str());
    }
}
