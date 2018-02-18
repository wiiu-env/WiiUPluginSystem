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

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ElfTools.h"
#include "PluginData.h"
#include "PluginLoader.h"
#include "utils/StringTools.h"
#include "common/retain_vars.h"

PluginLoader * PluginLoader::instance = NULL;

std::vector<PluginInformation *> PluginLoader::getPluginInformation(const char * path){
    std::vector<PluginInformation *> result;
    struct dirent *dp;
    DIR *dfd = NULL;

    if(path == NULL){
        DEBUG_FUNCTION_LINE("Path was NULL");
        return result;
    }

    if ((dfd = opendir(path)) == NULL){
        DEBUG_FUNCTION_LINE("Couldn't open dir %s",path);
        return result;
    }

    while ((dp = readdir(dfd)) != NULL){
        struct stat stbuf ;
        std::string full_file_path = StringTools::strfmt("%s/%s",path,dp->d_name);
        StringTools::RemoveDoubleSlashs(full_file_path);
        if( stat(full_file_path.c_str(),&stbuf ) == -1 ){
            DEBUG_FUNCTION_LINE("Unable to stat file: %s\n",full_file_path.c_str()) ;
            continue;
        }

        if ( ( stbuf.st_mode & S_IFMT ) == S_IFDIR ){ // Skip directories
            continue;
        }else{
            DEBUG_FUNCTION_LINE("Found file: %s\n",full_file_path.c_str()) ;
            PluginInformation * plugin = PluginInformation::loadPluginInformation(full_file_path);
            if(plugin != NULL){
                DEBUG_FUNCTION_LINE("Found plugin %s by %s. Size: %d kb \n",plugin->getName().c_str(),plugin->getAuthor().c_str(),plugin->getSize()/1024) ;
                result.push_back(plugin);
            } else {
                DEBUG_FUNCTION_LINE("%s is not a valid plugin\n",full_file_path.c_str()) ;
            }
        }
    }

    return result;
}

std::vector<PluginInformation *> PluginLoader::getPluginsLoadedInMemory(){
    std::vector<PluginInformation *> pluginInformation;
    for(s32 i = 0; i < gbl_replacement_data.number_used_plugins; i++){
        replacement_data_plugin_t * pluginInfo = &gbl_replacement_data.plugin_data[i];
        PluginInformation * curPlugin = PluginInformation::loadPluginInformation(pluginInfo->path);
        if(curPlugin != NULL){
            pluginInformation.push_back(curPlugin);
        }
    }
    return pluginInformation;
}

void PluginLoader::loadAndLinkPlugins(std::vector<PluginInformation *> pluginInformation){
    std::vector<PluginData *> loadedPlugins;
    for(size_t i = 0;i < pluginInformation.size(); i++){
        DEBUG_FUNCTION_LINE("loadAndLinkPlugins for %d\n",i) ;
        PluginInformation * cur_info = pluginInformation[i];
        PluginData * pluginData = loadAndLinkPlugin(cur_info);
        if(pluginData == NULL){
            DEBUG_FUNCTION_LINE("loadAndLinkPlugins failed for %d\n",i) ;
            continue;
        } else {
            loadedPlugins.push_back(pluginData);
        }
    }

    copyPluginDataIntoGlobalStruct(loadedPlugins);
    clearPluginData(loadedPlugins);
}

void PluginLoader::clearPluginData(std::vector<PluginData *> pluginData){
    for(size_t i = 0;i < pluginData.size(); i++){
        PluginData * curPluginData = pluginData[i];
        if(curPluginData != NULL){
            delete curPluginData;
        }
    }
}

void PluginLoader::clearPluginInformation(std::vector<PluginInformation *> pluginInformation){
    for(size_t i = 0;i < pluginInformation.size(); i++){
        PluginInformation * curPluginInformation = pluginInformation[i];
        if(curPluginInformation != NULL){
            delete curPluginInformation;
        }
    }
}

