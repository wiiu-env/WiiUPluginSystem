#ifndef RETAINS_VARS_H_
#define RETAINS_VARS_H_
#include "patcher/function_patcher.h"
#include "plugin/dynamic_linking_defines.h"

extern replacement_data_t gbl_replacement_data;
extern dyn_linking_relocation_data_t gbl_dyn_linking_data;

extern uint8_t gAppStatus;
extern uint64_t gGameTitleID;
extern volatile uint8_t gSDInitDone;


extern void * ntfs_mounts;
extern int32_t ntfs_mount_count;

extern struct buffer_store drc_store;
extern struct buffer_store tv_store;

#endif // RETAINS_VARS_H_
