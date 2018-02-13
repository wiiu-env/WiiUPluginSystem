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

#include "ModuleData.h"
#include <utils/logger.h>
#include <dynamic_libs/os_types.h>
#include <libelf.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <wups.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <utils/utils.h>
#include "ElfTools.h"

bool ModuleData::checkFile() {
    const char *extension;

    const char * path_c = getPath().c_str();

    /* find the file extension */
    extension = strrchr(path_c, '.');
    if (extension == NULL){
        extension = strchr(path_c, '\0');
    }else{
        extension++;
    }

    assert(extension != NULL);

    if (strcmp(extension, "mod") == 0 ||
        strcmp(extension, "o") == 0 ||
        strcmp(extension, "a") == 0 ||
        strcmp(extension, "elf") == 0) {
        return true;
    }
    return false;
}

bool ModuleData::load(uint8_t ** space) {
    bool result = false;
    int fd = -1;
    Elf *elf = NULL;

    /* check for compile errors */
    if (elf_version(EV_CURRENT) == EV_NONE){
        DEBUG_FUNCTION_LINE("Compiler errors in '%s' \n", getPath().c_str());
        goto exit_error;
    }

    fd = open(getPath().c_str(), O_RDONLY, 0);

    if (fd == -1){
        DEBUG_FUNCTION_LINE("failed to open '%s' \n", getPath().c_str());
        goto exit_error;
    }

    elf = elf_begin(fd, ELF_C_READ, NULL);

    if (elf == NULL){
        DEBUG_FUNCTION_LINE("elf was NULL\n");
        goto exit_error;
    }

    switch (elf_kind(elf)) {
        case ELF_K_AR:
            /* TODO */
            DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Archives not yet supported.\n", getPath().c_str());
            goto exit_error;
        case ELF_K_ELF:
            if(!this->loadElf(elf)){
                goto exit_error;
            }
            result = linkModuleElf(elf,space);
            break;
        default:
            DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Invalid ELF file.\n", getPath().c_str());
            goto exit_error;
    }

exit_error:
    if (elf != NULL)
        elf_end(elf);
    if (fd != -1)
        close(fd);
    return result;
}

bool ModuleData::loadElf( Elf *elf) {
    bool res = false;
    Elf_Scn *scn;
    Elf32_Ehdr *ehdr;
    char *ident;
    size_t shstrndx, sz, symtab_count, symtab_strndx;
    Elf32_Sym *symtab = NULL;

    size_t cur_size = 0;

    const char * path_c = getPath().c_str();

    assert(elf != NULL);
    assert(elf_kind(elf) == ELF_K_ELF);

    ident = elf_getident(elf, &sz);

    if (ident == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Invalid ELF header.\n", path_c);
        goto exit_error;
    }
    if (sz < 7) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Invalid ELF header.\n", path_c);
        goto exit_error;
    }
    if (ident[4] != ELFCLASS32) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Not 32 bit ELF.\n", path_c);
        goto exit_error;
    }
    if (ident[5] != ELFDATA2MSB) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Not Big Endian.\n", path_c);
        goto exit_error;
    }
    if (ident[6] != EV_CURRENT) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Unknown ELF version.\n", path_c);
        goto exit_error;
    }

    ehdr = elf32_getehdr(elf);

    if (ehdr == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Invalid ELF header\n", path_c);
        goto exit_error;
    }
    if (ehdr->e_type != ET_REL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Not relocatable ELF.\n", path_c);
        goto exit_error;
    }
    if (ehdr->e_machine != EM_PPC) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Architecture not EM_PPC.\n", path_c);
        goto exit_error;
    }
    if (ehdr->e_version != EV_CURRENT) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Unknown ELF version.\n", path_c);
        goto exit_error;
    }

    if (!ElfTools::loadElfSymtab(elf, &symtab, &symtab_count, &symtab_strndx)) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't parse symtab.\n", path_c);
        goto exit_error;
    }

    assert(symtab != NULL);

    DEBUG_FUNCTION_LINE("Reading metadata from path %s.\n", path_c);

    if(!metadataRead(elf, symtab, symtab_count, symtab_strndx)){
        goto exit_error;
    }

    if (elf_getshdrstrndx(elf, &shstrndx) != 0) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't find shdrstndx.\n", path_c);
        goto exit_error;
    }

    for (scn = elf_nextscn(elf, NULL); scn != NULL; scn = elf_nextscn(elf, scn)) {
        Elf32_Shdr *shdr;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL){
            continue;
        }

        if ((shdr->sh_type == SHT_PROGBITS || shdr->sh_type == SHT_NOBITS) &&
            (shdr->sh_flags & SHF_ALLOC)) {

            const char *name;

            name = elf_strptr(elf, shstrndx, shdr->sh_name);
            if (name == NULL){
                continue;
            }

            if (strcmp(name, ".wups.meta") == 0) {
                continue;
            } else if (strcmp(name, ".wups.load") == 0) {
                cur_size +=
                    shdr->sh_size / sizeof(wups_loader_entry_t) * 6*4;
            } else if (strcmp(name, ".wups.hooks") == 0) {
                cur_size +=
                    shdr->sh_size / sizeof(wups_loader_hook_t) * 2*4;
            } else {
                cur_size += shdr->sh_size;
                /* add alignment padding to size */
                if (shdr->sh_addralign > 3){
                    /* roundup to multiple of sh_addralign  */
                    cur_size += (-cur_size & (shdr->sh_addralign - 1));
                }else{
                    /* roundup to multiple of 4 */
                    cur_size += (-cur_size & 3);
                }
            }
        }
    }

    /* roundup to multiple of 4 */
    cur_size += (-cur_size & 3);

    this->setSize(cur_size);

    res = true;