PluginData * PluginLoader::loadAndLinkPlugin(PluginInformation * pluginInformation){
    DEBUG_FUNCTION_LINE("\n");
    PluginData * result = NULL;
    int fd = -1;
    Elf *elf = NULL;

    if(pluginInformation == NULL){
        DEBUG_FUNCTION_LINE("pluginInformation was NULL\n");
        goto exit_error;
    }

    if(pluginInformation->getSize() > ((u32) this->getCurrentStoreAddress() - (u32) this->startAddress)){
        DEBUG_FUNCTION_LINE("Not enough space left to loader the plugin into memory\n");
        goto exit_error;
    }

    /* check for compile errors */
    if (elf_version(EV_CURRENT) == EV_NONE){
        goto exit_error;
    }

    fd = open(pluginInformation->getPath().c_str(), O_RDONLY, 0);

    if (fd == -1){
        DEBUG_FUNCTION_LINE("failed to open '%s' \n", pluginInformation->getPath().c_str());
        goto exit_error;
    }

    elf = elf_begin(fd, ELF_C_READ, NULL);

    if (elf == NULL){
        DEBUG_FUNCTION_LINE("elf was NULL\n");
        goto exit_error;
    }

    DEBUG_FUNCTION_LINE("\n");
    result = new PluginData(pluginInformation);
    if(result == NULL){
        DEBUG_FUNCTION_LINE("Failed to create object\n");
        goto exit_error;
    }

    if(!this->loadAndLinkElf(result, elf, this->getCurrentStoreAddress())){
        delete result;
        result = NULL;
    }

exit_error:
    if (elf != NULL){
        elf_end(elf);
    }
    if (fd != -1){
        close(fd);
    }
    return result;
}

