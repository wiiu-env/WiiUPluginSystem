#ifndef RETAINS_VARS_H_
#define RETAINS_VARS_H_
#include "patcher/function_patcher.h"

extern replacement_data_t gbl_replacement_data;
extern u8 gAppStatus;
extern volatile u8 gSDInitDone;

extern void * ntfs_mounts;
extern int ntfs_mount_count;

#endif // RETAINS_VARS_H_
