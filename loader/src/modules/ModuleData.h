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

#ifndef _MODULE_DATA_H_
#define _MODULE_DATA_H_

#include <string>
#include <vector>
#include "EntryData.h"
#include "HookData.h"
#include "ModuleInformation.h"
#include <utils/logger.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libelf.h>

#ifdef __cplusplus
}
#endif

class ModuleData{
    public:
        ModuleData(ModuleInformation * moduleInformation){
            this->moduleInformation = moduleInformation;
        }

        ~ModuleData(){
            for(size_t i = 0;i< entry_data_list.size();i++){
                if(entry_data_list[i] != NULL){
                    delete entry_data_list[i];
                }
            }

            for(size_t i = 0;i< hook_data_list.size();i++){
                if(hook_data_list[i] != NULL){
                    delete hook_data_list[i];
                }
            }
        }

        void addEntryData(EntryData * entry_data){
            entry_data_list.push_back(entry_data);
        }

        std::vector<EntryData *> getEntryDataList(){
            return entry_data_list;
        }

        void addHookData(HookData * hook_data){
            hook_data_list.push_back(hook_data);
        }

        std::vector<HookData *> getHookDataList(){
            return hook_data_list;
        }

        ModuleInformation * getModuleInformation(){
            return moduleInformation;
        }

    private:

        ModuleInformation * moduleInformation;

        std::vector<EntryData *> entry_data_list;
        std::vector<HookData *> hook_data_list;
};


#endif
