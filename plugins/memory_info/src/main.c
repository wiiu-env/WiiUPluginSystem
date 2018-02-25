#include <wups.h>
#include <string.h>
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <utils/logger.h>
#include "main.h"

WUPS_PLUGIN_NAME("Memory Info");
WUPS_PLUGIN_DESCRIPTION("Prints information about the current memory usage when pressing L+R+X+Y+A+B.");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");


ON_APPLICATION_START(args) {
    InitOSFunctionPointers();
    InitSocketFunctionPointers();
    log_init();
    //InitVPadFunctionPointers();
}

void printMemHeapInfo(memHeapInfo * listStart, u32 listLength, const char * listname) {
    for(int i = 0; i<listLength; i++) {
        if(listStart[i].handle != 0) {
            s32 parent = MEMFindParentHeap(listStart[i].handle);
            DEBUG_FUNCTION_LINE("%s:", listname);
            if(parent == 0) {
                log_print(" parent:    <NONE>  ");
            } else {
                log_printf(" parent: 0x%08X ", parent);
            }
            log_printf(" address 0x%08X handle 0x%08X size %07d kb old size %07d kb flags 0x%04X\n",listStart[i].address,listStart[i].handle,listStart[i].size/1024,listStart[i].oldSize/1024,listStart[i].flags);
        }
    }
}

void print_memory_info() {
    DEBUG_FUNCTION_LINE("Print expanded heap infos\n");
    printMemHeapInfo(expHeapPointer,MAX_EXP_HEAP_INFOS, "Expanded heap:");

    DEBUG_FUNCTION_LINE("Print frmHeap\n");
    printMemHeapInfo(frmHeapPointer,MAX_FRM_HEAP_INFOS, "Frame heap:");

    DEBUG_FUNCTION_LINE("Print unitHeap\n");
    for(int i = 0; i<MAX_UNIT_HEAP_INFOS; i++) {
        if(unitHeapPointer[i].handle != 0) {
            DEBUG_FUNCTION_LINE("unitHeap:  address 0x%08X handle 0x%08X heapSize %07d kb memSize %07d kb align 0x%08X flags 0x%04X\n",unitHeapPointer[i].address,unitHeapPointer[i].handle,unitHeapPointer[i].heapSize/1024,unitHeapPointer[i].memSize/1024,unitHeapPointer[i].align,unitHeapPointer[i].flags);
        }
    }

    DEBUG_FUNCTION_LINE("Print set base heap info\n");
    for(int i = 0; i<MAX_BASE_HEAP_INFOS; i++) {
        if(baseHeaphandles[i].newHandle != 0) {
            DEBUG_FUNCTION_LINE("MEMSetBaseHeapHandle was called: oldHandle 0x%08X  newHandle 0x%08X arena %d\n",baseHeaphandles[i].oldHandle,baseHeaphandles[i].newHandle,baseHeaphandles[i].memArena);
        }
    }
    DEBUG_FUNCTION_LINE("Print set block heap info\n");
    for(int i = 0; i<MAX_BLOCK_HEAP_INFOS; i++) {
        if(blockHeapInfos[i].startAddress != 0) {
            DEBUG_FUNCTION_LINE("MEMInitBlockHeap was called: handle 0x%08X tracking 0x%08X\n",blockHeapInfos[i].handle,blockHeapInfos[i].tracking);
        }
    }
    DEBUG_FUNCTION_LINE("Print set user heap info\n");
    printMemHeapInfo(userHeapInfos,MAX_USER_HEAP_INFOS, "User heap:");

    DEBUG_FUNCTION_LINE("Testing malloc\n");
    u32 size = 0x1000;
    bool success = false;
    do {
        void * result = malloc(size);
        if(result != NULL) {
            //DEBUG_FUNCTION_LINE("Allocated %d kb from default heap\n",size/1024);
            free(result);
            result = NULL;
            success = true;
        } else {
            //DEBUG_FUNCTION_LINE("Failed to allocate %d kb from default heap\n",size/1024);
            success = false;
        }
        size += 0x1000;
    } while(success);
    DEBUG_FUNCTION_LINE("I was able to allocate %07d kb from default heap (alignment 4)\n",size/1024);

    for(int i = 0; i<2; i++) {
        s32 defaultHeap = MEMGetBaseHeapHandle(i);
        if(defaultHeap != 0) {
            u32 start = 0;
            u32 size_bytes = 0;
            OSGetMemBound(i+1,&start,&size_bytes);
            DEBUG_FUNCTION_LINE("Memory Bound MEM%d: startAddress 0x%08X size 0x%08X\n",i+1,start,size_bytes);

            s32 size = MEMGetAllocatableSizeForExpHeapEx(defaultHeap, 4);
            s32 totalSize = MEMGetTotalFreeSizeForExpHeap(defaultHeap);
            DEBUG_FUNCTION_LINE("BaseHandle address 0x%08X: MEM%d with %07d kb memory free in one block, %07d kb in total.\n",defaultHeap,i+1,size/1024,totalSize/1024);

            s32 parent = MEMFindParentHeap(defaultHeap);
            if(parent != 0) {
                size = MEMGetAllocatableSizeForExpHeapEx(parent, 4);
                s32 totalSize = MEMGetTotalFreeSizeForExpHeap(parent);
                DEBUG_FUNCTION_LINE("It's parent heap is 0x%08X: With %07d kb memory free in one block, %07d kb in total.\n",parent,size/1024,totalSize/1024);
            } else {
                DEBUG_FUNCTION_LINE("No parent found =(\n");
            }
        }
    }
}
