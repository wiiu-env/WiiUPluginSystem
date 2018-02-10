/* link_utils.cpp
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

#include "link_utils.h"
#include "elf_utils.h"
#include "utils.h"
#include <utils/utils.h>
#include <wups.h>
#include <libelf.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <utils/logger.h>
#include <common/retain_vars.h>
#include <unistd.h>
#include <dynamic_libs/os_functions.h>

//TODO: Adjust this.
#define MODULE_ENTRIES_CAPACITY_DEFAULT 128

//TODO: Adjust this.
#define MODULE_HOOKS_CAPACITY_DEFAULT 128

wups_loader_hook_t *module_hooks = NULL;
size_t module_hooks_count = 0;
size_t module_hooks_capacity = 0;

wups_loader_entry_t *module_entries = NULL;
size_t module_entries_count = 0;
size_t module_entries_capacity = 0;

bool Module_ListLink(uint8_t **space) {
    size_t i;
    bool result = false;

    for (i = 0; i < module_list_count; i++) {
        if (!Module_LinkModule(i, module_list[i]->path, space))
            goto exit_error;
    }

    result = true;
exit_error:
    if (!result) DEBUG_FUNCTION_LINE("Module_ListLink: exit_error\n");
    return result;
}

bool Module_LinkModule(size_t index, const char *path, uint8_t **space) {
    DEBUG_FUNCTION_LINE("Running Module_LinkModule for %s\n",path);

    int fd = -1;
    Elf *elf = NULL;
    bool result = false;

    /* check for compile errors */
    assert(elf_version(EV_CURRENT) != EV_NONE);

    fd = open(path, O_RDONLY, 0);

    if (fd == -1)
        goto exit_error;

    elf = elf_begin(fd, ELF_C_READ, NULL);

    if (elf == NULL)
        goto exit_error;

    switch (elf_kind(elf)) {
        case ELF_K_AR:
            /* TODO */
            goto exit_error;
        case ELF_K_ELF:
            DEBUG_FUNCTION_LINE("Calling now Module_LinkModuleElf\n");
            if (!Module_LinkModuleElf(index, elf, space))
                goto exit_error;
            break;
        default:
            goto exit_error;
    }

    result = true;
exit_error:
    if (!result) DEBUG_FUNCTION_LINE("Module_LinkModule: exit_error\n");
    if (elf != NULL)
        elf_end(elf);
    if (fd != -1)
        close(fd);
    return result;
}

