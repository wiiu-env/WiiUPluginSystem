/* based on plugin.c
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

#include "PluginInformation.h"
#include <utils/logger.h>
#include <dynamic_libs/os_types.h>
#include <libelf.h>
#include <string.h>
#include <sys/types.h>
#include <wups.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <utils/utils.h>
#include "ElfTools.h"

bool PluginInformation::checkFileExtenstion(const char * path) {
    if(path == NULL) {
        return false;
    }
    const char *extension;

    /* find the file extension */
    extension = strrchr(path, '.');
    if (extension == NULL) {
        extension = strchr(path, '\0');
    } else {
        extension++;
    }

    if(extension == NULL) {
        return false;
    }

    if (strcmp(extension, "mod") == 0 ||
            strcmp(extension, "o") == 0 ||
            strcmp(extension, "a") == 0 ||
            strcmp(extension, "elf") == 0) {
        return true;
    }
    return false;
}

bool PluginInformation::openAndParseElf() {
    bool result = false;
    int32_t fd = -1;
    Elf *elf = NULL;

    /* check for compile errors */
    if (elf_version(EV_CURRENT) == EV_NONE) {
        goto exit_error;
    }

    fd = open(getPath().c_str(), O_RDONLY, 0);

    if (fd == -1) {
        DEBUG_FUNCTION_LINE("failed to open '%s' \n", getPath().c_str());
        goto exit_error;
    }

    elf = elf_begin(fd, ELF_C_READ, NULL);

    if (elf == NULL) {
        DEBUG_FUNCTION_LINE("elf was NULL\n");
        goto exit_error;
    }

    switch (elf_kind(elf)) {
    case ELF_K_AR:
        /* TODO */
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Archives not yet supported.\n", getPath().c_str());
        goto exit_error;
    case ELF_K_ELF:
        result = this->parseElf(elf);
        break;
    default:
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Invalid ELF file.\n", getPath().c_str());
        goto exit_error;
    }

exit_error:
    if (elf != NULL) {
        elf_end(elf);
    }
    if (fd != -1) {
        close(fd);
    }
    return result;
}

