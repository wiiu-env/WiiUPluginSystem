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
#include <wups.h>
#include <libelf.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <utils/logger.h>
#include <unistd.h>

bool Module_LinkModule(module_information_t * module_information, uint8_t **space) {
     if(module_information == NULL){
        DEBUG_FUNCTION_LINE("module_information is null\n");
    }
    module_metadata_t *  metadata = module_information->metadata;
    const char * path = NULL;
    if(metadata != NULL && metadata->path != NULL){
        path = metadata->path;
    }else{
        DEBUG_FUNCTION_LINE("metadata is null\n");
        return false;
    }

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
            if (!Module_LinkModuleElf(module_information, elf, space))
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

bool Module_LinkModuleElf(module_information_t * module_information, Elf *elf, uint8_t **space) {
    Elf_Scn *scn;
    size_t symtab_count, section_count, shstrndx, symtab_strndx, entries_count;
    Elf32_Sym *symtab = NULL;
    uint8_t **destinations = NULL;
    wups_loader_entry_t *entries = NULL;
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
                entries = (wups_loader_entry_t*) malloc(sizeof(wups_loader_entry_t)*entries_count);

                if (entries == NULL)
                    goto exit_error;

                destinations[elf_ndxscn(scn)] = (uint8_t *)entries;
                if (!Module_ElfLoadSection(elf, scn, shdr, entries))
                    goto exit_error;
                Module_ElfLoadSymbols(elf_ndxscn(scn), entries, symtab, symtab_count);
                for(int i = 0;i<entries_count;i++){
                    wups_loader_entry_t * curEntry = &entries[i];
                    module_information->entries.push_back(curEntry);
                }

            } else {
                DEBUG_FUNCTION_LINE("Copy the function to %08X\n",*space);
                *space -= shdr->sh_size;
                if (shdr->sh_addralign > 3)
                    *space = (uint8_t *)((int)*space &
                        ~(shdr->sh_addralign - 1));
                else
                    *space = (uint8_t *)((int)*space & ~3);

                destinations[elf_ndxscn(scn)] = *space;

                assert(*space != NULL);
                if (!Module_ElfLoadSection(elf, scn, shdr, *space))
                    goto exit_error;
                Module_ElfLoadSymbols(elf_ndxscn(scn), *space, symtab, symtab_count);
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

            if (!Module_ElfLink(module_information, elf, elf_ndxscn(scn), destinations[elf_ndxscn(scn)],
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