bool PluginLoader::loadAndLinkElf(PluginData * pluginData, Elf *elf, void * endAddress) {
    if(pluginData == NULL || elf == NULL || endAddress == NULL){
        return false;
    }

    DEBUG_FUNCTION_LINE("\n");

    u32 curAddress = (u32) endAddress;

    Elf_Scn *scn;
    size_t symtab_count, section_count, shstrndx, symtab_strndx, entries_count, hooks_count;
    Elf32_Sym *symtab = NULL;
    uint8_t **destinations = NULL;
    wups_loader_entry_t *entries = NULL;
    wups_loader_hook_t *hooks = NULL;
    bool result = false;

    std::vector<wups_loader_entry_t *> entry_t_list;
    std::vector<wups_loader_hook_t *> hook_t_list;

    std::vector<EntryData *> entry_data_list;
    std::vector<HookData *> hook_data_list;

    if (!ElfTools::loadElfSymtab(elf, &symtab, &symtab_count, &symtab_strndx)){
        goto exit_error;
    }

    if(symtab == NULL){
     goto exit_error;
    }

    if (elf_getshdrnum(elf, &section_count) != 0){
        goto exit_error;
    }
    if (elf_getshdrstrndx(elf, &shstrndx) != 0){
        goto exit_error;
    }

    destinations = (uint8_t **) malloc(sizeof(uint8_t *) * section_count);

    for (scn = elf_nextscn(elf, NULL); scn != NULL; scn = elf_nextscn(elf, scn)) {
        Elf32_Shdr *shdr;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL){
            continue;
        }

        if ((shdr->sh_type == SHT_PROGBITS || shdr->sh_type == SHT_NOBITS) &&
            (shdr->sh_flags & SHF_ALLOC)) {

            const char *name;

            destinations[elf_ndxscn(scn)] = NULL;

            name = elf_strptr(elf, shstrndx, shdr->sh_name);
            if (name == NULL){
                continue;
            }

            if (strcmp(name, ".wups.meta") == 0) {
                continue;
            } else if (strcmp(name, ".wups.load") == 0) {
                if (entries != NULL){
                    goto exit_error;
                }

                entries_count = shdr->sh_size / sizeof(wups_loader_entry_t);
                entries = (wups_loader_entry_t *) malloc(sizeof(wups_loader_entry_t) * entries_count);

                if (entries == NULL){
                    goto exit_error;
                }

                destinations[elf_ndxscn(scn)] = (uint8_t *)entries;
                if (!ElfTools::elfLoadSection(elf, scn, shdr, entries)){
                    goto exit_error;
                }

                ElfTools::elfLoadSymbols(elf_ndxscn(scn), entries, symtab, symtab_count);

                for(size_t i = 0;i< entries_count;i++){
                    entry_t_list.push_back(&entries[i]);
                }
            }else if (strcmp(name, ".wups.hooks") == 0) {
                if (hooks != NULL){
                    goto exit_error;
                }

                hooks_count = shdr->sh_size / sizeof(wups_loader_hook_t);
                hooks = (wups_loader_hook_t *) malloc(sizeof(wups_loader_hook_t) * hooks_count);

                if (hooks == NULL){
                    goto exit_error;
                }

                destinations[elf_ndxscn(scn)] = (uint8_t *)hooks;
                if (!ElfTools::elfLoadSection(elf, scn, shdr, hooks)){
                    goto exit_error;
                }
                ElfTools::elfLoadSymbols(elf_ndxscn(scn), hooks, symtab, symtab_count);

                for(size_t i = 0;i< hooks_count;i++){
                    hook_t_list.push_back(&hooks[i]);
                }

            } else {
                curAddress -= shdr->sh_size;

                if (shdr->sh_addralign > 3){
                        curAddress = (u32)((int)curAddress & ~(shdr->sh_addralign - 1));
                } else {
                    curAddress = (u32)((int)curAddress & ~3);
                }
                destinations[elf_ndxscn(scn)] = (uint8_t *) curAddress;

                if((u32) curAddress < (u32) this->startAddress){
                    DEBUG_FUNCTION_LINE("Not enough space to load function %s into memory at %08X.\n",name,curAddress);
                    goto exit_error;
                }

                DEBUG_FUNCTION_LINE("Copy section %s to %08X\n",name,curAddress);
                if (!ElfTools::elfLoadSection(elf, scn, shdr, (void*) curAddress)){
                    goto exit_error;
                }
                ElfTools::elfLoadSymbols(elf_ndxscn(scn), (void*) curAddress, symtab, symtab_count);
            }
        }
    }

    if (entries == NULL){
        goto exit_error;
    }

    for (scn = elf_nextscn(elf, NULL); scn != NULL; scn = elf_nextscn(elf, scn)) {
        Elf32_Shdr *shdr;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL){
            continue;
        }

        if ((shdr->sh_type == SHT_PROGBITS || shdr->sh_type == SHT_NOBITS) &&
            (shdr->sh_flags & SHF_ALLOC) &&
            destinations[elf_ndxscn(scn)] != NULL) {

            if (!ElfTools::elfLink(elf, elf_ndxscn(scn), destinations[elf_ndxscn(scn)], symtab, symtab_count, symtab_strndx, true)){
                goto exit_error;
            }
        }
    }

    for(size_t j=0;j<hook_t_list.size();j++){
        wups_loader_hook_t * hook = hook_t_list[j];

        DEBUG_FUNCTION_LINE("Saving hook of plugin \"%s\". Type: %08X, target: %08X\n",pluginData->getPluginInformation()->getName().c_str(),hook->type,(void*) hook->target);
        HookData * hook_data = new HookData((void *) hook->target,hook->type);
        pluginData->addHookData(hook_data);
    }

    for(size_t j=0;j<entry_t_list.size();j++){
        wups_loader_entry_t * entry = entry_t_list[j];
        DEBUG_FUNCTION_LINE("Saving entry \"%s\" of plugin \"%s\". Library: %08X, target: %08X, call_addr: %08X\n",entry->_function.name,pluginData->getPluginInformation()->getName().c_str(),entry->_function.library,entry->_function.target, (void *) entry->_function.call_addr);
        EntryData * entry_data = new EntryData(entry->_function.name,entry->_function.library, (void *) entry->_function.target, (void *) entry->_function.call_addr);
        pluginData->addEntryData(entry_data);
    }

    this->setCurrentStoreAddress((void *) curAddress);

    result = true;
exit_error:
    if (!result) DEBUG_FUNCTION_LINE("exit_error\n");
    if (destinations != NULL){
        free(destinations);
    }
    if (symtab != NULL){
        free(symtab);
    }
    if (hooks != NULL){
        free(hooks);
    }
    if (entries != NULL){
        free(entries);
    }
    return result;
}

