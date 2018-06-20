#include <dynamic_libs/os_defs.h>
#include <dynamic_libs/os_functions.h>
#include "kernel_defs.h"
#include <kernel/syscalls.h>

static void KernelReadSRs(sr_table_t * table) {
    uint32_t i = 0;

    // calculate PT_size ((end-start)*8/4096)*4 or (end-start)/128
    // Minimum page table size is 64Kbytes.

    asm volatile("eieio; isync");

    asm volatile("mfspr %0, 25" : "=r" (table->sdr1));

    asm volatile("mfsr %0, 0" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 1" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 2" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 3" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 4" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 5" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 6" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 7" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 8" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 9" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 10" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 11" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 12" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 13" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 14" : "=r" (table->value[i]));
    i++;
    asm volatile("mfsr %0, 15" : "=r" (table->value[i]));
    i++;

    asm volatile("eieio; isync");
}

static void KernelWriteSRs(sr_table_t * table) {
    uint32_t i = 0;


    asm volatile("eieio; isync");

    // Writing didn't work for all at once so we only write number 8.
    // TODO: fix this and change it if required.

     /*asm volatile("mtsr 0, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 1, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 2, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 3, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 4, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 5, %0" : : "r" (table->value[i])); i++;*/
     //asm volatile("mtsr 6, %0" : : "r" (table->value[6])); i++;
     /*asm volatile("mtsr 7, %0" : : "r" (table->value[i])); i++;*/
     asm volatile("mtsr 8, %0" : : "r" (table->value[8])); i++;
     /*asm volatile("mtsr 9, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 10, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 11, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 12, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 13, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 14, %0" : : "r" (table->value[i])); i++;
     asm volatile("mtsr 15, %0" : : "r" (table->value[i])); i++;*/


    asm volatile("isync");
}

void KernelReadPTE(uint32_t* dest, uint32_t size) {
    uint32_t msr = 0;
    uint32_t oldmsr = 0;
    //TODO: Calculate from SDR1
    uint32_t addr_base = 0xFFE20000;
    asm volatile("mfmsr %0" : "=r" (msr));
    oldmsr = msr;
    msr &= ~0x10;
    for(uint32_t i = 0;i<size/0x04;i++){
        uint32_t value_read = 0;
        uint32_t addr = addr_base + (i*4);
        // Disable Data address translation
        asm volatile("mtmsr %0" : : "r" (msr));
        __asm __volatile("lwz %0,0(%1)" : "=r"(value_read) : "r"(addr));
        // Enable Data address translation
        asm volatile("mtmsr %0" : : "r" (oldmsr));
        dest[i] = value_read;
    }
}

void KernelWritePTE(uint32_t * in_addr, uint32_t size) {
    uint32_t msr = 0;
    uint32_t oldmsr = 0;
    //TODO: Calculate from SDR1
    uint32_t addr_base = 0xFFE20000;
    asm volatile("mfmsr %0" : "=r" (msr));
    oldmsr = msr;
    msr &= ~0x10;
    for(uint32_t i = 0;i<size/0x04;i++){
        uint32_t addr = addr_base + (i*4);
        uint32_t value = in_addr[i];
        // Disable Data address translation
        asm volatile("mtmsr %0" : : "r" (msr));
        __asm __volatile("stw %0,0(%1)" : : "r"(value),"r"(addr));
        // Enable Data address translation
        asm volatile("mtmsr %0" : : "r" (oldmsr));
    }
}

void KernelWriteWitoutDAT(uint32_t addr, uint32_t value) {
    uint32_t msr = 0;
    uint32_t oldmsr = 0;
    //TODO: Calculate from SDR1
    //uint32_t addr_base = 0xFFE20000;
    asm volatile("mfmsr %0" : "=r" (msr));
    oldmsr = msr;
    msr &= ~0x10;
    // Disable Data address translation
    asm volatile("mtmsr %0" : : "r" (msr));
    __asm __volatile("stw %0,0(%1)" : : "r"(value),"r"(addr));
    // Enable Data address translation
    asm volatile("mtmsr %0" : : "r" (oldmsr));
}

void SC0x0A_KernelWriteWitoutDAT(uint32_t addr,uint32_t value);

void wups_init_kernel_syscalls(){
    //! assign 1 so that this variable gets into the retained .data section
    static uint8_t ucSyscallsSetupRequired = 1;
    if(!ucSyscallsSetupRequired)
        return;

    ucSyscallsSetupRequired = 0;

    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl1 + (0x0A * 4)), (uint32_t)KernelWriteWitoutDAT);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl2 + (0x0A * 4)), (uint32_t)KernelWriteWitoutDAT);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl3 + (0x0A * 4)), (uint32_t)KernelWriteWitoutDAT);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl4 + (0x0A * 4)), (uint32_t)KernelWriteWitoutDAT);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl5 + (0x0A * 4)), (uint32_t)KernelWriteWitoutDAT);

    // Override all writes to SR8 with nops.
    SC0x0A_KernelWriteWitoutDAT(0xFFF1D754,0x60000000);
    SC0x0A_KernelWriteWitoutDAT(0xFFF1D64C,0x60000000);
    SC0x0A_KernelWriteWitoutDAT(0xFFE00638,0x60000000);

    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl1 + (0x36 * 4)), (uint32_t)KernelReadSRs);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl2 + (0x36 * 4)), (uint32_t)KernelReadSRs);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl3 + (0x36 * 4)), (uint32_t)KernelReadSRs);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl4 + (0x36 * 4)), (uint32_t)KernelReadSRs);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl5 + (0x36 * 4)), (uint32_t)KernelReadSRs);

    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl1 + (0x37 * 4)), (uint32_t)KernelReadPTE);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl2 + (0x37 * 4)), (uint32_t)KernelReadPTE);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl3 + (0x37 * 4)), (uint32_t)KernelReadPTE);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl4 + (0x37 * 4)), (uint32_t)KernelReadPTE);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl5 + (0x37 * 4)), (uint32_t)KernelReadPTE);

    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl1 + (0x09 * 4)), (uint32_t)KernelWritePTE);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl2 + (0x09 * 4)), (uint32_t)KernelWritePTE);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl3 + (0x09 * 4)), (uint32_t)KernelWritePTE);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl4 + (0x09 * 4)), (uint32_t)KernelWritePTE);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl5 + (0x09 * 4)), (uint32_t)KernelWritePTE);

    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl1 + (0x0A * 4)), (uint32_t)KernelWriteSRs);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl2 + (0x0A * 4)), (uint32_t)KernelWriteSRs);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl3 + (0x0A * 4)), (uint32_t)KernelWriteSRs);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl4 + (0x0A * 4)), (uint32_t)KernelWriteSRs);
    kern_write((void*)(OS_SPECIFICS->addr_KernSyscallTbl5 + (0x0A * 4)), (uint32_t)KernelWriteSRs);
}