bool Module_LinkModuleElf(size_t index, Elf *elf, uint8_t **space) {
    Elf_Scn *scn;
    size_t symtab_count, section_count, shstrndx, symtab_strndx, entries_count, hooks_count;
    Elf32_Sym *symtab = NULL;
    uint8_t **destinations = NULL;
    wups_loader_entry_t *entries = NULL;
    wups_loader_hook_t *hooks = NULL;
    bool result = false;

    if (!Module_LoadElfSymtab(elf, &symtab, &symtab_count, &symtab_strndx))
        goto exit_error;

    assert(symtab != NULL);

    if (elf_getshdrnum(elf, &section_count) != 0)
        goto exit_error;
    if (elf_getshdrstrndx(elf, &shstrndx) != 0)
        goto exit_error;

    destinations = (uint8_t **) malloc(sizeof(uint8_t *) * section_count);

    for (scn = elf_nextscn(elf, NULL);
         scn != NULL;
         scn = elf_nextscn(elf, scn)) {

        Elf32_Shdr *shdr;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL)
            continue;

        if ((shdr->sh_type == SHT_PROGBITS || shdr->sh_type == SHT_NOBITS) &&
            (shdr->sh_flags & SHF_ALLOC)) {

            const char *name;

            destinations[elf_ndxscn(scn)] = NULL;

            name = elf_strptr(elf, shstrndx, shdr->sh_name);
            if (name == NULL)
                continue;

            if (strcmp(name, ".wups.meta") == 0) {
                continue;
            } else if (strcmp(name, ".wups.load") == 0) {
                if (entries != NULL)
                    goto exit_error;

                entries_count = shdr->sh_size / sizeof(wups_loader_entry_t);
                entries = (wups_loader_entry_t*) Module_ListAllocate(
                    &module_entries, sizeof(wups_loader_entry_t),
                    entries_count, &module_entries_capacity,
                    &module_entries_count, MODULE_ENTRIES_CAPACITY_DEFAULT);

                if (entries == NULL)
                    goto exit_error;

                destinations[elf_ndxscn(scn)] = (uint8_t *)entries;
                if (!Module_ElfLoadSection(elf, scn, shdr, entries))
                    goto exit_error;
                Module_ElfLoadSymbols(elf_ndxscn(scn), entries, symtab, symtab_count);

                int entries_offset = module_entries_count - entries_count;

                for(int i = 0;i< entries_count;i++){
                    gbl_replacement_data.module_data[index].functions[i].entry_index = entries_offset +i;
                    gbl_replacement_data.module_data[index].number_used_functions++;
                }
            }else if (strcmp(name, ".wups.hooks") == 0) {
                if (hooks != NULL)
                    goto exit_error;

                hooks_count = shdr->sh_size / sizeof(wups_loader_hook_t);
                hooks = (wups_loader_hook_t*) Module_ListAllocate(
                    &module_hooks, sizeof(wups_loader_hook_t),
                    hooks_count, &module_hooks_capacity,
                    &module_hooks_count, MODULE_HOOKS_CAPACITY_DEFAULT);

                if (hooks == NULL)
                    goto exit_error;

                destinations[elf_ndxscn(scn)] = (uint8_t *)hooks;
                if (!Module_ElfLoadSection(elf, scn, shdr, hooks))
                    goto exit_error;
                Module_ElfLoadSymbols(elf_ndxscn(scn), hooks, symtab, symtab_count);

                int hook_offset = module_hooks_count - hooks_count;

                for(int i = 0;i< hooks_count;i++){
                    gbl_replacement_data.module_data[index].hooks[i].hook_index = hook_offset +i;
                    gbl_replacement_data.module_data[index].number_used_hooks++;
                }
            } else {
                *space -= shdr->sh_size;

                if (shdr->sh_addralign > 3)
                    *space = (uint8_t *)((int)*space &
                        ~(shdr->sh_addralign - 1));
                else
                    *space = (uint8_t *)((int)*space & ~3);

                destinations[elf_ndxscn(scn)] = *space;

                assert(*space != NULL);
                if((u32) *space < getApplicationEndAddr()){
                    DEBUG_FUNCTION_LINE("Not enough space to load function %s into memory at %08X.\n",name,*space);
                    goto exit_error;
                }

                DEBUG_FUNCTION_LINE("Copy section %s to %08X\n",name,*space);
                if (!Module_ElfLoadSection(elf, scn, shdr, *space))
                    goto exit_error;
                Module_ElfLoadSymbols(
                    elf_ndxscn(scn), *space, symtab, symtab_count);
            }
        }
    }

    if (entries == NULL)
        goto exit_error;

    for (scn = elf_nextscn(elf, NULL);
         scn != NULL;
         scn = elf_nextscn(elf, scn)) {

        Elf32_Shdr *shdr;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL)
            continue;

        if ((shdr->sh_type == SHT_PROGBITS || shdr->sh_type == SHT_NOBITS) &&
            (shdr->sh_flags & SHF_ALLOC) &&
            destinations[elf_ndxscn(scn)] != NULL) {

            if (!Module_ElfLink(
                    index, elf, elf_ndxscn(scn), destinations[elf_ndxscn(scn)],
                    symtab, symtab_count, symtab_strndx, true))
                goto exit_error;
        }
    }

    result = true;
exit_error:
    if (!result) DEBUG_FUNCTION_LINE("Module_LinkModuleElf: exit_error\n");
    if (destinations != NULL)
        free(destinations);
    if (symtab != NULL)
        free(symtab);
    return result;
}

