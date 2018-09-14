#include "DynamicLinkingHelper.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include "utils/logger.h"
#include "ElfTools.h"
#include "PluginLoader.h"

DynamicLinkingHelper * DynamicLinkingHelper::instance = NULL;

dyn_linking_function_t * DynamicLinkingHelper::getOrAddFunctionEntryByName(const char* functionName) {
    if(functionName == NULL) {
        return NULL;
    }
    dyn_linking_function_t * result = NULL;
    for(int32_t i = 0; i < DYN_LINK_FUNCTION_LIST_LENGTH; i++) {
        dyn_linking_function_t * curEntry = &gbl_dyn_linking_data.functions[i];
        if(strlen(curEntry->functionName) == 0) {
            strncpy(curEntry->functionName,functionName,DYN_LINK_FUNCTION_NAME_LENGTH);
            result = curEntry;
            break;
        }
        if(strncmp(curEntry->functionName,functionName,DYN_LINK_FUNCTION_NAME_LENGTH) == 0) {
            result = curEntry;
            break;
        }
    }
    return result;
}

dyn_linking_import_t * DynamicLinkingHelper::getOrAddFunctionImportByName(const char* importName) {
    return getOrAddImport(importName, false);

}

dyn_linking_import_t * DynamicLinkingHelper::getOrAddDataImportByName(const char* importName) {
    return getOrAddImport(importName, true);

}

dyn_linking_import_t * DynamicLinkingHelper::getOrAddImport(const char* importName, bool isData) {
    if(importName == NULL) {
        return NULL;
    }
    dyn_linking_import_t * result = NULL;
    for(int32_t i = 0; i < DYN_LINK_IMPORT_LIST_LENGTH; i++) {
        dyn_linking_import_t * curEntry = &gbl_dyn_linking_data.imports[i];
        if(strlen(curEntry->importName) == 0) {
            strncpy(curEntry->importName,importName,DYN_LINK_IMPORT_NAME_LENGTH);
            curEntry->isData = isData;
            result = curEntry;
            break;
        }
        if(strncmp(curEntry->importName,importName,DYN_LINK_IMPORT_NAME_LENGTH) == 0 && (curEntry->isData == isData)) {
            return curEntry;
        }
    }
    return result;
}

bool DynamicLinkingHelper::addReloationEntry(RelocationData * relocationData) {
    return addReloationEntry(relocationData->getType(), relocationData->getOffset(), relocationData->getAddend(), relocationData->getDestination(), relocationData->getName(), relocationData->getImportRPLInformation());
}

bool DynamicLinkingHelper::addReloationEntry(char type, size_t offset, int32_t addend, void *destination, std::string name, ImportRPLInformation * rplInfo) {
    dyn_linking_import_t * importInfoGbl = DynamicLinkingHelper::getOrAddImport(rplInfo->getName().c_str(),rplInfo->isData());
    if(importInfoGbl == NULL) {
        DEBUG_FUNCTION_LINE("Getting import info failed. Probably maximum of %d rpl files to import reached.\n",DYN_LINK_IMPORT_LIST_LENGTH);
        return false;
    }

    dyn_linking_function_t * functionInfo = DynamicLinkingHelper::getOrAddFunctionEntryByName(name.c_str());
    if(functionInfo == NULL) {
        DEBUG_FUNCTION_LINE("Getting import info failed. Probably maximum of %d function to be relocated reached.\n",DYN_LINK_FUNCTION_LIST_LENGTH);
        return false;
    }

    return addReloationEntry(type, offset, addend, destination, functionInfo, importInfoGbl);
}

bool DynamicLinkingHelper::addReloationEntry(char type, size_t offset, int32_t addend, void *destination, dyn_linking_function_t * functionName, dyn_linking_import_t * importInfo) {
    for(int32_t i = 0; i < DYN_LINK_RELOCATION_LIST_LENGTH; i++) {
        dyn_linking_relocation_entry_t * curEntry = &gbl_dyn_linking_data.entries[i];
        if(curEntry->functionEntry != NULL) {
            continue;
        }
        curEntry->type = type;
        curEntry->offset = offset;
        curEntry->addend = addend;
        curEntry->destination = destination;
        curEntry->functionEntry = functionName;
        curEntry->importEntry = importInfo;

        return true;
    }
    return false;
}

std::vector<dyn_linking_relocation_entry_t *> DynamicLinkingHelper::getAllValidDynamicLinkingRelocations() {
    std::vector<dyn_linking_relocation_entry_t *> result;
    for(int32_t i = 0; i < DYN_LINK_RELOCATION_LIST_LENGTH; i++) {
        if(gbl_dyn_linking_data.entries[i].functionEntry == NULL) {
            break;
        }
        result.push_back(&gbl_dyn_linking_data.entries[i]);
    }
    return result;
}

