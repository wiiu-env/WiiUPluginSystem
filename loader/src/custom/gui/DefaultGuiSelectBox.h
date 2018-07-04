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
#ifndef DEFAULT_GUI_SELECTBOX_H_
#define DEFAULT_GUI_SELECTBOX_H_

#include <gui/GuiSelectBox.h>

//!A simple select box with default values.
class DefaultGuiSelectBox : public GuiSelectBox{
	public:
		//!Constructor
		//!\param checked Checked
		DefaultGuiSelectBox(std::string caption, GuiFrame *parent = NULL);
		//!Destructor
		virtual ~DefaultGuiSelectBox();

	protected:
        GuiImageData * topBackgroundImg_imgdata;
        GuiImage topBackgroundImg_img;

        GuiImageData * topHighlightedImg_imgdata;
        GuiImage topHighlightedImg_img;

        GuiImageData * valueImageData;
        GuiImageData * valueSelectedImageData;
        GuiImageData * valueHighlightedImageData;

        GuiSound * buttonClickSound;
};

#endif
