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
#include "ContentHome.h"

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
{
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
    welcomeHeadLineLabel.setFontSize(40);
    welcomeHeadLineLabel.setPosition(-330,-50);

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

    append(&welcomeHeadLineLabel);
    append(&twitterLogoImage);
    append(&githubLogoImage);
    append(&twitterLabel);
    append(&URLLabel);
    append(&exitHomeFrame);
    append(&exitPlusFrame);
}

ContentHome::~ContentHome(){
    //Resources::RemoveImageData(logoImageData);  <-- will reduce lag. Will be deleted at the end anyway
    remove(&bgImageColor);
    remove(&welcomeHeadLineLabel);
    remove(&twitterLogoImage);
    remove(&githubLogoImage);
    remove(&twitterLabel);
    remove(&URLLabel);
    remove(&exitHomeFrame);
    remove(&exitPlusFrame);
}