exit_error:
    if (symtab != NULL){
        free(symtab);
    }

    return res;
}

bool ModuleData::metadataRead(Elf *elf, Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx) {
    char *metadata = NULL, *metadata_cur, *metadata_end;
    const char *game, *name, *author, *version, *license, *wups;

    Elf_Scn *scn;
    size_t shstrndx;

    if (elf_getshdrstrndx(elf, &shstrndx) != 0) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't find shstrndx\n", path);
        goto exit_error;
    }

    for (scn = elf_nextscn(elf, NULL); scn != NULL; scn = elf_nextscn(elf, scn)) {
        Elf32_Shdr *shdr;
        const char *name;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL){
            continue;
        }

        name = elf_strptr(elf, shstrndx, shdr->sh_name);
        if (name == NULL){
            continue;
        }

        if (strcmp(name, ".wups.meta") == 0) {
            if (shdr->sh_size == 0){
                continue;
            }

            if (metadata != NULL){
                continue;
            }

            metadata = (char*) malloc(shdr->sh_size);

            if (metadata == NULL){
                continue;
            }

            if (!ElfTools::elfLoadSection(elf, scn, shdr, metadata)) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't load .wups.meta.\n", path);
                goto exit_error;
            }

            ElfTools::elfLoadSymbols(elf_ndxscn(scn), metadata, symtab, symtab_count);

            if (!elfLink(elf, elf_ndxscn(scn), metadata, symtab, symtab_count, symtab_strndx, false)) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - .wups.meta contains invalid relocations.\n", path);
                goto exit_error;
            }

            metadata_end = metadata + shdr->sh_size;
            metadata_end[-1] = '\0';
        }
    }

    if (metadata == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Not a WUPS module file.\n", path);
        goto exit_error;
    }

    game = NULL;
    name = NULL;
    author = NULL;
    version = NULL;
    license = NULL;
    wups = NULL;

    for (metadata_cur = metadata; metadata_cur < metadata_end; metadata_cur = strchr(metadata_cur, '\0') + 1) {

        char *eq;

        assert(metadata_cur >= metadata && metadata_cur < metadata_end);

        if (*metadata_cur == '\0'){
            continue;
        }

        eq = strchr(metadata_cur, '=');
        if (eq == NULL){
            continue;
        }

        if (strncmp(metadata_cur, "game", eq - metadata_cur) == 0) {
            if (game != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_MODULE_GAME declarations.\n", path);
                goto exit_error;
            }
            game = eq + 1;
        } else if (strncmp(metadata_cur, "name", eq - metadata_cur) == 0) {
            if (name != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_MODULE_NAME declarations.\n", path);
                goto exit_error;
            }
            name = eq + 1;
        } else if (strncmp(metadata_cur, "author", eq - metadata_cur) == 0) {
            if (author != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_MODULE_AUTHOR declarations.\n", path);
                goto exit_error;
            }
            author = eq + 1;
        } else if (strncmp(metadata_cur, "version", eq - metadata_cur) == 0) {
            if (version != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_MODULE_VERSION declarations.\n", path);
                goto exit_error;
            }
            version = eq + 1;
        } else if (strncmp(metadata_cur, "license", eq - metadata_cur) == 0) {
            if (license != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_MODULE_LICENSE declarations.\n", path);
                goto exit_error;
            }
            license = eq + 1;
        } else if (strncmp(metadata_cur, "wups", eq - metadata_cur) == 0) {
            if (wups != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_MODULE_NAME declarations.\n", path);
                goto exit_error;
            }
            wups = eq + 1;
        }
    }

    if (game == NULL){
        game = "";
    }
    // TODO:
    /*if (wups == NULL || strcmp(wups, "0.1") != 0) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Unrecognised WUPS version.\n", path);
        goto exit_error;
    }*/
    if (name == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Missing WUPS_MODULE_NAME declaration.\n",path);
        goto exit_error;
    }
    if (author == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Missing WUPS_MODULE_AUTHOR declaration.\n", path);
        goto exit_error;
    }
    if (version == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Missing WUPS_MODULE_VERSION declaration.\n", path);
        goto exit_error;
    }
    if (license == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Missing WUPS_MODULE_LICENSE declaration.\n", path);
        goto exit_error;
    }

    this->setName(name);
    this->setAuthor(author);
    this->setVersion(version);
    this->setLicense(license);

    return true;

