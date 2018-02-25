#ifndef RETAINS_VARS_H_
#define RETAINS_VARS_H_
#include <dynamic_libs/fs_defs.h>
#include <dynamic_libs/os_types.h>

#define ASYNC_RESULT_CACHE_SIZE     50
#define FS_QUEUE_MESSAGE_COUNT      5

extern u8 gAppStatus;
extern volatile bool gSDInitDone;
extern volatile bool gUSBInitDone;

extern char gModFolder[FS_MAX_ENTNAME_SIZE];

#endif // RETAINS_VARS_H_
