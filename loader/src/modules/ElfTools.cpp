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

#include "ElfTools.h"
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <libelf.h>
#include <utils/logger.h>

bool ElfTools::elfLoadSection(const Elf *elf, Elf_Scn *scn, const Elf32_Shdr *shdr,void *destination) {

    assert(destination != NULL);

    switch (shdr->sh_type) {
        case SHT_SYMTAB:
        case SHT_PROGBITS: {
            Elf_Data *data;
            size_t n;

            n = 0;
            for (data = elf_getdata(scn, NULL); data != NULL; data = elf_getdata(scn, data)) {
                memcpy((char *)destination + n, data->d_buf, data->d_size);
                n += data->d_size;
            }
            return true;
        } case SHT_NOBITS: {
            memset(destination, 0, shdr->sh_size);
            return true;
        } default:
            return false;
    }
}

bool ElfTools::loadElfSymtab(Elf *elf, Elf32_Sym **symtab, size_t *symtab_count, size_t *symtab_strndx) {
    Elf_Scn *scn;
    bool result = false;

    for (scn = elf_nextscn(elf, NULL);
         scn != NULL;
         scn = elf_nextscn(elf, scn)) {

        Elf32_Shdr *shdr;

        shdr = elf32_getshdr(scn);
        if (shdr == NULL)
            continue;

        if (shdr->sh_type == SHT_SYMTAB) {
            size_t sym;

            assert (*symtab == NULL);
            *symtab = (Elf32_Sym *)malloc(shdr->sh_size);
            if (*symtab == NULL)
                continue;

            *symtab_count = shdr->sh_size / sizeof(Elf32_Sym);
            *symtab_strndx = shdr->sh_link;

            if (!elfLoadSection(elf, scn, shdr, *symtab))
                goto exit_error;

            for (sym = 0; sym < *symtab_count; sym++){
                (*symtab)[sym].st_other = 0;
            }

            break;
        }
    }

    if (*symtab == NULL)
        goto exit_error;

    result = true;
exit_error:
    return result;
}


void ElfTools::elfLoadSymbols(size_t shndx, const void *destination, Elf32_Sym *symtab, size_t symtab_count) {
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


bool ElfTools::elfLink(Elf *elf, size_t shndx, void *destination, Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx, bool allow_globals) {
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

bool ElfTools::elfLinkOne(char type, size_t offset, int addend, void *destination, uint32_t symbol_addr) {
    int value;
    char *target = (char *)destination + offset;
    bool result = false;

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
        case R_PPC_PLTREL24:
        case R_PPC_LOCAL24PC:
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
            DEBUG_FUNCTION_LINE("Module_ElfLinkOne01: %02X\n",type);
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
        case R_PPC_PLTREL24:
        case R_PPC_LOCAL24PC:
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
        }default:
            DEBUG_FUNCTION_LINE("Module_ElfLinkOne01: %02X\n",type);
            goto exit_error;
    }

    result = true;
exit_error:
    if (!result) DEBUG_FUNCTION_LINE("Module_ElfLinkOne: exit_error\n");
    return result;
}

