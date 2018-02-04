#ifndef _ELF_UTILS_H_
#define _ELF_UTILS_H_

/* Main */
#ifdef __cplusplus
extern "C" {
#endif

#include <libelf.h>

#ifdef __cplusplus
}
#endif

#include "module_parser.h"

bool Module_ElfLoadSection(const Elf *elf, Elf_Scn *scn, const Elf32_Shdr *shdr,void *destination);
bool Module_LoadElfSymtab(Elf *elf, Elf32_Sym **symtab, size_t *symtab_count, size_t *symtab_strndx);
void Module_ElfLoadSymbols(size_t shndx, const void *destination, Elf32_Sym *symtab, size_t symtab_count);
bool Module_ElfLink(size_t index, Elf *elf, size_t shndx, void *destination, Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx, bool allow_globals);
bool Module_ElfLinkOne(char type, size_t offset, int addend, void *destination, uint32_t symbol_addr);

extern size_t module_relocations_count;
extern module_unresolved_relocation_t *module_relocations;
extern size_t module_relocations_capacity;
#endif
