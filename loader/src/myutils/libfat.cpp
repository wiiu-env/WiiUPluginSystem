#include <utils/logger.h>
#include "libfat.h"
#include <iosuhax.h>
#include <fat.h>
#include "common/retain_vars.h"

int mount_libfatAll(){
    int res = -1;
    if((res = fatInitDefault()) >= 0){
        DEBUG_FUNCTION_LINE("fatInitDefault success\n");
        return 0;
    }else{
        DEBUG_FUNCTION_LINE("fatInitDefault failed %d\n",res);
    }
    return -1;
}

void unmount_libfat(const char * path){
    fatUnmount(path);
}
