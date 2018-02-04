#ifndef _OWN_UTILS_H_
#define _OWN_UTILS_H_

/* Main */
#ifdef __cplusplus
extern "C" {
#endif

void dumpHex(const void* data, size_t size);

#ifdef __cplusplus
}
#endif

void *Module_ListAllocate(void *list, size_t entry_size, size_t num,size_t *capacity, size_t *count, size_t default_capacity);

#endif
