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
    const char *name;
    const char *author;
    const char *version;
    const char *license;
    size_t size;
    size_t entries_count;
} module_metadata_t;

typedef struct {
    size_t module;
    const char *name;
    void *address;
    size_t offset;
    char type;
    int addend;
} module_unresolved_relocation_t;

extern bool module_has_error;
/* whether or not to delay loading for debug messages. */
extern bool module_has_info;

extern size_t module_list_size;
extern module_metadata_t **module_list;
extern size_t module_list_count;

bool Module_CheckFile(const char *path);
void Module_Load(const char *path);

#endif
