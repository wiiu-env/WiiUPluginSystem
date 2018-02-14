#include "retain_vars.h"

u8 gAppStatus __attribute__((section(".data"))) = 0;

volatile u8 gSDInitDone __attribute__((section(".data"))) = 0;

char gModFolder[FS_MAX_ENTNAME_SIZE] __attribute__((section(".data")));
//char gLastMetaPath[FS_MAX_ENTNAME_SIZE] __attribute__((section(".data")));
