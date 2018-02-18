/* based on module.c
 *   by Alex Chadwick
 *
 * Copyright (C) 2014, Alex Chadwick
 * Modified 2018, Maschell
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _MODULE_LOADER_H_
#define _MODULE_LOADER_H_

#include <vector>
#include "ModuleInformation.h"
#include "ModuleData.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <utils/utils.h>

#ifdef __cplusplus
}
#endif


#define PLUGIN_LOCATION_END_ADDRESS 0x01000000

class ModuleLoader{

public:
    static ModuleLoader *getInstance() {
        if(!instance){
            instance = new ModuleLoader((void*)getApplicationEndAddr(),(void *)PLUGIN_LOCATION_END_ADDRESS);
        }
        return instance;
    }

    static void destroyInstance() {
        if(instance){
            delete instance;
            instance = NULL;
        }
    }

    /**
        \brief Parses the meta data of all plugins in the given directory.

        \param path the path of the directory which should be scanned.

        \return a list of ModuleInformation objects, one for each valid plugin.
    **/
    std::vector<ModuleInformation *> getModuleInformation(const char * path);

    /**
        \brief Gets plugin information from the global struct.

        \return a list of MetaInformation objects for all plugins currently loaded and linked (relocated). Will only contain
                plugin which are still on the sd card.
    **/
    //std::vector<ModuleInformation *> getModulesLoadedInMemory();

    /**
        \brief  Takes a list of modules that should be linked (relocated) loaded into the memory.
                The function that should be replaced will be replaced in the order of the given plugin list.
                So two plugin will override the same function, the plugin first in this list will override the function first.
                Also the hooks of the plugins will be called in the order their plugin where passed to this method.

        \param A list of plugin that should be linked (relocated) an loaded into memory
    **/
    void loadAndLinkModules(std::vector<ModuleInformation *> moduleInformation);
private:
    ModuleLoader(void * startAddress, void * endAddress){
        // TODO: Check if endAddress > startAddress.
        this->startAddress = startAddress;
        this->endAddress = endAddress;
        this->currentStoreAddress = endAddress;
    }

    ~ModuleLoader(){

    }

    static ModuleLoader *instance;

    /**
        \brief  Iterates through the vector and delete all it's elements

        \param A list of ModuleData* that should be deleted.
    **/
    void clearModuleData(std::vector<ModuleData *> moduleData);

    /**
        \brief Load

        \param moduleInformation a ModuleInformation object of the plugin that should be linked (relocated) and loaded.

        \return NULL on error. On success it will return a ModuleData object.
    **/
    ModuleData * loadAndLinkModule(ModuleInformation * moduleInformation);

    /**
        \brief  Loads a plugin into memory (in the startAddress/endAddress range defined in this loader) and relocates it.
                Modifies the moduleData param. Adds loaded functions and hooks.
        \param moduleData object where the result should be stored
        \param elf source elf from where the sections will be loaded
        \param storeAddressEnd the address where the plugin data will be stored in memory. Saving BACKWARD.

    **/
    bool loadAndLinkElf(ModuleData * moduleData, Elf *elf, void * storeAddressEnd);

     /**
        \brief  Copies the needed information into a global, persistent struct. This struct holds information on which
                function should be override in which order and which hook should be called.
        \param modules list of modules that should be used.

    **/
    void copyModuleDataIntoGlobalStruct(std::vector<ModuleData *> modules);

    void * getCurrentStoreAddress(){
        return this->currentStoreAddress;
    }

    void setCurrentStoreAddress(void *  addr){
        this->currentStoreAddress = addr;
    }

    void * startAddress = NULL;
    void * endAddress = NULL;
    void * currentStoreAddress = NULL;
};


#endif
