#include "retain_vars.h"

u8 gAppStatus __attribute__((section(".data"))) = 0;

volatile bool gSDInitDone __attribute__((section(".data"))) = false;
volatile bool gUSBInitDone __attribute__((section(".data"))) = false;

char gModFolder[FS_MAX_ENTNAME_SIZE] __attribute__((section(".data")));
//char gLastMetaPath[FS_MAX_ENTNAME_SIZE] __attribute__((section(".data")));
