/****************************************************************************
 * Copyright (C) 2018 Maschell
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

#ifndef _WUPS_CONFIG_CATEGORY_H_
#define _WUPS_CONFIG_CATEGORY_H_

#include <string>
#include <vector>
#include <wups/config/WUPSConfigItem.h>

class WUPSConfigCategory {
public:
    WUPSConfigCategory(std::string name) {
        this->name = name;
    }

    ~WUPSConfigCategory() {
        for (auto & element : items) {
            delete element;
        }
    }

    /**
        \return Returns the name of this WUPSConfigCategory
    **/
    std::string getName(){
        return this->name;
    }

    /**
        \brief  Adds a given WUPSConfigItem to this WUPSConfigCategory.
                The item will be added to the end of the list.
                This class holds responsibility for deleting the created instance.

        \param item: The item that will be added to this config.

        \return On success, the inserted item will be returned.
                On error NULL will be returned. In this case the caller still has the responsibility
                for deleting the WUPSConfigItem instance.
    **/
    WUPSConfigItem * addItem(WUPSConfigItem * item){
        items.push_back(item);
        return item;
    }

    /**
        \return Returns a vector with all items.
    **/
    std::vector<WUPSConfigItem *> getItems(){
        return this->items;
    }

private:
    std::string name;
    std::vector<WUPSConfigItem *> items;
};

#endif