void PluginLoader::copyPluginDataIntoGlobalStruct(std::vector<PluginData *> plugins){
    // Reset data
    memset((void*)&gbl_replacement_data,0,sizeof(gbl_replacement_data));
    int plugin_index = 0;
    // Copy data to global struct.
    for(size_t i = 0; i< plugins.size();i++){
        PluginData * cur_plugin = plugins.at(i);
        PluginInformation * cur_pluginInformation = cur_plugin->getPluginInformation();

        std::vector<EntryData *> entry_data_list = cur_plugin->getEntryDataList();
        std::vector<HookData *> hook_data_list = cur_plugin->getHookDataList();
        if(plugin_index >= MAXIMUM_PLUGINS ){
            DEBUG_FUNCTION_LINE("Maximum of %d plugins reached. %s won't be loaded!\n",MAXIMUM_PLUGINS,cur_pluginInformation->getName().c_str());
            continue;
        }
        if(entry_data_list.size() > MAXIMUM_FUNCTION_PER_PLUGIN){
            DEBUG_FUNCTION_LINE("Plugin %s would replace to many function (%d, maximum is %d). It won't be loaded.\n",cur_pluginInformation->getName().c_str(),entry_data_list.size(),MAXIMUM_FUNCTION_PER_PLUGIN);
            continue;
        }
        if(hook_data_list.size() > MAXIMUM_HOOKS_PER_PLUGIN){
            DEBUG_FUNCTION_LINE("Plugin %s would set too many hooks (%d, maximum is %d). It won't be loaded.\n",cur_pluginInformation->getName().c_str(),hook_data_list.size(),MAXIMUM_HOOKS_PER_PLUGIN);
            continue;
        }

        replacement_data_plugin_t * plugin_data = &gbl_replacement_data.plugin_data[plugin_index];

        strncpy(plugin_data->plugin_name,cur_pluginInformation->getName().c_str(),MAXIMUM_PLUGIN_NAME_LENGTH-1);
        strncpy(plugin_data->path,cur_pluginInformation->getPath().c_str(),MAXIMUM_PLUGIN_PATH_NAME_LENGTH-1);

        for(size_t j = 0; j < entry_data_list.size();j++){
            replacement_data_function_t * function_data = &plugin_data->functions[j];

            EntryData * cur_entry = entry_data_list[j];
            DEBUG_FUNCTION_LINE("Adding entry \"%s\" for plugin \"%s\"\n",cur_entry->getName().c_str(),plugin_data->plugin_name);

            //TODO: Warning/Error if string is too long.
            strncpy(function_data->function_name,cur_entry->getName().c_str(),MAXIMUM_FUNCTION_NAME_LENGTH-1);

            function_data->library = cur_entry->getLibrary();
            function_data->replaceAddr = (u32) cur_entry->getReplaceAddress();
            function_data->replaceCall = (u32) cur_entry->getReplaceCall();

            plugin_data->number_used_functions++;
        }

        DEBUG_FUNCTION_LINE("Entries for plugin \"%s\": %d\n",plugin_data->plugin_name,plugin_data->number_used_functions);

        for(size_t j = 0; j < hook_data_list.size();j++){
            replacement_data_hook_t * hook_data = &plugin_data->hooks[j];

            HookData * hook_entry = hook_data_list[j];

            DEBUG_FUNCTION_LINE("Set hook for plugin \"%s\" of type %08X to target %08X\n",plugin_data->plugin_name,hook_entry->getType(),(void*) hook_entry->getFunctionPointer());
            hook_data->func_pointer = (void*) hook_entry->getFunctionPointer();
            hook_data->type         = hook_entry->getType();
            plugin_data->number_used_hooks++;
        }

        DEBUG_FUNCTION_LINE("Hooks for plugin \"%s\": %d\n",plugin_data->plugin_name,plugin_data->number_used_hooks);

        plugin_index++;
        gbl_replacement_data.number_used_plugins++;
    }
}
