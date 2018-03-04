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

#ifndef _PLUGIN_LOADER_H_
#define _PLUGIN_LOADER_H_

#include <vector>
#include "PluginInformation.h"
#include "PluginData.h"
#include "dynamic_libs/os_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <utils/utils.h>

#ifdef __cplusplus
}
#endif


#define PLUGIN_LOCATION_END_ADDRESS 0x01000000

class PluginLoader {

public:
    static PluginLoader *getInstance() {
        if(!instance) {
            instance = new PluginLoader((void*)getApplicationEndAddr(),(void *)PLUGIN_LOCATION_END_ADDRESS);
        }
        return instance;
    }

    static void destroyInstance() {
        if(instance) {
            delete instance;
            instance = NULL;
        }
    }

    /**
        \brief Parses the meta data of all plugins in the given directory.

        \param path the path of the directory which should be scanned.

        \return a list of PluginInformation objects, one for each valid plugin.
    **/
    std::vector<PluginInformation *> getPluginInformation(const char * path);

    /**
        \brief Gets plugin information from the global struct.

        \return a list of MetaInformation objects for all plugins currently loaded and linked (relocated). Will only contain
                plugin which are still on the sd card.
    **/
    std::vector<PluginInformation *> getPluginsLoadedInMemory();

    /**
        \brief  Takes a list of plugins that should be linked (relocated) loaded into the memory.
                The function that should be replaced will be replaced in the order of the given plugin list.
                So two plugin will override the same function, the plugin first in this list will override the function first.
                Also the hooks of the plugins will be called in the order their plugin where passed to this method.

        \param A list of plugin that should be linked (relocated) an loaded into memory
    **/
    void loadAndLinkPlugins(std::vector<PluginInformation *> pluginInformation);

    /**
        \brief  Iterates through the vector and delete all it's elements

        \param A list of PluginInformation* that should be deleted.
    **/
    void clearPluginInformation(std::vector<PluginInformation*> PluginInformation);

    size_t getTotalSpace() {
        return ((u32) this->endAddress - (u32) this->startAddress);
    }

    size_t getAvailableSpace() {
        return ((u32) this->currentStoreAddress - (u32) this->startAddress);
    }

    size_t getUsedSpace() {
        return getTotalSpace() - getAvailableSpace();
    }

    void resetPluginLoader() {
        this->currentStoreAddress = endAddress;
    }

private:
    PluginLoader(void * startAddress, void * endAddress) {
        // TODO: Check if endAddress > startAddress.
        this->startAddress = startAddress;
        this->endAddress = endAddress;
        this->currentStoreAddress = endAddress;
    }

    ~PluginLoader() {

    }

    static PluginLoader *instance;

    /**
        \brief  Iterates through the vector and delete all it's elements

        \param A list of PluginData* that should be deleted.
    **/
    void clearPluginData(std::vector<PluginData *> pluginData);

    /**
        \brief Load

        \param pluginInformation a PluginInformation object of the plugin that should be linked (relocated) and loaded.

        \return NULL on error. On success it will return a PluginData object.
    **/
    PluginData * loadAndLinkPlugin(PluginInformation * pluginInformation);

    /**
        \brief  Loads a plugin into memory (in the startAddress/endAddress range defined in this loader) and relocates it.
                Modifies the pluginData param. Adds loaded functions and hooks.
        \param pluginData object where the result should be stored
        \param elf source elf from where the sections will be loaded
        \param storeAddressEnd the address where the plugin data will be stored in memory. Saving BACKWARD.

    **/
    bool loadAndLinkElf(PluginData * pluginData, Elf *elf, void * storeAddressEnd);

    /**
       \brief  Copies the needed information into a global, persistent struct. This struct holds information on which
               function should be override in which order and which hook should be called.
       \param plugins list of plugins that should be used.

    **/
    void copyPluginDataIntoGlobalStruct(std::vector<PluginData *> plugins);

    void * getCurrentStoreAddress() {
        return this->currentStoreAddress;
    }

    void setCurrentStoreAddress(void *  addr) {
        this->currentStoreAddress = addr;
    }

    void * startAddress = NULL;
    void * endAddress = NULL;
    void * currentStoreAddress = NULL;
};


#endif
