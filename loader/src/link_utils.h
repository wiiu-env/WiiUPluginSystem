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

bool Module_LinkModule(module_information_t * module_information, uint8_t **space);
bool Module_LinkModuleElf(module_information_t * module_information, Elf *elf, uint8_t **space);
#endif
