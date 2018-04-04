#ifndef __WUPS_MEMORY_MAPPING_H_
#define __WUPS_MEMORY_MAPPING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <dynamic_libs/os_types.h>
#include "mykernel/kernel_defs.h"

typedef struct pageInformation_ {
    uint32_t addr;
    uint32_t size;
    uint32_t ks;
    uint32_t kp;
    uint32_t nx;
    uint32_t pp;
    uint32_t phys;
} pageInformation;

typedef struct _memory_values_t {
    unsigned int start_address;
    unsigned int end_address;
} memory_values_t;

typedef struct _memory_mapping_t {
    unsigned int effective_start_address;
    const memory_values_t* physical_addresses;
} memory_mapping_t;


#define SEGMENT_UNIQUE_ID           0x00AABBCC // Unique ID. Chosen arbitrary.

#define MEMORY_START_BASE           0x80000000 // At most: 128MB for plugins heap.
#define MEMORY_START_PLUGIN_HEAP    MEMORY_START_BASE + 0x08000000 // At most: 128MB for plugins heap.
#define MEMORY_START_PLUGIN_SPACE   MEMORY_START_BASE + 0x00800000 // At most: 120MB for plugins.
#define MEMORY_START_PLUGIN_LOADER  MEMORY_START_BASE + 0x00000000 // At most: 8MB for the plugin loader.

const memory_values_t mem_vals_loader[] = {
    {0x28000000 + 0x06620000          , 0x28000000 + 0x06E20000},               // 8MB          0x80000000 0x80800000 -> 0x2E700000 0x2EF00000
    {0,0}
};

const memory_values_t mem_vals_plugins[] = {
    {0x28000000 + 0x06E20000          , 0x28000000 + 0x07E20000},                // 16MB         0x80800000 0x81800000 -> 0x2EF00000 0x2FF00000
    {0,0}
};

// Values needs to be aligned to 0x20000 and size needs to be a multiple of 0x20000
const memory_values_t mem_vals_heap[] = {
    // 5.5.2 EUR
    {0x28000000 + 0x000E0000, 0x28000000 + 0x00160000}, // size: 512 kB
    {0x28000000 + 0x003C0000, 0x28000000 + 0x004C0000}, // size: 1024 kB
    {0x28000000 + 0x00900000, 0x28000000 + 0x00B00000}, // size: 2048 kB
    {0x28000000 + 0x00E40000, 0x28000000 + 0x00EC0000}, // size: 512 kB
    {0x28000000 + 0x00EE0000, 0x28000000 + 0x00F60000}, // size: 512 kB
    {0x28000000 + 0x00FA0000, 0x28000000 + 0x01020000}, // size: 512 kB
    {0x28000000 + 0x01040000, 0x28000000 + 0x013C0000}, // size: 3584 kB
    {0x28000000 + 0x02060000, 0x28000000 + 0x021A0000}, // size: 1280 kB
    {0x28000000 + 0x02BC0000, 0x28000000 + 0x02CA0000}, // size: 896 kB
    {0x28000000 + 0x04B60000, 0x28000000 + 0x04B80000}, // size: 128 kB
    {0x28000000 + 0x053C0000, 0x28000000 + 0x05880000}, // size: 4864 kB
    {0x28000000 + 0x058E0000, 0x28000000 + 0x06000000}, // size: 7296 kB
    {0x28000000 + 0x07E20000, 0x28000000 + 0x07F80000}, // size: 1408 kB
    {0x28000000 + 0x080E0000, 0x28000000 + 0x08180000}, // size: 640 kB
    {0x28000000 + 0x083C0000, 0x28000000 + 0x084C0000}, // size: 1024 kB
    {0x28000000 + 0x086E0000, 0x28000000 + 0x08760000}, // size: 512 kB
    {0x28000000 + 0x08C20000, 0x28000000 + 0x08F20000}, // size: 3072 kB
    {0x28000000 + 0x09000000, 0x28000000 + 0x09E20000}, // size: 14464 kB

    // Not usable on 5.5.2
    //
    // Used in notifications {0x28000000 + 0x01720000, 0x28000000 + 0x018A0000}, // size: 1536 kB
    //                       {0x28000000 + 0x03820000, 0x28000000 + 0x038C0000}, // size: 640 kB
    //                       {0x28000000 + 0x03920000, 0x28000000 + 0x039A0000}, // size: 512 kB
    // Used in notifications {0x28000000 + 0x04B80000, 0x28000000 + 0x051E0000}, // size: 6528 kB
    //                       {0x28000000 + 0x08F20000, 0x28000000 + 0x09000000}, // size: 896 kB

    // Porting to other/newer firmware:
    // Map this to check for free regions.
    // Use MemoryMapper::testFreeMemory() to see regions with are 0x00000000;
    // Then map the promising regions, and do the write/read check.
    // Writing numbers into the area, open the home menu and all background apps an check if anything was
    // overridden.
    // {0x28000000 + 0x00000000, 0x28000000 + 0x0A000000},              //

    {0,0}
};

const memory_mapping_t mem_mapping[] = {
    {MEMORY_START_PLUGIN_LOADER,    mem_vals_loader},
    {MEMORY_START_PLUGIN_SPACE,     mem_vals_plugins},
    {MEMORY_START_PLUGIN_HEAP,      mem_vals_heap},
    {0,NULL}
};

class MemoryMapping {

public:
    static bool isMemoryMapped();

    static void setupMemoryMapping();

    static void printPageTableTranslation(sr_table_t srTable, u32 * translation_table);

    static void writeTestValuesToMemory();

    static void readTestValuesFromMemory();

    static void searchEmptyMemoryRegions();

    static u32 getHeapAddress();

    static u32 getHeapSize();

private:

    static void memoryMappingForRegions(const memory_mapping_t * memory_mapping, sr_table_t SRTable, u32 * translation_table);

    static bool mapMemory(u32 pa_start_address,u32 pa_end_address,u32 ea_start_address, sr_table_t SRTable, u32 * translation_table);
};


#ifdef __cplusplus
}
#endif

#endif // __KERNEL_DEFS_H_
