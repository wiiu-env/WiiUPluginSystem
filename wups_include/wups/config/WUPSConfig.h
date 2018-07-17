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

#ifndef _WUPS_CONFIG_H_
#define _WUPS_CONFIG_H_

#include <string>
#include <vector>
#include <wups/config/WUPSConfigCategory.h>

class WUPSConfig {
public:
    WUPSConfig(std::string name) {
        this->name = name;
    }

    ~WUPSConfig() {
        for (auto & element : categories) {
            delete element;
        }
    }

    /**
        \return Returns the name of this WUPSConfig
    **/
    std::string getName(){
        return this->name;
    }

    /**
        \brief  Creates a new WUPSCategory add its to this WUPSConfig.
                The category will be added to the end of the list.
                This class holds responsibility for deleting the created instance.

        \param categoryName: The name of the category that will be created.

        \return On success, the created and inserted category will be returned.
    **/
    WUPSConfigCategory * addCategory(std::string categoryName){
        WUPSConfigCategory * curCat = new WUPSConfigCategory(categoryName);
        categories.push_back(curCat);
        return curCat;
    }

    /**
        \brief  Adds a given WUPSConfigCategory to this WUPSConfig.
                The category will be added to the end of the list.
                This class holds responsibility for deleting the created instance.

        \param category: The category that will be added to this config.

        \return On success, the inserted category will be returned.
                On error NULL will be returned. In this case the caller still has the responsibility
                for deleting the WUPSConfigCategory instance.
    **/
    WUPSConfigCategory * addCategory(WUPSConfigCategory * category){
        categories.push_back(category);
        return category;
    }

    /**
        \return Returns a vector with all categories.
    **/
    std::vector<WUPSConfigCategory *> getCategories(){
        return this->categories;
    }

private:
    std::string name;
    std::vector<WUPSConfigCategory *> categories;
};

#endif
