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
#include "DefaultGuiSwitch.h"
/**
 * Constructor for the DefaultGuiSwitch class.
 */

DefaultGuiSwitch::DefaultGuiSwitch(bool checked)
 : GuiSwitch(NULL,checked)
 ,switchbase_imgdata(Resources::GetImageData("switchIconBase.png"))
 ,switchbase_img(switchbase_imgdata)
 ,switchbase_highlighted_imgdata(Resources::GetImageData("switchIconBaseHighlighted.png"))
 ,switchbase_highlighted_img(switchbase_highlighted_imgdata)
 ,switchOn_imgdata(Resources::GetImageData("switchIconOn.png"))
 ,switchOn_img(switchOn_imgdata)
 ,switchOff_imgdata(Resources::GetImageData("switchIconOff.png"))
 ,switchOff_img(switchOff_imgdata)
{
    setSize(switchbase_img.getWidth(),switchbase_img.getHeight());
    this->setImageBackground(&switchbase_img);
    this->setImageHighlighted(&switchbase_highlighted_img);
    this->setImageOn(&switchOn_img);
    this->setImageOff(&switchOff_img);
}
/**
 * Destructor for the DefaultGuiSwitch class.
 */
DefaultGuiSwitch::~DefaultGuiSwitch()
{
    Resources::RemoveImageData(switchbase_imgdata);
    Resources::RemoveImageData(switchbase_highlighted_imgdata);
    Resources::RemoveImageData(switchOn_imgdata);
    Resources::RemoveImageData(switchOff_imgdata);
}
