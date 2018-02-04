#ifndef _MODULE_PARSER_H_
#define _MODULE_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libelf.h>

#ifdef __cplusplus
}
#endif

#include <wups.h>
#include <stdbool.h>
#include <vector>

typedef struct {
    const char *path;
    const char *game;
    const char *name;
    const char *author;
    const char *version;
    const char *license;
    size_t size;
    size_t entries_count;
} module_metadata_t;

typedef struct {
    const char *name;
    void *address;
    size_t offset;
    char type;
    int addend;
} module_unresolved_relocation_t;

typedef struct {
    std::vector<module_unresolved_relocation_t *> rel;
    std::vector<wups_loader_entry_t *> entries;
    module_metadata_t * metadata;
} module_information_t;

bool Module_CheckFile(const char *path);
void printModuleInformation(module_information_t* module_information);
module_information_t* Module_LoadModuleInformation(const char *path);

#endif
