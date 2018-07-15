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
#include "common/common.h"
#include "ContentHome.h"
#include "plugin/PluginLoader.h"
#include "custom/gui/DefaultGuiSwitch.h"
#include "Application.h"

ContentHome::ContentHome():ContentTemplate()
    , welcomeHeadLineLabel(gettext("Welcome to the Wii U plugin loader"))
    , twitterLogoImageData(Resources::GetImageData("TwitterIcon.png"))
    , twitterLogoImage(twitterLogoImageData)
    , githubLogoImageData(Resources::GetImageData("GithubIcon.png"))
    , githubLogoImage(githubLogoImageData)
    , twitterLabel("MaschellDev")
    , URLLabel("github.com/Maschell/WiiUPluginSystem")
    , homebutton_imgdata(Resources::GetImageData("HomeButtonIcon.png"))
    , homebutton_img(homebutton_imgdata)
    , exitHome(gettext("Exit to HBL  "))
    , plusbutton_imgdata(Resources::GetImageData("PlusButtonIcon.png"))
    , plusbutton_img(plusbutton_imgdata)
    , exitPlus(gettext("Apply Patches"))
    , touchTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::VPAD_TOUCH)
    , wpadTouchTrigger(GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5, GuiTrigger::BUTTON_A)
    , buttonClickSound(Resources::GetSound("settings_click_2.mp3"))
    , buttonATrigger(GuiTrigger::CHANNEL_ALL, GuiTrigger::BUTTON_A, true)
    , buttonUpTrigger(GuiTrigger::CHANNEL_ALL, GuiTrigger::BUTTON_UP | GuiTrigger::STICK_L_UP, true)
    , buttonDownTrigger(GuiTrigger::CHANNEL_ALL, GuiTrigger::BUTTON_DOWN | GuiTrigger::STICK_L_DOWN, true)
    , DPADButtons(0,0) {
    glm::vec4 textColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);

    homebutton_img.setAlignment(ALIGN_LEFT);
    exitHome.setPosition(46,-8);
    exitHome.setFontSize(35);
    exitHome.setColor(textColor);
    exitHome.setAlignment(ALIGN_LEFT);
    exitHomeFrame.setAlignment(ALIGN_BOTTOM|ALIGN_RIGHT);
    exitHomeFrame.setPosition(-250,40);
    exitHomeFrame.append(&homebutton_img);
    exitHomeFrame.append(&exitHome);
    exitHomeFrame.setScale(0.7f);

    plusbutton_img.setAlignment(ALIGN_LEFT);
    exitPlus.setPosition(46,-8);
    exitPlus.setFontSize(35);
    exitPlus.setColor(textColor);
    exitPlus.setAlignment(ALIGN_LEFT);
    exitPlusFrame.setAlignment(ALIGN_BOTTOM|ALIGN_RIGHT);
    exitPlusFrame.setPosition(-250,80);
    exitPlusFrame.append(&plusbutton_img);
    exitPlusFrame.append(&exitPlus);
    exitPlusFrame.setScale(0.7f);

    welcomeHeadLineLabel.setColor(textColor);
    welcomeHeadLineLabel.setAlignment(ALIGN_MIDDLE|ALIGN_TOP);
    welcomeHeadLineLabel.setFontSize(50);
    welcomeHeadLineLabel.setPosition(-370,-50);

    twitterLogoImage.setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
    twitterLogoImage.setPosition(10,20);
    twitterLabel.setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
    twitterLabel.setColor(textColor);
    twitterLabel.setPosition(52,50);

    githubLogoImage.setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
    githubLogoImage.setPosition(240,20);

    URLLabel.setColor(textColor);
    URLLabel.setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
    URLLabel.setPosition(280,50);

    DPADButtons.setTrigger(&buttonATrigger);
    DPADButtons.setTrigger(&buttonDownTrigger);
    DPADButtons.setTrigger(&buttonUpTrigger);
    DPADButtons.clicked.connect(this, &ContentHome::OnDPADClick);
    append(&DPADButtons);

    PluginLoader * pluginLoader  = PluginLoader::getInstance();
    std::vector<PluginInformation *> pluginList = pluginLoader->getPluginInformation(WUPS_PLUGIN_PATH);
    std::vector<PluginInformation *> pluginListLoaded = pluginLoader->getPluginsLoadedInMemory();

    pluginsFrame.setAlignment(ALIGN_TOP_CENTER);
    pluginsFrame.setPosition(0,-80);
    pluginsFrame.setSize(getWidth(),getHeight());
    append(&pluginsFrame);

    float frameoffset = 0;
    float frameheight = 50.0f;
    int32_t selectionMappingIndex = 0;
    selectionMappingMin = 0;
    for (std::vector<PluginInformation *>::iterator it = pluginList.begin() ; it != pluginList.end(); ++it) {
        PluginInformation * curPlugin = *it;

        DefaultGuiSwitch * element = new DefaultGuiSwitch(false);
        selectionMapping[selectionMappingIndex] = element;
        selectionMappingIndex++;

        element->setTrigger(&touchTrigger);
        element->setTrigger(&wpadTouchTrigger);
        element->setSoundClick(buttonClickSound);
        element->valueChanged.connect(this, &ContentHome::OnValueChanged);

        for (std::vector<PluginInformation *>::iterator itOther = pluginListLoaded.begin() ; itOther != pluginListLoaded.end(); ++itOther) {
            PluginInformation * otherPlugin = *itOther;
            if(otherPlugin->getPath().compare(curPlugin->getPath()) == 0) {
                element->setValue(true);
            }
        }
        pluginMapping[element] = curPlugin;

        GuiFrame * frame = new GuiFrame();
        GuiFrame * left = new GuiFrame();
        GuiFrame * right = new GuiFrame();

        frame->append(left);
        frame->append(right);
        frame->setAlignment(ALIGN_TOP_CENTER);
        frame->setSize(getWidth()*0.65f,frameheight);

        GuiText * text = new GuiText(curPlugin->getName().c_str());
        text->setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
        text->setFontSize(40);
        text->setColor(textColor);
        text->setAlignment(ALIGN_LEFT);
        left->append(text);
        left->setAlignment(ALIGN_LEFT);
        right->setAlignment(ALIGN_RIGHT);
        element->setAlignment(ALIGN_RIGHT);
        element->setPosition(0,10);
        element->setScale(2.0f);

        right->append(element);

        frameoffset -= frameheight;
        frame->setPosition(0,frameoffset);
        pluginsFrame.append(frame);

        toDelete.push_back(element);
        toDelete.push_back(frame);
        toDelete.push_back(left);
        toDelete.push_back(right);
        toDelete.push_back(text);
    }
    selectionMappingMax = selectionMapping.size() -1;
    if(selectionMappingMax < 0) {
        selectionMappingMax = 0;
    }

    pluginLoader->clearPluginInformation(pluginListLoaded);

    append(&welcomeHeadLineLabel);
    append(&twitterLogoImage);
    append(&githubLogoImage);
    append(&twitterLabel);
    append(&URLLabel);
    append(&exitHomeFrame);
    append(&exitPlusFrame);

    auto fp = std::bind(&ContentHome::linkPlugins, this);
    Application::instance()->setLinkPluginsCallback(fp);
}

