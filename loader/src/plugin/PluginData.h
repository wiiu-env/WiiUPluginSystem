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

#ifndef _PLUGIN_DATA_H_
#define _PLUGIN_DATA_H_

#include <string>
#include <vector>
#include "FunctionData.h"
#include "HookData.h"
#include "PluginInformation.h"
#include "RelocationData.h"
#include "ImportRPLInformation.h"
#include <utils/logger.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libelf.h>

#ifdef __cplusplus
}
#endif

class PluginData {
public:
    PluginData(PluginInformation * pluginInformation) {
        this->pluginInformation = pluginInformation;
    }

    ~PluginData() {
        for(size_t i = 0; i< function_data_list.size(); i++) {
            if(function_data_list[i] != NULL) {
                delete function_data_list[i];
            }
        }

        for(size_t i = 0; i< hook_data_list.size(); i++) {
            if(hook_data_list[i] != NULL) {
                delete hook_data_list[i];
            }
        }

        for(size_t i = 0; i< relocation_data_list.size(); i++) {
            if(relocation_data_list[i] != NULL) {
                delete relocation_data_list[i];
            }
        }

        for(size_t i = 0; i< importRPLInformation_list.size(); i++) {
            if(importRPLInformation_list[i] != NULL) {
                delete importRPLInformation_list[i];
            }
        }
    }

    void addFunctionData(FunctionData * function_data) {
        function_data_list.push_back(function_data);
    }

    std::vector<FunctionData *> getFunctionDataList() {
        return function_data_list;
    }

    void addHookData(HookData * hook_data) {
        hook_data_list.push_back(hook_data);
    }

    std::vector<HookData *> getHookDataList() {
        return hook_data_list;
    }

    void addRelocationData(RelocationData * relocation_data) {
        relocation_data_list.push_back(relocation_data);
    }

    std::vector<RelocationData *> getRelocationDataList() {
        return relocation_data_list;
    }

    void addImportRPLInformation(ImportRPLInformation * importRPLInformation) {
        importRPLInformation_list.push_back(importRPLInformation);
    }

    std::vector<ImportRPLInformation *> getImportRPLInformationList() {
        return importRPLInformation_list;
    }

    /**
        Returns a ImportRPLInformation for a given section header index.

        \param section_header_index: ID of section in elf, started counting at 1.

        \return A pointer to the corresponding ImportRPLInformation, return NULL if no corresponding information was found.
    **/
   ImportRPLInformation * getImportRPLInformationBySectionHeaderIndex(int section_header_index) {
        for(size_t i = 0; i< importRPLInformation_list.size(); i++) {
            if(importRPLInformation_list[i] != NULL && importRPLInformation_list[i]->getSectionHeaderIndex() == section_header_index) {
                return importRPLInformation_list[i];
            }
        }
        return NULL;
    }

    PluginInformation * getPluginInformation() {
        return pluginInformation;
    }

private:

    PluginInformation * pluginInformation;

    std::vector<FunctionData *> function_data_list;
    std::vector<HookData *> hook_data_list;
    std::vector<RelocationData *> relocation_data_list;
    std::vector<ImportRPLInformation *> importRPLInformation_list;
};


#endif
