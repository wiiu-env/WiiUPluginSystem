#include <wups.h>

#ifdef __cplusplus
extern "C" {
#endif

static VideoMemoryAllocFunction vid_mem_alloc_ptr   __attribute__((section(".data"))) = NULL;
static VideoMemoryFreeFunction  vid_mem_free_ptr    __attribute__((section(".data"))) = NULL;

void WUPS_InitVidMem(wups_loader_init_vid_mem_args_t args) {
    vid_mem_alloc_ptr =  args.vid_mem_alloc_ptr;
    vid_mem_free_ptr =  args.vid_mem_free_ptr;
}

void * WUPS_VideoMemMemalign(uint32_t size, int32_t align) {
     if(vid_mem_alloc_ptr != NULL) {
        return vid_mem_alloc_ptr(size, align);
    }
    return 0;
}

void * WUPS_VideoMemAlloc(uint32_t size) {
    return WUPS_VideoMemMemalign(size, 4);
}

void WUPS_VideoMemFree(void *addr) {
    if(vid_mem_free_ptr != NULL) {
        vid_mem_free_ptr(addr);
    }
}

#ifdef __cplusplus
}
#endif
