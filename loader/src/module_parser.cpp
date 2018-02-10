/* module_parser.cpp
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

#include "module_parser.h"
#include "elf_utils.h"
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
#include "common/retain_vars.h"
#include "utils.h"

bool module_has_error;
bool module_has_info;

#define MODULE_LIST_CAPACITY_DEFAULT 16

size_t module_list_size = 0;
module_metadata_t **module_list = NULL;
size_t module_list_count = 0;
static size_t module_list_capacity = 0;

static void Module_LoadElf(const char *path, Elf *elf);
static module_metadata_t *Module_MetadataRead(const char *path, size_t index, Elf *elf, Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx);

bool Module_CheckFile(const char *path) {
    const char *extension;

    /* find the file extension */
    extension = strrchr(path, '.');
    if (extension == NULL){
        extension = strchr(path, '\0');
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

void Module_Load(const char *path) {
    int fd = -1;
    Elf *elf = NULL;

    /* check for compile errors */
    if (elf_version(EV_CURRENT) == EV_NONE)
        goto exit_error;

    fd = open(path, O_RDONLY, 0);

    if (fd == -1)
        goto exit_error;

    elf = elf_begin(fd, ELF_C_READ, NULL);

    if (elf == NULL)
        goto exit_error;

    switch (elf_kind(elf)) {
        case ELF_K_AR:
            /* TODO */
            DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Archives not yet supported.\n", path);
            module_has_info = true;
            goto exit_error;
        case ELF_K_ELF:
            Module_LoadElf(path, elf);
            break;
        default:
            DEBUG_FUNCTION_LINE(
                "Warning: Ignoring '%s' - Invalid ELF file.\n", path);
            goto exit_error;
    }

exit_error:
    if (elf != NULL)
        elf_end(elf);
    if (fd != -1)
        close(fd);
}

static void Module_LoadElf(const char *path, Elf *elf) {
    Elf_Scn *scn;
    Elf32_Ehdr *ehdr;
    char *ident;
    size_t shstrndx, sz, symtab_count, i, symtab_strndx;
    Elf32_Sym *symtab = NULL;
    module_metadata_t *metadata = NULL;
    module_metadata_t **list_ptr;

    assert(elf != NULL);
    assert(elf_kind(elf) == ELF_K_ELF);

    ident = elf_getident(elf, &sz);

    if (ident == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Invalid ELF header.\n", path);
        module_has_info = true;
        goto exit_error;
    }
    if (sz < 7) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Invalid ELF header.\n", path);
        module_has_info = true;
        goto exit_error;
    }
    if (ident[4] != ELFCLASS32) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Not 32 bit ELF.\n", path);
        module_has_info = true;
        goto exit_error;
    }
    if (ident[5] != ELFDATA2MSB) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Not Big Endian.\n", path);
        module_has_info = true;
        goto exit_error;
    }
    if (ident[6] != EV_CURRENT) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Unknown ELF version.\n", path);
        module_has_info = true;
        goto exit_error;
    }

    ehdr = elf32_getehdr(elf);

    if (ehdr == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Invalid ELF header\n", path);
        module_has_info = true;
        goto exit_error;
    }
    if (ehdr->e_type != ET_REL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Not relocatable ELF.\n", path);
        module_has_info = true;
        goto exit_error;
    }
    if (ehdr->e_machine != EM_PPC) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Architecture not EM_PPC.\n", path);
        module_has_info = true;
        goto exit_error;
    }
    if (ehdr->e_version != EV_CURRENT) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Unknown ELF version.\n", path);
        module_has_info = true;
        goto exit_error;
    }

    if (!Module_LoadElfSymtab(elf, &symtab, &symtab_count, &symtab_strndx)) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't parse symtab.\n", path);
        module_has_info = true;
        goto exit_error;
    }

    assert(symtab != NULL);

    DEBUG_FUNCTION_LINE("Reading metadata from path %s.\n", path);

    metadata = Module_MetadataRead(path, module_list_count, elf, symtab, symtab_count, symtab_strndx);

    if (metadata == NULL) /* error reporting done inside method */
        goto exit_error;

    if (elf_getshdrstrndx(elf, &shstrndx) != 0) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't find shdrstndx.\n", path);
        module_has_info = true;
        goto exit_error;
    }

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

            name = elf_strptr(elf, shstrndx, shdr->sh_name);
            if (name == NULL)
                continue;

            if (strcmp(name, ".wups.meta") == 0) {
                continue;
            } else if (strcmp(name, ".wups.load") == 0) {
                metadata->size +=
                    shdr->sh_size / sizeof(wups_loader_entry_t) * 6*4;
            } else if (strcmp(name, ".wups.hooks") == 0) {
                metadata->size +=
                    shdr->sh_size / sizeof(wups_loader_hook_t) * 2*4;
            } else {
                metadata->size += shdr->sh_size;
                /* add alignment padding to size */
                if (shdr->sh_addralign > 3)
                    /* roundup to multiple of sh_addralign  */
                    metadata->size +=
                        (-metadata->size & (shdr->sh_addralign - 1));
                else
                    /* roundup to multiple of 4 */
                    metadata->size += (-metadata->size & 3);
            }
        }
    }

    /* roundup to multiple of 4 */
    metadata->size += (-metadata->size & 3);

    strncpy(gbl_replacement_data.module_data[module_list_count].module_name,metadata->name,MAXIMUM_MODULE_NAME_LENGTH-1);

    list_ptr = (module_metadata_t **)Module_ListAllocate(
        &module_list, sizeof(module_metadata_t *), 1, &module_list_capacity,
        &module_list_count, MODULE_LIST_CAPACITY_DEFAULT);
    if (list_ptr == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - ENOMEM.\n", path);
        module_has_info = true;
        goto exit_error;
    }

    assert(module_list != NULL);
    assert(module_list_count <= module_list_capacity);

    *list_ptr = metadata;
    module_list_size += metadata->size;
    /* prevent the data being freed */
    metadata = NULL;

    gbl_replacement_data.number_used_modules++;