exit_error:

    if (metadata != NULL){
        free(metadata);
    }
    return false;
}

bool ModuleData::elfLink(Elf *elf, size_t shndx, void *destination, Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx, bool allow_globals) {
    Elf_Scn *scn;

    for (scn = elf_nextscn(elf, NULL); scn != NULL; scn = elf_nextscn(elf, scn)) {
        Elf32_Shdr *shdr;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL)
            continue;

        switch (shdr->sh_type) {
            case SHT_REL: {
                const Elf32_Rel *rel;
                Elf_Data *data;
                size_t i;

                if (shdr->sh_info != shndx){
                    continue;
                }

                data = elf_getdata(scn, NULL);
                if (data == NULL){
                    continue;
                }

                rel = (const Elf32_Rel *) data->d_buf;

                for (i = 0; i < shdr->sh_size / sizeof(Elf32_Rel); i++) {
                    uint32_t symbol_addr;
                    size_t symbol;

                    symbol = ELF32_R_SYM(rel[i].r_info);

                    if (symbol > symtab_count)
                        return false;

                    switch (symtab[symbol].st_shndx) {
                        case SHN_ABS: {
                            symbol_addr = symtab[symbol].st_value;
                            break;
                        } case SHN_COMMON: {
                            return false;
                        } case SHN_UNDEF: {

                            if (allow_globals) {
                                DEBUG_FUNCTION_LINE("The elf still have unresolved relocations. This is not supported.");
                                /*
                                Not support and not needed.

                                module_unresolved_relocation_t *reloc;
                                char *name;

                                reloc = (module_unresolved_relocation_t *) Module_ListAllocate(
                                    &module_relocations,
                                    sizeof(module_unresolved_relocation_t), 1,
                                    &module_relocations_capacity,
                                    &module_relocations_count,
                                    MODULE_RELOCATIONS_CAPCITY_DEFAULT);
                                if (reloc == NULL)
                                    return false;

                                name = elf_strptr(
                                    elf, symtab_strndx, symtab[symbol].st_name);

                                if (name == NULL) {
                                    module_relocations_count--;
                                    return false;
                                }

                                reloc->name = strdup(name);
                                if (reloc->name == NULL) {
                                    module_relocations_count--;
                                    return false;
                                }

                                reloc->module = index;
                                reloc->address = destination;
                                reloc->offset = rel[i].r_offset;
                                reloc->type = ELF32_R_TYPE(rel[i].r_info);
                                reloc->addend = *(int *)((char *)destination + rel[i].r_offset);

                                continue;
                                */
                                return false;
                            } else {
                                return false;
                            }
                        } default: {
                            if (symtab[symbol].st_other != 1) {
                                return false;
                            }

                            symbol_addr = symtab[symbol].st_value;
                            break;
                        }
                    }

                    if (!ElfTools::elfLinkOne(ELF32_R_TYPE(rel[i].r_info), rel[i].r_offset, *(int *)((char *)destination + rel[i].r_offset), destination, symbol_addr)){
                        return false;
                    }
                }
                break;
            } case SHT_RELA: {
                const Elf32_Rela *rela;
                Elf_Data *data;
                size_t i;

                if (shdr->sh_info != shndx)
                    continue;

                data = elf_getdata(scn, NULL);
                if (data == NULL)
                    continue;

                rela = (const Elf32_Rela *) data->d_buf;

                for (i = 0; i < shdr->sh_size / sizeof(Elf32_Rela); i++) {
                    uint32_t symbol_addr;
                    size_t symbol;

                    symbol = ELF32_R_SYM(rela[i].r_info);

                    if (symbol > symtab_count)
                        return false;

                    switch (symtab[symbol].st_shndx) {
                        case SHN_ABS: {
                            symbol_addr = symtab[symbol].st_value;
                            break;
                        } case SHN_COMMON: {
                            return false;
                        } case SHN_UNDEF: {
                            if (allow_globals) {
                                DEBUG_FUNCTION_LINE("The elf still have unresolved relocations. This is not supported.");
                                /*
                                Not support and not needed.
                                module_unresolved_relocation_t *reloc;
                                char *name;

                                reloc = (module_unresolved_relocation_t *) Module_ListAllocate(
                                    &module_relocations,
                                    sizeof(module_unresolved_relocation_t), 1,
                                    &module_relocations_capacity,
                                    &module_relocations_count,
                                    MODULE_RELOCATIONS_CAPCITY_DEFAULT);
                                if (reloc == NULL)
                                    return false;

                                name = elf_strptr(
                                    elf, symtab_strndx, symtab[symbol].st_name);

                                if (name == NULL) {
                                    module_relocations_count--;
                                    return false;
                                }

                                reloc->name = strdup(name);
                                if (reloc->name == NULL) {
                                    module_relocations_count--;
                                    return false;
                                }

                                DEBUG_FUNCTION_LINE("Adding relocation!\n");

                                reloc->module = index;
                                reloc->address = destination;
                                reloc->offset = rela[i].r_offset;
                                reloc->type = ELF32_R_TYPE(rela[i].r_info);
                                reloc->addend = rela[i].r_addend;

                                continue;*/
                                return false;
                            } else
                                return false;
                        } default: {

                            if (symtab[symbol].st_other != 1){
                                return false;
                            }
                            symbol_addr = symtab[symbol].st_value;
                            break;
                        }
                    }

                    if (!ElfTools::elfLinkOne(ELF32_R_TYPE(rela[i].r_info), rela[i].r_offset,rela[i].r_addend, destination, symbol_addr)){
                        return false;
                    }
                }
                break;
            }
        }
    }

    return true;
}