/**
Relocation the real_XXX calls into the "replace_data" area of the replacement_data_function_t struct.
This function also fill the other data needed of the replacement_data_function_t struct.
**/
bool Module_ListLinkFinal() {
    int has_error = 0;
    bool result;

    int relocations = 0;
    DEBUG_FUNCTION_LINE("Found modules: %d\n",gbl_replacement_data.number_used_modules);
    for(int module_index=0;module_index<gbl_replacement_data.number_used_modules;module_index++){
        replacement_data_module_t * module_data = &gbl_replacement_data.module_data[module_index];
        DEBUG_FUNCTION_LINE("Module name: %s\n",module_data->module_name);
        for(int j=0;j<module_data->number_used_hooks;j++){
            replacement_data_hook_t * hook_data = &module_data->hooks[j];
            if(hook_data->hook_index > module_entries_count-1){
                DEBUG_FUNCTION_LINE("Error. hook_index was too high: %d. maximum is %d\n",hook_data->hook_index,module_entries_count-1);
                goto exit_error;
            }
            wups_loader_hook_t * hook  = &module_hooks[hook_data->hook_index];
            DEBUG_FUNCTION_LINE("Set hook %d of module \"%s\" of type %08X\n",hook_data->hook_index,module_data->module_name,hook->type);
            hook_data->func_pointer = (void*) hook->target;
            hook_data->type         = hook->type;

        }


        DEBUG_FUNCTION_LINE("number of used function: %d\n",module_data->number_used_functions);
        for(int j=0;j<module_data->number_used_functions;j++){
            replacement_data_function_t * function_data = &module_data->functions[j];
            if(function_data->entry_index > module_entries_count-1){
                DEBUG_FUNCTION_LINE("Error. entry_index was too high: %d. maximum is %d\n",function_data->entry_index,module_entries_count-1);
                goto exit_error;
            }
            wups_loader_entry_t * entry  = &module_entries[function_data->entry_index];
            strncpy(function_data->function_name,entry->data._function.name,MAXIMUM_FUNCTION_NAME_LENGTH-1);
            function_data->library = entry->data._function.library;
            function_data->replaceAddr = (u32) entry->data._function.target;
            function_data->replaceCall = (u32) entry->data._function.call_addr;

            /*
            We don't need this...

            DEBUG_FUNCTION_LINE("Searching for relocations %d\n",module_relocations_count);

            module_unresolved_relocation_t *reloc = NULL;
            for (int relocation_index = 0;relocation_index < module_relocations_count;relocation_index++) {
                DEBUG_FUNCTION_LINE("Try relocation %d\n",relocation_index);
                if (module_relocations[relocation_index].module == module_index){
                    if(strcmp(entry->data._function.real_function_name,module_relocations[relocation_index].name) == 0){
                        DEBUG_FUNCTION_LINE("Found the entry we want to replace %s\n",entry->data._function.real_function_name);
                        reloc = &module_relocations[relocation_index];
                        break;
                    }
                }
            }

            if(reloc != NULL){
                u32 call_addr = (u32) &function_data->replace_data[0];
                //DEBUG_FUNCTION_LINE("Found reloc. We need to find the symbol for: %s in lib %d\n",entry->data._function.name,entry->data._function.library);
                //u32 call_addr = (u32) new_GetAddressOfFunction("OSFatal",WUPS_LOADER_LIBRARY_COREINIT);
                DEBUG_FUNCTION_LINE("Relocating\n");
                if (!Module_ElfLinkOne(reloc->type, reloc->offset, reloc->addend, reloc->address, (uint32_t)call_addr)){
                    goto exit_error;
                }else{
                    relocations++;
                    DEBUG_FUNCTION_LINE("Success! relocated to %08X\n",call_addr);
                }
            }else{

            }*/
        }
    }
    module_relocations_count -= relocations;

    //assert(entry_index == module_entries_count);
    //assert(relocation_index == module_relocations_count);

    if (has_error)
        goto exit_error;

    result = true;
exit_error:
    if (!result) DEBUG_FUNCTION_LINE("Module_ListLinkFinal: exit_error\n");
    //module_relocations_count = 0;
    //module_relocations_capacity = 0;
    if(module_relocations_count == 0){
        free(module_relocations);
    }
    return result;
}

