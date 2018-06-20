#ifndef __WUPS_KERNEL_DEFS_H_
#define __WUPS_KERNEL_DEFS_H_

#include <dynamic_libs/fs_functions.h>
#include <dynamic_libs/os_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _sr_table_t
{
    uint32_t value[16];
    uint32_t sdr1;
} sr_table_t;

typedef struct _bat_t
{
    uint32_t h;
    uint32_t l;
} bat_t;

typedef struct _bat_table_t
{
    bat_t bat[8];
} bat_table_t;


#ifdef __cplusplus
}
#endif

#endif // __KERNEL_DEFS_H_
