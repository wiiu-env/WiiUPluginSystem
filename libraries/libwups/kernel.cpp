#include <wups.h>

#ifdef __cplusplus
extern "C" {
#endif

static KernelReadFunction       kern_read_ptr __attribute__((section(".data"))) = NULL;
static KernelWriteFunction      kern_write_ptr __attribute__((section(".data"))) = NULL;
static KernelCopyDataFunction   kern_copy_data_ptr __attribute__((section(".data"))) = NULL;

void WUPS_InitKernel(wups_loader_init_kernel_args_t args) {
    kern_read_ptr =         args.kern_read_ptr;
    kern_write_ptr =        args.kern_write_ptr;
    kern_copy_data_ptr =    args.kern_copy_data_ptr;
}

uint32_t WUPS_KernelRead(const void *addr) {
    if(kern_read_ptr != NULL) {
        return kern_read_ptr(addr);
    }
    return 0;
}

void WUPS_KernelWrite(void *addr, uint32_t value) {
    if(kern_write_ptr != NULL) {
        kern_write_ptr(addr, value);
    }
}
void WUPS_KernelCopyDataFunction(uint32_t addr, uint32_t src, uint32_t len) {
    if(kern_copy_data_ptr != NULL) {
        kern_copy_data_ptr(addr, src, len);
    }
}

#ifdef __cplusplus
}
#endif
