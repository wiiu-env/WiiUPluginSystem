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

int Module_ElfLoadSection(const Elf *elf, Elf_Scn *scn, const Elf32_Shdr *shdr, void *destination);
int Module_LoadElfSymtab(Elf *elf, Elf32_Sym **symtab, size_t *symtab_count,size_t *symtab_strndx);
void Module_ElfLoadSymbols(size_t shndx, const void *destination, Elf32_Sym *symtab, size_t symtab_count);
bool Module_ElfLink(module_information_t * module_information, Elf *elf, size_t shndx, void *destination,Elf32_Sym *symtab, size_t symtab_count, size_t symtab_strndx,int allow_globals);
int Module_ElfLinkOne(char type, size_t offset, int addend, void *destination, uint32_t symbol_addr);
void *Module_ListAllocate(void *list, size_t entry_size, size_t num, size_t *capacity, size_t *count, size_t default_capacity);

#endif
