#include "retain_vars.h"
replacement_data_t gbl_replacement_data __attribute__((section(".data")));
u8 gAppStatus __attribute__((section(".data"))) = 0;
volatile u8 gSDInitDone __attribute__((section(".data"))) = 0;

void * ntfs_mounts __attribute__((section(".data"))) = NULL;
int ntfs_mount_count __attribute__((section(".data"))) = 0;
