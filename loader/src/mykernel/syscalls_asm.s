.globl SC0x36_KernelReadSRs
SC0x36_KernelReadSRs:
    li r0, 0x3600
    sc
    blr

     .globl SC0x0A_KernelWriteSRs
SC0x0A_KernelWriteSRs:
    li r0, 0x0A00
    sc
    blr

     .globl SC0x0A_KernelWriteWitoutDAT
SC0x0A_KernelWriteWitoutDAT:
    li r0, 0x0A00
    sc
    blr

     .globl SC0x37_KernelReadPTE
SC0x37_KernelReadPTE:
    li r0, 0x3700
    sc
    blr

     .globl SC0x09_KernelWritePTE
SC0x09_KernelWritePTE:
    li r0, 0x0900
    sc
    blr