exit_error:
    if (metadata != NULL)
        free(metadata);
    if (symtab != NULL)
        free(symtab);
}

static module_metadata_t *Module_MetadataRead(const char *path, size_t index, Elf *elf, Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx) {
    char *metadata = NULL, *metadata_cur, *metadata_end, *tmp;
    const char *game, *name, *author, *version, *license, *wups;
    module_metadata_t *ret = NULL;
    Elf_Scn *scn;
    size_t shstrndx, entries_count;

    if (elf_getshdrstrndx(elf, &shstrndx) != 0) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't find shstrndx\n", path);
        module_has_info = true;
        goto exit_error;
    }

    entries_count = 0;

    for (scn = elf_nextscn(elf, NULL);
         scn != NULL;
         scn = elf_nextscn(elf, scn)) {

        Elf32_Shdr *shdr;
        const char *name;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL)
            continue;

        name = elf_strptr(elf, shstrndx, shdr->sh_name);
        if (name == NULL)
            continue;

        if (strcmp(name, ".wups.meta") == 0) {
            if (shdr->sh_size == 0)
                continue;

            if (metadata != NULL)
                continue;
            metadata = (char*) malloc(shdr->sh_size);
            if (metadata == NULL)
                continue;

            if (!Module_ElfLoadSection(elf, scn, shdr, metadata)) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't load .wups.meta.\n", path);
                module_has_info = true;
                goto exit_error;
            }

            Module_ElfLoadSymbols(
                elf_ndxscn(scn), metadata, symtab, symtab_count);

            if (!Module_ElfLink(
                    index, elf, elf_ndxscn(scn), metadata,
                    symtab, symtab_count, symtab_strndx, false)) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - .wups.meta contains invalid relocations.\n", path);
                module_has_info = true;
                goto exit_error;
            }

            metadata_end = metadata + shdr->sh_size;
            metadata_end[-1] = '\0';
        } else if (strcmp(name, ".wups.load") == 0) {
            entries_count = shdr->sh_size / sizeof(wups_loader_entry_t);
        }
    }

    if (metadata == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Not a WUPS module file.\n", path);
        module_has_info = true;
        goto exit_error;
    }

    game = NULL;
    name = NULL;
    author = NULL;
    version = NULL;
    license = NULL;
    wups = NULL;

    for (metadata_cur = metadata;
         metadata_cur < metadata_end;
         metadata_cur = strchr(metadata_cur, '\0') + 1) {

        char *eq;

        assert(metadata_cur >= metadata && metadata_cur < metadata_end);

        if (*metadata_cur == '\0')
            continue;

        eq = strchr(metadata_cur, '=');
        if (eq == NULL)
            continue;

        if (strncmp(metadata_cur, "game", eq - metadata_cur) == 0) {
            if (game != NULL) {
                DEBUG_FUNCTION_LINE(
                    "Warning: Ignoring '%s' - Multiple WUPS_MODULE_GAME "
                    "declarations.\n", path);
                module_has_info = true;
                goto exit_error;
            }
            game = eq + 1;
        } else if (strncmp(metadata_cur, "name", eq - metadata_cur) == 0) {
            if (name != NULL) {
                DEBUG_FUNCTION_LINE(
                    "Warning: Ignoring '%s' - Multiple WUPS_MODULE_NAME "
                    "declarations.\n", path);
                module_has_info = true;
                goto exit_error;
            }
            name = eq + 1;
        } else if (strncmp(metadata_cur, "author", eq - metadata_cur) == 0) {
            if (author != NULL) {
                DEBUG_FUNCTION_LINE(
                    "Warning: Ignoring '%s' - Multiple WUPS_MODULE_AUTHOR "
                    "declarations.\n", path);
                module_has_info = true;
                goto exit_error;
            }
            author = eq + 1;
        } else if (strncmp(metadata_cur, "version", eq - metadata_cur) == 0) {
            if (version != NULL) {
                DEBUG_FUNCTION_LINE(
                    "Warning: Ignoring '%s' - Multiple WUPS_MODULE_VERSION "
                    "declarations.\n", path);
                module_has_info = true;
                goto exit_error;
            }
            version = eq + 1;
        } else if (strncmp(metadata_cur, "license", eq - metadata_cur) == 0) {
            if (license != NULL) {
                DEBUG_FUNCTION_LINE(
                    "Warning: Ignoring '%s' - Multiple WUPS_MODULE_LICENSE "
                    "declarations.\n", path);
                module_has_info = true;
                goto exit_error;
            }
            license = eq + 1;
        } else if (strncmp(metadata_cur, "wups", eq - metadata_cur) == 0) {
            if (wups != NULL) {
                DEBUG_FUNCTION_LINE(
                    "Warning: Ignoring '%s' - Multiple WUPS_MODULE_NAME "
                    "declarations.\n", path);
                module_has_info = true;
                goto exit_error;
            }
            wups = eq + 1;
        }
    }

    if (game == NULL)
        game = "";
    if (wups == NULL || strcmp(wups, "0.1") != 0) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Unrecognised BSlug version.\n", path);
        module_has_info = true;
        goto exit_error;
    }
    if (name == NULL) {
        DEBUG_FUNCTION_LINE(
            "Warning: Ignoring '%s' - Missing WUPS_MODULE_NAME declaration.\n",
            path);
        module_has_info = true;
        goto exit_error;
    }
    if (author == NULL) {
        DEBUG_FUNCTION_LINE(
            "Warning: Ignoring '%s' - Missing WUPS_MODULE_AUTHOR "
            "declaration.\n", path);
        module_has_info = true;
        goto exit_error;
    }
    if (version == NULL) {
        DEBUG_FUNCTION_LINE(
            "Warning: Ignoring '%s' - Missing WUPS_MODULE_VERSION "
            "declaration.\n", path);
        module_has_info = true;
        goto exit_error;
    }
    if (license == NULL) {
        DEBUG_FUNCTION_LINE(
            "Warning: Ignoring '%s' - Missing WUPS_MODULE_LICENSE "
            "declaration.\n", path);
        module_has_info = true;
        goto exit_error;
    }

    ret = (module_metadata_t *)malloc(
        sizeof(module_metadata_t) + strlen(path) +
        strlen(game) + strlen(name) + strlen(author) +
        strlen(version) + strlen(license) + 6);
    if (ret == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't parse WUPS metadata.\n", path);
        module_has_info = true;
        goto exit_error;
    }

    tmp = (char *)(ret + 1);
    strcpy(tmp, path);
    ret->path = tmp;
    tmp = tmp + strlen(path) + 1;
    strcpy(tmp, name);
    ret->name = tmp;
    tmp = tmp + strlen(name) + 1;
    strcpy(tmp, author);
    ret->author = tmp;
    tmp = tmp + strlen(author) + 1;
    strcpy(tmp, version);
    ret->version = tmp;
    tmp = tmp + strlen(version) + 1;
    strcpy(tmp, license);
    ret->license = tmp;
    ret->size = 0;
    ret->entries_count = entries_count;

exit_error:
    if (metadata != NULL)
        free(metadata);

    return ret;
}
