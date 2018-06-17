#include "retain_vars.h"
#include "myutils/overlay_helper.h"
replacement_data_t gbl_replacement_data __attribute__((section(".data")));
dyn_linking_relocation_data_t gbl_dyn_linking_data __attribute__((section(".data")));

u8 gAppStatus __attribute__((section(".data"))) = 0;
u64 gGameTitleID __attribute__((section(".data"))) = 0;
volatile u8 gSDInitDone __attribute__((section(".data"))) = 0;

void * ntfs_mounts __attribute__((section(".data"))) = NULL;
int ntfs_mount_count __attribute__((section(".data"))) = 0;

struct buffer_store drc_store __attribute__((section(".data")));
struct buffer_store tv_store __attribute__((section(".data")));
