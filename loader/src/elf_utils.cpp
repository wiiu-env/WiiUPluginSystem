/* elf_utils.cpp
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

int Module_ElfLoadSection(const Elf *elf, Elf_Scn *scn, const Elf32_Shdr *shdr, void *destination) {
    DEBUG_FUNCTION_LINE("\n");
    assert(destination != NULL);

    switch (shdr->sh_type) {
        case SHT_SYMTAB:
        case SHT_PROGBITS: {
            Elf_Data *data;
            size_t n;

            n = 0;
            for (data = elf_getdata(scn, NULL);
                 data != NULL;
                 data = elf_getdata(scn, data)) {
                memcpy((char *)destination + n, data->d_buf, data->d_size);
                n += data->d_size;
            }
            return 1;
        } case SHT_NOBITS: {
            memset(destination, 0, shdr->sh_size);
            return 1;
        } default:
            return 0;
    }
}

int Module_LoadElfSymtab(Elf *elf, Elf32_Sym **symtab, size_t *symtab_count,size_t *symtab_strndx) {
    Elf_Scn *scn;
    int result = 0;

    for (scn = elf_nextscn(elf, NULL);
         scn != NULL;
         scn = elf_nextscn(elf, scn)) {

        Elf32_Shdr *shdr;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL)
            continue;

        if (shdr->sh_type == SHT_SYMTAB) {
            size_t shstrndx;
            if (elf_getshdrstrndx(elf, &shstrndx) == 0) {
                char* name = elf_strptr(elf, shstrndx, shdr->sh_name);
                if (name != NULL){
                    DEBUG_FUNCTION_LINE("SHT_SYMTAB name: %s\n",name);
                }else{
                    DEBUG_FUNCTION_LINE("Name null\n");
                }
            }else{
                DEBUG_FUNCTION_LINE("Couldn't find shstrndx\n");
            }


            size_t sym;

            assert (*symtab == NULL);
            *symtab = (Elf32_Sym *) malloc(shdr->sh_size);
            if (*symtab == NULL)
                continue;

            *symtab_count = shdr->sh_size / sizeof(Elf32_Sym);
            *symtab_strndx = shdr->sh_link;

            if (!Module_ElfLoadSection(elf, scn, shdr, *symtab))
                goto exit_error;

            for (sym = 0; sym < *symtab_count; sym++)
                (*symtab)[sym].st_other = 0;

            break;
        }
    }

    if (*symtab == NULL)
        goto exit_error;

    result = 1;
exit_error:
    return result;
}

void Module_ElfLoadSymbols(size_t shndx, const void *destination, Elf32_Sym *symtab, size_t symtab_count) {

    size_t i;

    /* use the st_other field (no defined meaning) to indicate whether or not a
     * symbol address has been calculated. */
    for (i = 0; i < symtab_count; i++) {
        if (symtab[i].st_shndx == shndx &&
            symtab[i].st_other == 0) {

            symtab[i].st_value += (Elf32_Addr)destination;
            symtab[i].st_other = 1;
        }
    }
}

bool Module_ElfLink(module_information_t * module_information, Elf *elf, size_t shndx, void *destination,Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx,int allow_globals) {
    DEBUG_FUNCTION_LINE("In Module_ElfLink\n");
    Elf_Scn *scn;

    for (scn = elf_nextscn(elf, NULL);
         scn != NULL;
         scn = elf_nextscn(elf, scn)) {

        Elf32_Shdr *shdr;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL)
            continue;
        DEBUG_FUNCTION_LINE("shdr->sh_type: %d\n",shdr->sh_type);
        switch (shdr->sh_type) {
            case SHT_REL: {
                const Elf32_Rel *rel;
                Elf_Data *data;
                size_t i;

                if (shdr->sh_info != shndx)
                    continue;

                data = elf_getdata(scn, NULL);
                if (data == NULL)
                    continue;

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
                            DEBUG_FUNCTION_LINE("SHN_UNDEF\n");
                            if (allow_globals) {
                                module_unresolved_relocation_t *reloc;
                                char *name;

                                reloc = (module_unresolved_relocation_t*) malloc(sizeof(module_unresolved_relocation_t));
                                if (reloc == NULL){
                                    DEBUG_FUNCTION_LINE("WARNING: failed to allocate space for relocate struct.\n");
                                    return false;
                                }

                                name = elf_strptr(elf, symtab_strndx, symtab[symbol].st_name);

                                if (name == NULL) {
                                    DEBUG_FUNCTION_LINE("WARNING: name == NULL.\n");
                                    return false;
                                }

                                reloc->name = strdup(name);
                                if (reloc->name == NULL) {
                                    DEBUG_FUNCTION_LINE("WARNING: strdup failed.\n");
                                    return false;
                                }

                                reloc->address = destination;
                                reloc->offset = rel[i].r_offset;
                                reloc->type = ELF32_R_TYPE(rel[i].r_info);
                                reloc->addend =
                                    *(int *)((char *)destination +
                                        rel[i].r_offset);


                                DEBUG_FUNCTION_LINE("Relocate push_back!.\n");

                                module_information->rel.push_back(reloc);

                                continue;
                            } else
                                return false;
                        } default: {
                            if (symtab[symbol].st_other != 1)
                                return false;

                            symbol_addr = symtab[symbol].st_value;
                            break;
                        }
                    }

                    if (!Module_ElfLinkOne(
                            ELF32_R_TYPE(rel[i].r_info), rel[i].r_offset,
                            *(int *)((char *)destination + rel[i].r_offset),
                            destination, symbol_addr))
                        return false;
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

                rela = (Elf32_Rela*) data->d_buf;

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
                            DEBUG_FUNCTION_LINE("SHN_UNDEF2\n");
                            if (allow_globals) {
                                module_unresolved_relocation_t *reloc;
                                char *name;

                                reloc = (module_unresolved_relocation_t*) malloc(sizeof(module_unresolved_relocation_t));
                                if (reloc == NULL){
                                    DEBUG_FUNCTION_LINE("WARNING: failed to allocate space for relocate struct.\n");
                                    return false;
                                }


                                name = elf_strptr(elf, symtab_strndx, symtab[symbol].st_name);

                                if (name == NULL) {
                                    DEBUG_FUNCTION_LINE("WARNING: name == NULL.\n");
                                    return false;
                                }

                                reloc->name = strdup(name);
                                if (reloc->name == NULL) {
                                    DEBUG_FUNCTION_LINE("WARNING: strdup failed.\n");
                                    return false;
                                }

                                reloc->address = destination;
                                reloc->offset = rela[i].r_offset;
                                reloc->type = ELF32_R_TYPE(rela[i].r_info);
                                reloc->addend = rela[i].r_addend;

                                DEBUG_FUNCTION_LINE("Relocate push_back!.\n");

                                module_information->rel.push_back(reloc);

                                continue;
                            } else
                                return false;
                        } default: {
                            if (symtab[symbol].st_other != 1)
                                return false;

                            symbol_addr = symtab[symbol].st_value;
                            break;
                        }
                    }

                    if (!Module_ElfLinkOne(
                            ELF32_R_TYPE(rela[i].r_info), rela[i].r_offset,
                            rela[i].r_addend, destination, symbol_addr))
                        return false;
                }
                break;
            }
        }
    }

    return 1;
}

