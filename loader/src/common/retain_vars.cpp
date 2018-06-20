#include "retain_vars.h"
#include "myutils/overlay_helper.h"
replacement_data_t gbl_replacement_data __attribute__((section(".data")));
dyn_linking_relocation_data_t gbl_dyn_linking_data __attribute__((section(".data")));

uint8_t gAppStatus __attribute__((section(".data"))) = 0;
uint64_t gGameTitleID __attribute__((section(".data"))) = 0;
volatile uint8_t gSDInitDone __attribute__((section(".data"))) = 0;

void * ntfs_mounts __attribute__((section(".data"))) = NULL;
int32_t ntfs_mount_count __attribute__((section(".data"))) = 0;

struct buffer_store drc_store __attribute__((section(".data")));
struct buffer_store tv_store __attribute__((section(".data")));


char gbl_common_data[0x20000] __attribute__((section(".data")));
char * gbl_common_data_ptr __attribute__((section(".data"))) = gbl_common_data;
