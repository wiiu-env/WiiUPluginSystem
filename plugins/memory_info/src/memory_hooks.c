#include <wups.h>
#include <dynamic_libs/os_types.h>
#include "memory_defs.h"
#include "c_retain_vars.h"

DECL_FUNCTION(s32, MEMCreateFrmHeapEx, void* address, u32 size, u16 flags) {
    s32 result = real_MEMCreateFrmHeapEx(address,size,flags);
    for(int i = 0; i<MAX_FRM_HEAP_INFOS; i++) {
        if(frmHeapPointer[i].handle == 0) {
            frmHeapPointer[i].handle = result;
            frmHeapPointer[i].address = address;
            frmHeapPointer[i].size = size;
            frmHeapPointer[i].flags = flags;
            break;
        }
    }
    return result;
}

DECL_FUNCTION(s32, MEMCreateUserHeapHandle, void* startAddress, u32 size) {
    s32 result = real_MEMCreateUserHeapHandle(startAddress,size);
    for(int i = 0; i<MAX_USER_HEAP_INFOS; i++) {
        if(userHeapInfos[i].handle == 0) {
            userHeapInfos[i].handle = result;
            userHeapInfos[i].address = startAddress;
            userHeapInfos[i].size = size;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(s32, MEMCreateExpHeapEx, void* address, u32 size, u16 flags) {
    u32 oldSize = size;
    s32 result = 0;
    bool allocated = false;

    /*
    real_MEMCreateExpHeapEx Just accepts ALL sizes... so.. well..
    if(size > 0x3B600000 && (u32) address < 0x11000000){
        //We are pretty sure this the MEM2 default heap.
        //Let's try to increase it's capacity.
        //u32 maximumSize = 0x50000000 - (u32) address;
        u32 maximumSize = 0x80000000;
        //u32 maximumSize = size + 0x500000;
        maximumSize -= 0x500000; // remove 5MB, just to leave at least _some_ space.

        u32 curSizeTest = maximumSize;

        do{
            if(curSizeTest < oldSize){
                break;
            }
            result = real_MEMCreateExpHeapEx(address,curSizeTest,flags);
            if(result == 0){ // On failure:
                maximumSize -= 0x100000; // reduce by 1MB and try again.
            }else{
                allocated = true;
                break;
            }
        }while(!allocated); // Until the allocation was successful, or we are below default size.
    }*/
    if(!allocated || result == 0) {
        result = real_MEMCreateExpHeapEx(address,size,flags);
    }

    for(int i = 0; i<MAX_EXP_HEAP_INFOS; i++) {
        if(expHeapPointer[i].handle == 0) {
            expHeapPointer[i].handle = result;
            expHeapPointer[i].address = address;
            expHeapPointer[i].size = size;
            expHeapPointer[i].oldSize = oldSize;
            expHeapPointer[i].flags = flags;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(s32, MEMCreateUnitHeapEx, void* address, u32 heapSize, u32 memSize, u32 align, u16 flags) {
    s32 result = real_MEMCreateUnitHeapEx(address,heapSize,memSize,align,flags);

    for(int i = 0; i<MAX_UNIT_HEAP_INFOS; i++) {
        if(unitHeapPointer[i].handle == 0) {
            unitHeapPointer[i].handle = result;
            unitHeapPointer[i].address = address;
            unitHeapPointer[i].heapSize = heapSize;
            unitHeapPointer[i].memSize = memSize;
            unitHeapPointer[i].align = align;
            unitHeapPointer[i].flags = flags;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(void *, MEMDestroyExpHeap, s32 heap) {
    void * result = real_MEMDestroyExpHeap(heap);

    for(int i = 0; i<MAX_EXP_HEAP_INFOS; i++) {
        if(expHeapPointer[i].handle == heap) {
            expHeapPointer[i].handle = 0;
            expHeapPointer[i].address = 0;
            expHeapPointer[i].size = 0;
            expHeapPointer[i].flags = 0;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(void *, MEMDestroyFrmHeap, s32 heap) {
    void * result = real_MEMDestroyFrmHeap(heap);

    for(int i = 0; i<MAX_FRM_HEAP_INFOS; i++) {
        if(frmHeapPointer[i].handle == heap) {
            frmHeapPointer[i].handle = 0;
            frmHeapPointer[i].address = 0;
            frmHeapPointer[i].size = 0;
            frmHeapPointer[i].flags = 0;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(void *, MEMDestroyUnitHeap, s32 heap) {
    void * result = real_MEMDestroyUnitHeap(heap);

    for(int i = 0; i<MAX_UNIT_HEAP_INFOS; i++) {
        if(unitHeapPointer[i].handle == heap) {
            unitHeapPointer[i].handle = 0;
            unitHeapPointer[i].address = 0;
            unitHeapPointer[i].heapSize = 0;
            unitHeapPointer[i].memSize = 0;
            unitHeapPointer[i].align = 0;
            unitHeapPointer[i].flags = 0;
            break;
        }
    }

    return result;
}

DECL_FUNCTION(s32, MEMSetBaseHeapHandle, s32 arena, s32 heap) {
    s32 result = real_MEMSetBaseHeapHandle(arena,heap);
    for(int i = 0; i<MAX_BASE_HEAP_INFOS; i++) {
        if(baseHeaphandles[i].newHandle == 0) {
            baseHeaphandles[i].oldHandle = result;
            baseHeaphandles[i].newHandle = heap;
            baseHeaphandles[i].memArena = arena;
            break;
        }
    }
    return result;
}

DECL_FUNCTION(s32, MEMInitBlockHeap,void * tracking, void * startAddress,void * endAddress, void * initTrackMemory, u32 initTrackMemoryBytes, u16 flags) {
    s32 result = real_MEMInitBlockHeap(tracking,startAddress,endAddress,initTrackMemory,initTrackMemoryBytes,flags);
    for(int i = 0; i<MAX_BLOCK_HEAP_INFOS; i++) {
        if(blockHeapInfos[i].startAddress == NULL) {
            blockHeapInfos[i].handle = result;
            blockHeapInfos[i].tracking = tracking;
            blockHeapInfos[i].startAddress = startAddress;
            blockHeapInfos[i].endAddress = endAddress;
            blockHeapInfos[i].initTrackMem = initTrackMemory;
            blockHeapInfos[i].initTrackMemBytes = initTrackMemoryBytes;
            blockHeapInfos[i].flags = flags;
            break;
        }
    }
    return result;
}

WUPS_MUST_REPLACE(MEMInitBlockHeap,         WUPS_LOADER_LIBRARY_COREINIT,   MEMInitBlockHeap);
WUPS_MUST_REPLACE(MEMSetBaseHeapHandle,     WUPS_LOADER_LIBRARY_COREINIT,   MEMSetBaseHeapHandle);
WUPS_MUST_REPLACE(MEMDestroyUnitHeap,       WUPS_LOADER_LIBRARY_COREINIT,   MEMDestroyUnitHeap);
WUPS_MUST_REPLACE(MEMDestroyFrmHeap,        WUPS_LOADER_LIBRARY_COREINIT,   MEMDestroyFrmHeap);
WUPS_MUST_REPLACE(MEMDestroyExpHeap,        WUPS_LOADER_LIBRARY_COREINIT,   MEMDestroyExpHeap);
WUPS_MUST_REPLACE(MEMCreateUnitHeapEx,      WUPS_LOADER_LIBRARY_COREINIT,   MEMCreateUnitHeapEx);
WUPS_MUST_REPLACE(MEMCreateExpHeapEx,       WUPS_LOADER_LIBRARY_COREINIT,   MEMCreateExpHeapEx);
WUPS_MUST_REPLACE(MEMCreateUserHeapHandle,  WUPS_LOADER_LIBRARY_COREINIT,   MEMCreateUserHeapHandle);
WUPS_MUST_REPLACE(MEMCreateFrmHeapEx,       WUPS_LOADER_LIBRARY_COREINIT,   MEMCreateFrmHeapEx);

/*
Unused stuff that might get useful in the future.

void * allocateFromExpHeap(s32 * usedHandle, u32 size, u32 align){
    void * result = NULL;
    for(int i = 0;i<MAX_EXP_HEAP_INFOS;i++){
        if(expHeapPointer[i].handle != 0){
            result = MEMAllocFromExpHeapEx(expHeapPointer[i].handle, size, align);
            if(result != NULL){
                *usedHandle = expHeapPointer[i].handle;
                return result;
            }
        }
    }
    return NULL;
}

void * allocateFromFrmHeap(s32 * usedHandle, u32 size, u32 align){
    void * result = NULL;
    for(int i = 0;i<MAX_FRM_HEAP_INFOS;i++){
        if(frmHeapPointer[i].handle != 0){
            result = MEMAllocFromFrmHeapEx(frmHeapPointer[i].handle, size, align);
            if(result != NULL){
                *usedHandle = frmHeapPointer[i].handle;
                return result;
            }
        }
    }
    return NULL;
}

void freeToExpHeap(s32 usedHandle, void * addr){
    MEMFreeToExpHeap(usedHandle, addr);
}
allocatorExpInfo allocatorExpPointer[MAX_ALLOCATOR_EXP_INFOS] __attribute__((section(".data")));

typedef struct allocatorExpInfo_{
    void * allocator;
    s32 heap;
    s32 align;
} allocatorExpInfo;

DECL(void , MEMInitAllocatorForExpHeap, void* allocator, s32 heap, s32 align){
    for(int i = 0;i<MAX_ALLOCATOR_EXP_INFOS;i++){
        if(allocatorExpPointer[i].allocator == 0){
            allocatorExpPointer[i].allocator = (void*)allocator;
            allocatorExpPointer[i].heap = heap;
            allocatorExpPointer[i].align = align;
            break;
        }
    }
    real_MEMInitAllocatorForExpHeap(allocator,heap,align);
    return;
}
*/
