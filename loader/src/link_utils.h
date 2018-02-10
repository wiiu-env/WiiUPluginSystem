#ifndef _LINK_UTILS_H_
#define _LINK_UTILS_H_

/* Main */
#ifdef __cplusplus
extern "C" {
#endif

#include <libelf.h>

#ifdef __cplusplus
}
#endif

#include "module_parser.h"

bool Module_ListLink(uint8_t **space);
bool Module_LinkModule(size_t index, const char *path, uint8_t **space);
bool Module_LinkModuleElf(size_t index, Elf *elf, uint8_t **space);
bool Module_ListLinkFinal();

extern wups_loader_entry_t *module_entries;
extern size_t module_entries_count;
extern size_t module_entries_capacity;

#endif
