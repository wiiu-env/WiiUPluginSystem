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

#ifndef _MODULE_INFORMATION_H_
#define _MODULE_INFORMATION_H_

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

class ModuleInformation{
    public:
        /**

        returns ModuleInformation* if a valid plugin was found at the given path. Otherwise returns NULL
        **/
        static ModuleInformation * loadModuleInformation(std::string path){
            if(ModuleInformation::checkFileExtenstion(path.c_str())){
                DEBUG_FUNCTION_LINE("Checkfile successfully, loading now Module Information\n");
                ModuleInformation * moduleInformation = new ModuleInformation(path);
                if(moduleInformation->openAndParseElf()){
                    return moduleInformation;
                }else{
                    delete moduleInformation;
                    return NULL;
                }
            } else {
                return NULL;
            }
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

        size_t getSize(){
            return this->size;
        }
    private:
        ModuleInformation(std::string path){
            this->path = path;
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

        static bool checkFileExtenstion(const char * path);

        bool openAndParseElf();

        bool parseElf(Elf *elf);

        bool metadataRead(Elf *elf, Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx);

        bool loadedSuccessfully = false;

        std::string path;
        std::string name;
        std::string author;
        std::string version;
        std::string license;
        size_t size;
};


#endif
