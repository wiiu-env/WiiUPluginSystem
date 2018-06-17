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
    for(int i = 0; i < DYN_LINK_FUNCTION_LIST_LENGTH; i++) {
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
    for(int i = 0; i < DYN_LINK_IMPORT_LIST_LENGTH; i++) {
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

bool DynamicLinkingHelper::addReloationEntry(RelocationData * relocationData){
    return addReloationEntry(relocationData->getType(), relocationData->getOffset(), relocationData->getAddend(), relocationData->getDestination(), relocationData->getName(), relocationData->getImportRPLInformation());
}

bool DynamicLinkingHelper::addReloationEntry(char type, size_t offset, int addend, void *destination, std::string name, ImportRPLInformation * rplInfo){
    dyn_linking_import_t * importInfoGbl = DynamicLinkingHelper::getOrAddImport(rplInfo->getName().c_str(),rplInfo->isData());
    if(importInfoGbl == NULL){
        DEBUG_FUNCTION_LINE("Getting import info failed. Probably maximum of %d rpl files to import reached.\n",DYN_LINK_IMPORT_LIST_LENGTH);
        return false;
    }

    dyn_linking_function_t * functionInfo = DynamicLinkingHelper::getOrAddFunctionEntryByName(name.c_str());
    if(functionInfo == NULL){
        DEBUG_FUNCTION_LINE("Getting import info failed. Probably maximum of %d function to be relocated reached.\n",DYN_LINK_FUNCTION_LIST_LENGTH);
        return false;
    }

    return addReloationEntry(type, offset, addend, destination, functionInfo, importInfoGbl);
}

bool DynamicLinkingHelper::addReloationEntry(char type, size_t offset, int addend, void *destination, dyn_linking_function_t * functionName, dyn_linking_import_t * importInfo){
     for(int i = 0; i < DYN_LINK_RELOCATION_LIST_LENGTH; i++) {
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
    for(int i = 0; i < DYN_LINK_RELOCATION_LIST_LENGTH; i++) {
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
            int isData = 0;
            if(importEntry->isData) {
                isData = 1;
                //DEBUG_FUNCTION_LINE("isData\n");
            }
            OSDynLoad_FindExport(importEntry->handle, isData, functionEntry->functionName, &functionEntry->address);

            if(!functionEntry->address) {
                DEBUG_FUNCTION_LINE("OSDynLoad_FindExport failed for %s on import %s\n", functionEntry->functionName,importEntry->importName);
                return false;
            }else{
                //DEBUG_FUNCTION_LINE("OSDynLoad_FindExport successful for %s on import %s: %08X\n",functionEntry->functionName,importEntry->importName,functionEntry->address);
            }
        } else {
            //DEBUG_FUNCTION_LINE("We cached the address of function %s :%08X\n",functionEntry->functionName,functionEntry->address);
        }
        //DEBUG_FUNCTION_LINE("We would link now: type: %02X offset: %08X addend: %d destination: %08X target: %08X for %s\n",curEntry->type, curEntry->offset, curEntry->addend, curEntry->destination, functionEntry->address,functionEntry->functionName);
        DEBUG_FUNCTION_LINE("Resolving relocation to %s\n",functionEntry->functionName);
        ElfTools::elfLinkOne(curEntry->type, curEntry->offset, curEntry->addend, curEntry->destination, (u32) functionEntry->address);
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
