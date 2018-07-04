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
#ifndef DEFAULT_GUI_SWTICH_H_
#define DEFAULT_GUI_SWTICH_H_

#include <gui/GuiSwitch.h>

//!A simple switch
class DefaultGuiSwitch : public GuiSwitch{
	public:
		//!Constructor
		//!\param checked Checked
		DefaultGuiSwitch(bool checked);
		//!Destructor
		virtual ~DefaultGuiSwitch();

	protected:
        GuiImageData * switchbase_imgdata = NULL;
        GuiImage switchbase_img;

        GuiImageData * switchbase_highlighted_imgdata = NULL;
        GuiImage switchbase_highlighted_img;

        GuiImageData * switchOn_imgdata = NULL;
        GuiImage switchOn_img;

        GuiImageData * switchOff_imgdata = NULL;
        GuiImage switchOff_img;
};

#endif
