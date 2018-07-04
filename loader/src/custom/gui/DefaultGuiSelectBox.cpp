/****************************************************************************
 * Copyright (C) 2017 Maschell
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
#include "DefaultGuiSelectBox.h"
/**
 * Constructor for the DefaultGuiSelectBox class.
 */

DefaultGuiSelectBox::DefaultGuiSelectBox(std::string caption, GuiFrame *parent)
 : GuiSelectBox(NULL,caption)
 ,topBackgroundImg_imgdata(Resources::GetImageData("gameSettingsButton.png"))
 ,topBackgroundImg_img(topBackgroundImg_imgdata)
 ,topHighlightedImg_imgdata(Resources::GetImageData("gameSettingsButtonSelected.png"))
 ,topHighlightedImg_img(topHighlightedImg_imgdata)
 ,valueImageData(Resources::GetImageData("gameSettingsButtonEx.png"))
 ,valueSelectedImageData(Resources::GetImageData("gameSettingsButtonExSelected.png"))
 ,valueHighlightedImageData(Resources::GetImageData("gameSettingsButtonExHighlighted.png"))
 ,buttonClickSound(Resources::GetSound("settings_click_2.mp3"))
{
    setSize(topBackgroundImg_img.getWidth(),topBackgroundImg_img.getHeight());
    this->setImageTopBackground(&topBackgroundImg_img);
    this->setImageTopHighlighted(&topHighlightedImg_img);
    this->setImageValueBackground(valueImageData);
    this->setImageValueHighlighted(valueHighlightedImageData);
    this->setImageValueSelected(valueSelectedImageData);
    this->setSoundClick(buttonClickSound);
}
/**
 * Destructor for the DefaultGuiSelectBox class.
 */
DefaultGuiSelectBox::~DefaultGuiSelectBox()
{
    Resources::RemoveImageData(topBackgroundImg_imgdata);
    Resources::RemoveImageData(topHighlightedImg_imgdata);
    Resources::RemoveImageData(valueImageData);
    Resources::RemoveImageData(valueSelectedImageData);
    Resources::RemoveImageData(valueHighlightedImageData);
    Resources::RemoveSound(buttonClickSound);
}
