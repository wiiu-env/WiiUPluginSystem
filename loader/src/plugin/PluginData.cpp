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

#include <string>
#include <vector>
#include <map>
#include "PluginData.h"
#include "PluginLoader.h"

ImportRPLInformation * PluginData::getImportRPLInformationBySectionHeaderIndex(int32_t section_header_index) {
    for(size_t i = 0; i< importRPLInformation_list.size(); i++) {
        if(importRPLInformation_list[i] != NULL && importRPLInformation_list[i]->getSectionHeaderIndex() == section_header_index) {
            return importRPLInformation_list[i];
        }
    }
    return NULL;
}

uint32_t PluginData::getMemoryForCommonBySymbol(size_t symbol, size_t align, size_t size) {
    std::map<size_t,uint32_t>::iterator it = memoryBySymbol.find(symbol);
    if(it == memoryBySymbol.end()) {
        memoryBySymbol[symbol] = PluginLoader::getInstance()->getMemoryFromDataSection(align, size);
        DEBUG_FUNCTION_LINE("Setting value %08X\n",memoryBySymbol.find(symbol)->second);
    } else {
        DEBUG_FUNCTION_LINE("Using cached value %08X\n",memoryBySymbol.find(symbol)->second);
    }

    return memoryBySymbol.find(symbol)->second;
}
