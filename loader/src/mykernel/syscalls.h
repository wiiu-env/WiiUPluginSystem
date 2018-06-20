#ifndef __WUPS_SYSCALLS_H_
#define __WUPS_SYSCALLS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "kernel_defs.h"

void SC0x0A_KernelWriteSRs(sr_table_t * table);
void SC0x36_KernelReadSRs(sr_table_t * table);
void SC0x37_KernelReadPTE(uint32_t* dest, uint32_t size);
void SC0x09_KernelWritePTE(uint32_t* addr,uint32_t size);
void KernelTest();

#ifdef __cplusplus
}
#endif

#endif // __WUPS_SYSCALLS_H_
