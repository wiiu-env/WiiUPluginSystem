#ifndef __SDCAFIINE_FS_UTILS_H_
#define __SDCAFIINE_FS_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <dynamic_libs/os_types.h>

int is_gamefile(const char *path);
char * getPathWithNewBase(const char * inPath, const char * newBase);
char * getRelativePath(const char *path);

#ifdef __cplusplus
}
#endif

#endif // __FS_UTILS_H_