bool DynamicLinkingHelper::fillRelocations(std::vector<dyn_linking_relocation_entry_t *> entries ) {
    for(size_t i = 0; i < entries.size(); i++) {
        dyn_linking_relocation_entry_t * curEntry = entries[i];
        dyn_linking_function_t * functionEntry = curEntry->functionEntry;
        void * destination = curEntry->destination;
        if(functionEntry == NULL) {
            DEBUG_FUNCTION_LINE("FunctionEntry was NULL\n");
            return false;
        }
        if(destination == NULL) {
            DEBUG_FUNCTION_LINE("destination was NULL\n");
            return false;
        }
        if(functionEntry->address == NULL) {
            dyn_linking_import_t * importEntry = curEntry->importEntry;

            if(importEntry == NULL) {
                DEBUG_FUNCTION_LINE("importEntry was NULL\n");
                return false;
            }

            if(importEntry->handle != 0) {
                //DEBUG_FUNCTION_LINE("We cached import handle for %s\n",importEntry->importName);
            } else {
                OSDynLoad_Acquire(importEntry->importName, &importEntry->handle);
            }
            int32_t isData = 0;
            if(importEntry->isData) {
                isData = 1;
                //DEBUG_FUNCTION_LINE("isData\n");
            }
            OSDynLoad_FindExport(importEntry->handle, isData, functionEntry->functionName, &functionEntry->address);

            if(!functionEntry->address) {
                DEBUG_FUNCTION_LINE("OSDynLoad_FindExport failed for %s on import %s\n", functionEntry->functionName,importEntry->importName);
                return false;
            } else {
                //DEBUG_FUNCTION_LINE("OSDynLoad_FindExport successful for %s on import %s: %08X\n",functionEntry->functionName,importEntry->importName,functionEntry->address);

                /*
                90 61 ff e0     stw     r3,-32(r1)
                3c 60 12 34     lis     r3,4660
                60 63 56 78     ori     r3,r3,22136
                7c 69 03 a6     mtctr   r3
                80 61 ff e0     lwz     r3,-32(r1)
                4e 80 04 20     bctr*/
                functionEntry->big_jump[0] = 0x9061FFE0;
                functionEntry->big_jump[1] = 0x3C600000 | ((((uint32_t) functionEntry->address) >> 16) & 0x0000FFFF); // lis r3, real_addr@h
                functionEntry->big_jump[2] = 0x60630000 |  (((uint32_t) functionEntry->address) & 0x0000ffff); // ori r3, r3, real_addr@l
                functionEntry->big_jump[3] = 0x7C6903A6; // mtctr   r3
                functionEntry->big_jump[4] = 0x8061FFE0; // lwz     r3,-32(r1)
                functionEntry->big_jump[5] = 0x4E800420; // bctr
                DCFlushRange((void*)functionEntry->big_jump, sizeof(functionEntry->big_jump));
                ICInvalidateRange((unsigned char*)functionEntry->big_jump, sizeof(functionEntry->big_jump));

                //DEBUG_FUNCTION_LINE("Created jump code address: %08X big_jump: %08X for %s \n",functionEntry->address,functionEntry->big_jump,functionEntry->functionName);
                //dumpHex(functionEntry->big_jump,sizeof(functionEntry->big_jump));
            }
        } else {
            //DEBUG_FUNCTION_LINE("We cached the address of function %s :%08X\n",functionEntry->functionName,functionEntry->address);
        }

        //DEBUG_FUNCTION_LINE("Linking: t: %02X o: %08X a: %d dest: %08X tar: %08X big_j: %08X\n",curEntry->type, curEntry->offset, curEntry->addend, curEntry->destination, functionEntry->address, (uint32_t) functionEntry->big_jump);

        DEBUG_FUNCTION_LINE("Resolving relocation to %s\n",functionEntry->functionName);

        if(!curEntry->importEntry->isData && (uint32_t) functionEntry->address > 0x01FFFFC) {
            ElfTools::elfLinkOne(curEntry->type, curEntry->offset, curEntry->addend, curEntry->destination, (uint32_t) functionEntry->big_jump);
        } else {
            ElfTools::elfLinkOne(curEntry->type, curEntry->offset, curEntry->addend, curEntry->destination, (uint32_t) functionEntry->address);
        }

    }

    PluginLoader::flushCache();

    DEBUG_FUNCTION_LINE("Clearing cache.\n");
    for(size_t i = 0; i < entries.size(); i++) {
        dyn_linking_relocation_entry_t * curEntry = entries[i];
        curEntry->functionEntry->address = 0;
        curEntry->importEntry->handle = 0;
    }
    return true;
}

void DynamicLinkingHelper::clearAll() {
    memset((void*)&gbl_dyn_linking_data,0,sizeof(gbl_dyn_linking_data));
}