void ContentHome::OnDPADClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger) {
    if(trigger == &buttonATrigger) {
        if((controller->chan & (GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5)) && controller->data.validPointer) {
            return;
        }
        GuiToggle * toggleElement = selectionMapping[selectionMappingCur];
        if(toggleElement != NULL) {
            buttonClickSound->Play();
            DEBUG_FUNCTION_LINE("Pressed a for %d\n",selectionMappingCur);
            toggleElement->setValue(!toggleElement->getValue());
        }
    } else if(trigger == &buttonUpTrigger || trigger == &buttonDownTrigger) {
        if(selectionMappingCur == -1) {
            selectionMappingCur = selectionMappingMin;
        } else {
            if(trigger == &buttonUpTrigger) {
                if(selectionMappingCur > 0) {
                    selectionMappingCur--;
                } else {
                    selectionMappingCur = selectionMappingMax;
                }
            } else if(trigger == &buttonDownTrigger) {
                if(selectionMappingCur < selectionMappingMax) {
                    selectionMappingCur++;
                } else {
                    selectionMappingCur = 0;
                }
            }
        }
        DEBUG_FUNCTION_LINE("%d\n",selectionMappingCur);
        updateButtons = true;
    }

}

void ContentHome::OnValueChanged(GuiToggle * toggle,bool value) {

}

bool ContentHome::linkPlugins() {
    std::vector<PluginInformation*> willBeLoaded;

    for (auto const& x : pluginMapping) {
        GuiToggle * guiElement = x.first;
        if(guiElement->getValue()) {
            PluginInformation* pluginInformation = x.second;
            DEBUG_FUNCTION_LINE("We want to link %s\n",pluginInformation->getName().c_str());
            willBeLoaded.push_back(pluginInformation);
        }
    }

    return PluginLoader::getInstance()->loadAndLinkPlugins(willBeLoaded);
}


void ContentHome::update(GuiController * c) {
    ContentTemplate::update(c);
    if(updateButtons){
        for (auto const& x : selectionMapping) {
            int index = x.first;
            GuiToggle* toggle = x.second;
            if(toggle != NULL) {
                if(index != selectionMappingCur) {
                    toggle->clearState(STATE_SELECTED);
                } else {
                    toggle->setState(STATE_SELECTED);
                }
            }
        }
        updateButtons = false;
    }
}

ContentHome::~ContentHome() {
    //Resources::RemoveImageData(logoImageData);  <-- will reduce lag. Will be deleted at the end anyway
    remove(&bgImageColor);
    remove(&welcomeHeadLineLabel);
    remove(&twitterLogoImage);
    remove(&githubLogoImage);
    remove(&twitterLabel);
    remove(&URLLabel);
    remove(&exitHomeFrame);
    remove(&exitPlusFrame);

    for (auto const& x : pluginMapping) {
        // x.first is also in the toDelete vector.
        PluginInformation* pluginInformation = x.second;
        delete pluginInformation;
    }

    Resources::RemoveSound(buttonClickSound);
    for (std::vector<GuiElement *>::iterator it = toDelete.begin() ; it != toDelete.end(); ++it) {
        delete *it;
    }
}
