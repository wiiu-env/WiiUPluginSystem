#include <utils/logger.h>
#include "libfat.h"
#include <iosuhax.h>
#include <fat.h>
#include "common/retain_vars.h"

#define LIBFAT_WIIU_DEFAULT_CACHE_PAGES 4
#define LIBFAT_WIIU_DEFAULT_SECTORS_PAGE 64


int32_t mount_libfatAll() {
    int32_t res = -1;
    if((res = fatInitEx (LIBFAT_WIIU_DEFAULT_CACHE_PAGES, true, LIBFAT_WIIU_DEFAULT_SECTORS_PAGE)) >= 0) {
        DEBUG_FUNCTION_LINE("fatInitDefault success\n");
        return 0;
    } else {
        DEBUG_FUNCTION_LINE("fatInitDefault failed %d\n",res);
    }
    return -1;
}

void unmount_libfat(const char * path) {
    fatUnmount(path);
}
