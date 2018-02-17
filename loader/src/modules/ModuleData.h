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

#ifndef _MODULE_DATA_H_
#define _MODULE_DATA_H_

#include <string>
#include <vector>
#include "EntryData.h"
#include "HookData.h"
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
        ModuleData(std::string path, uint8_t ** space){
            this->path = path;
            if(checkFile()){
                DEBUG_FUNCTION_LINE("Checkfile successfully, loading now \n");
                this->loadedSuccessfully = load(space);
            } else {
                this->loadedSuccessfully = false;
            }
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

        void setName(const char * name){
            this->name = name;
        }

        void setAuthor(const char * author){
            this->author = author;
        }

        void setVersion(const char * version){
            this->version = version;
        }

        void setLicense(const char * license){
            this->license = license;
        }

        void setSize(size_t size){
            this->size = size;
        }

        std::string getName(){
            return this->name;
        }

        std::string getAuthor(){
            return this->author;
        }

        std::string getVersion(){
            return this->version;
        }

        std::string getLicense(){
            return this->license;
        }

        std::string getPath(){
            return path;
        }

        bool isLoadedSuccessfully(){
            return loadedSuccessfully;
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

    private:

        bool checkFile();

        bool load(uint8_t ** space);

        bool loadElf(Elf *elf);

        bool metadataRead(Elf *elf, Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx);

        bool linkModuleElf(Elf *elf, uint8_t **space);

        bool loadedSuccessfully = false;

        std::string path;
        std::string name;
        std::string author;
        std::string version;
        std::string license;
        size_t size;

        std::vector<EntryData *> entry_data_list;
        std::vector<HookData *> hook_data_list;
};


#endif