bool ModuleData::linkModuleElf(Elf *elf, uint8_t **space) {
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

    assert(symtab != NULL);

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
                if (!ElfTools::elfLoadSection(elf, scn, shdr, *space)){
                    goto exit_error;
                }
                ElfTools::elfLoadSymbols(elf_ndxscn(scn), *space, symtab, symtab_count);
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

            if (!elfLink(elf, elf_ndxscn(scn), destinations[elf_ndxscn(scn)], symtab, symtab_count, symtab_strndx, true)){
                goto exit_error;
            }
        }
    }

    for(size_t j=0;j<hook_t_list.size();j++){
        wups_loader_hook_t * hook = hook_t_list[j];

        DEBUG_FUNCTION_LINE("Set hook of module \"%s\" of type %08X to target %08X\n",getName().c_str(),hook->type,(void*) hook->target);
        HookData * hook_data = new HookData((void *) hook->target,hook->type);
        addHookData(hook_data);
    }

    for(size_t j=0;j<entry_t_list.size();j++){
        wups_loader_entry_t * entry = entry_t_list[j];
        DEBUG_FUNCTION_LINE("Set hook %s of module \"%s\" of type %08X to target %08X\n",entry->_function.name,getName().c_str(),entry->_function.library,entry->_function.target);
        EntryData * entry_data = new EntryData(entry->_function.name,entry->_function.library, (void *) entry->_function.target, (void *) entry->_function.call_addr);
        addEntryData(entry_data);
    }

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
