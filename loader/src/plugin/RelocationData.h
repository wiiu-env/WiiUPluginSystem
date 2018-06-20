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

#ifndef _RELOCATION_DATA_H_
#define _RELOCATION_DATA_H_

#include <wups.h>
#include <string>
#include "ImportRPLInformation.h"

class RelocationData {

public:
    RelocationData(char type, size_t offset, int32_t addend, void *destination, std::string name, ImportRPLInformation * rplInfo) {
        this->type = type;
        this->offset = offset;
        this->addend = addend;
        this->destination = destination;
        this->name = name;
        this->rplInfo = rplInfo;
    }

    ~RelocationData() {

    }

    char getType() {
        return type;
    }

    size_t getOffset() {
        return offset;
    }

    int32_t getAddend() {
        return addend;
    }

    void * getDestination() {
        return destination;
    }

    std::string getName() {
        return name;
    }

    ImportRPLInformation * getImportRPLInformation() {
        return rplInfo;
    }

    void printInformation() {
        DEBUG_FUNCTION_LINE("%s destination: %08X offset: %08X type: %02X addend: %d rplName: %s isData: %d \n",name.c_str(), destination, offset, type, addend, rplInfo->getName().c_str(), rplInfo->isData() );
    }

private:
    char type;
    size_t offset;
    int32_t addend;
    void *destination;
    std::string name;
    ImportRPLInformation * rplInfo;
};


#endif