int Module_ElfLinkOne(char type, size_t offset, int addend, void *destination, uint32_t symbol_addr) {
    DEBUG_FUNCTION_LINE("Module_ElfLinkOne\n");
    int value;
    char *target = (char *)destination + offset;
    int result = 0;

    switch (type) {
        case R_PPC_ADDR32:
        case R_PPC_ADDR24:
        case R_PPC_ADDR16:
        case R_PPC_ADDR16_HI:
        case R_PPC_ADDR16_HA:
        case R_PPC_ADDR16_LO:
        case R_PPC_ADDR14:
        case R_PPC_ADDR14_BRTAKEN:
        case R_PPC_ADDR14_BRNTAKEN:
        case R_PPC_UADDR32:
        case R_PPC_UADDR16: {
            value = (int)symbol_addr + addend;
            break;
        } case R_PPC_REL24:
        case R_PPC_REL14:
        case R_PPC_REL14_BRTAKEN:
        case R_PPC_REL14_BRNTAKEN:
        case R_PPC_REL32:
        case R_PPC_ADDR30: {
            value = (int)symbol_addr + addend - (int)target;
            break;
        } case R_PPC_SECTOFF:
        case R_PPC_SECTOFF_LO:
        case R_PPC_SECTOFF_HI:
        case R_PPC_SECTOFF_HA: {
            value = offset + addend;
            break;
        } case R_PPC_EMB_NADDR32:
        case R_PPC_EMB_NADDR16:
        case R_PPC_EMB_NADDR16_LO:
        case R_PPC_EMB_NADDR16_HI:
        case R_PPC_EMB_NADDR16_HA: {
            value = addend - (int)symbol_addr;
            break;
        } default:
            goto exit_error;
    }


    switch (type) {
        case R_PPC_ADDR32:
        case R_PPC_UADDR32:
        case R_PPC_REL32:
        case R_PPC_SECTOFF:
        case R_PPC_EMB_NADDR32: {
            *(int *)target = value;
            break;
        } case R_PPC_ADDR24:
        case R_PPC_REL24: {
            *(int *)target =
                (*(int *)target & 0xfc000003) | (value & 0x03fffffc);
            break;
        } case R_PPC_ADDR16:
        case R_PPC_UADDR16:
        case R_PPC_EMB_NADDR16: {
            *(short *)target = value;
            break;
        } case R_PPC_ADDR16_HI:
        case R_PPC_SECTOFF_HI:
        case R_PPC_EMB_NADDR16_HI: {
            *(short *)target = value >> 16;
            break;
        } case R_PPC_ADDR16_HA:
        case R_PPC_SECTOFF_HA:
        case R_PPC_EMB_NADDR16_HA: {
            *(short *)target = (value >> 16) + ((value >> 15) & 1);
            break;
        } case R_PPC_ADDR16_LO:
        case R_PPC_SECTOFF_LO:
        case R_PPC_EMB_NADDR16_LO: {
            *(short *)target = value & 0xffff;
            break;
        } case R_PPC_ADDR14:
        case R_PPC_REL14: {
            *(int *)target =
                (*(int *)target & 0xffff0003) | (value & 0x0000fffc);
            break;
        } case R_PPC_ADDR14_BRTAKEN:
        case R_PPC_REL14_BRTAKEN: {
            *(int *)target =
                (*(int *)target & 0xffdf0003) | (value & 0x0000fffc) |
                0x00200000;
            break;
        } case R_PPC_ADDR14_BRNTAKEN:
        case R_PPC_REL14_BRNTAKEN: {
            *(int *)target =
                (*(int *)target & 0xffdf0003) | (value & 0x0000fffc);
            break;
        } case R_PPC_ADDR30: {
            *(int *)target =
                (*(int *)target & 0x00000003) | (value & 0xfffffffc);
            break;
        } default:
            goto exit_error;
    }

    result = 1;
exit_error:
    if (!result) DEBUG_FUNCTION_LINE("Module_ElfLinkOne: exit_error\n");
    return result;
}
