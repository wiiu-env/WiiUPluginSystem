#include "retain_vars.h"

u8 gUsingLibIOSUHAX __attribute__((section(".data"))) = 0;
u8 gAppStatus __attribute__((section(".data"))) = 0;
volatile u8 gSDInitDone __attribute__((section(".data"))) = 0;

char gModFolder[FS_MAX_ENTNAME_SIZE] __attribute__((section(".data")));
//char gLastMetaPath[FS_MAX_ENTNAME_SIZE] __attribute__((section(".data")));

void * ntfs_mounts __attribute__((section(".data"))) = NULL;
int ntfs_mount_count __attribute__((section(".data"))) = 0;

vc_vector* g_dirhandles = NULL;
vc_vector* g_filehandles = NULL;