bool PluginInformation::parseElf( Elf *elf) {
    bool res = false;
    Elf_Scn *scn;
    Elf32_Ehdr *ehdr;
    char *ident;
    size_t shstrndx, sz, symtab_count, symtab_strndx;
    Elf32_Sym *symtab = NULL;

    size_t cur_size = 0;

    const char * path_c = getPath().c_str();

    if(elf == NULL) {
        goto exit_error;
    }
    if(elf_kind(elf) != ELF_K_ELF) {
        goto exit_error;
    }

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

    if(symtab == NULL) {
        goto exit_error;
    }

    if(!metadataRead(elf, symtab, symtab_count, symtab_strndx)) {
        goto exit_error;
    }

    if (elf_getshdrstrndx(elf, &shstrndx) != 0) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't find shdrstndx.\n", path_c);
        goto exit_error;
    }

    for (scn = elf_nextscn(elf, NULL); scn != NULL; scn = elf_nextscn(elf, scn)) {
        Elf32_Shdr *shdr;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL) {
            continue;
        }

        if ((shdr->sh_type == SHT_PROGBITS || shdr->sh_type == SHT_NOBITS) &&
                (shdr->sh_flags & SHF_ALLOC)) {

            const char *name;

            name = elf_strptr(elf, shstrndx, shdr->sh_name);
            if (name == NULL) {
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
                if (shdr->sh_addralign > 3) {
                    /* roundup to multiple of sh_addralign  */
                    cur_size += (-cur_size & (shdr->sh_addralign - 1));
                } else {
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
    if (symtab != NULL) {
        free(symtab);
    }

    return res;
}

bool PluginInformation::metadataRead(Elf *elf, Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx) {
    char *metadata = NULL, *metadata_cur, *metadata_end;
    const char *name, *author, *version, *license, *wups, *buildtimestamp, *description;

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
        if (shdr == NULL) {
            continue;
        }

        name = elf_strptr(elf, shstrndx, shdr->sh_name);
        if (name == NULL) {
            continue;
        }

        if (strcmp(name, ".wups.meta") == 0) {
            if (shdr->sh_size == 0) {
                continue;
            }

            if (metadata != NULL) {
                continue;
            }

            metadata = (char*) malloc(shdr->sh_size);

            if (metadata == NULL) {
                continue;
            }

            if (!ElfTools::elfLoadSection(elf, scn, shdr, metadata)) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't load .wups.meta.\n", path);
                goto exit_error;
            }

            ElfTools::elfLoadSymbols(elf_ndxscn(scn), metadata, symtab, symtab_count);

            if (!ElfTools::elfLink(elf, elf_ndxscn(scn), metadata, symtab, symtab_count, symtab_strndx, false)) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - .wups.meta contains invalid relocations.\n", path);
                goto exit_error;
            }

            metadata_end = metadata + shdr->sh_size;
            metadata_end[-1] = '\0';
        }
    }

    if (metadata == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Not a WUPS plugin file.\n", path);
        goto exit_error;
    }

    name = NULL;
    author = NULL;
    version = NULL;
    license = NULL;
    wups = NULL;
    buildtimestamp = NULL;
    description = NULL;

    for (metadata_cur = metadata; metadata_cur < metadata_end; metadata_cur = strchr(metadata_cur, '\0') + 1) {

        char *eq;

        if(metadata_cur < metadata || metadata_cur >= metadata_end) {
            goto exit_error;
        }

        if (*metadata_cur == '\0') {
            continue;
        }

        eq = strchr(metadata_cur, '=');
        if (eq == NULL) {
            continue;
        }

        if (strncmp(metadata_cur, "name", eq - metadata_cur) == 0) {
            if (name != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_PLUGIN_NAME declarations.\n", path);
                goto exit_error;
            }
            name = eq + 1;
        } else if (strncmp(metadata_cur, "author", eq - metadata_cur) == 0) {
            if (author != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_PLUGIN_AUTHOR declarations.\n", path);
                goto exit_error;
            }
            author = eq + 1;
        } else if (strncmp(metadata_cur, "version", eq - metadata_cur) == 0) {
            if (version != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_PLUGIN_VERSION declarations.\n", path);
                goto exit_error;
            }
            version = eq + 1;
        } else if (strncmp(metadata_cur, "license", eq - metadata_cur) == 0) {
            if (license != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_PLUGIN_LICENSE declarations.\n", path);
                goto exit_error;
            }
            license = eq + 1;
        } else if (strncmp(metadata_cur, "buildtimestamp", eq - metadata_cur) == 0) {
            if (buildtimestamp != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_PLUGIN_TIMESTAMP declarations.\n", path);
                goto exit_error;
            }
            buildtimestamp = eq + 1;
        } else if (strncmp(metadata_cur, "description", eq - metadata_cur) == 0) {
            if (description != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_PLUGIN_LICENSE declarations.\n", path);
                goto exit_error;
            }
            description = eq + 1;
        } else if (strncmp(metadata_cur, "wups", eq - metadata_cur) == 0) {
            if (wups != NULL) {
                DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Multiple WUPS_PLUGIN_NAME declarations.\n", path);
                goto exit_error;
            }
            wups = eq + 1;
        }
    }

    if (description == NULL) {
        description = "";
    }

    if (wups == NULL || strcmp(wups, "0.1") != 0) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Unrecognised WUPS version.\n", path);
        goto exit_error;
    }
    if (buildtimestamp == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Couldn't find buildtimestamp.\n", path);
        goto exit_error;
    }
    if (name == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Missing WUPS_PLUGIN_NAME declaration.\n",path);
        goto exit_error;
    }
    if (author == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Missing WUPS_PLUGIN_AUTHOR declaration.\n", path);
        goto exit_error;
    }
    if (version == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Missing WUPS_PLUGIN_VERSION declaration.\n", path);
        goto exit_error;
    }
    if (license == NULL) {
        DEBUG_FUNCTION_LINE("Warning: Ignoring '%s' - Missing WUPS_PLUGIN_LICENSE declaration.\n", path);
        goto exit_error;
    }

    this->setName(name);
    this->setAuthor(author);
    this->setVersion(version);
    this->setLicense(license);
    this->setBuildTimestamp(buildtimestamp);
    this->setDescription(description);

    return true;

exit_error:

    if (metadata != NULL) {
        free(metadata);
    }
    return false;
}
