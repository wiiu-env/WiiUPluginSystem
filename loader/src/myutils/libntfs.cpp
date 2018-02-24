#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/logger.h>
#include "libntfs.h"
#include <iosuhax.h>
#include <ntfs.h>
#include "common/retain_vars.h"

int mountAllNTFS() {
    int i;
    // Mount all NTFS volumes on all inserted block devices
    ntfs_mount_count = ntfsMountAll((ntfs_md **) &ntfs_mounts, NTFS_DEFAULT | NTFS_RECOVER);
    if (ntfs_mount_count == -1) {
        DEBUG_FUNCTION_LINE("Error whilst mounting devices.\n");
    } else if (ntfs_mount_count == 0) {
        DEBUG_FUNCTION_LINE("No NTFS volumes were found and/or mounted.\n");
    } else {
        DEBUG_FUNCTION_LINE("%i NTFS volumes(s) mounted!\n", ntfs_mount_count);
    }
    // List all mounted NTFS volumes
    for (i = 0; i < ntfs_mount_count; i++) {
        DEBUG_FUNCTION_LINE("%i - %s:/ (%s)\n", i + 1, ((ntfs_md *)ntfs_mounts)[i].name, ntfsGetVolumeName(((ntfs_md *)ntfs_mounts)[i].name));
    }
    return ntfs_mount_count;
}

int unmountAllNTFS(void) {
    if (ntfs_mounts) {
        int i = 0;
        for (i = 0; i < ntfs_mount_count; i++) {
            ntfsUnmount(((ntfs_md *)ntfs_mounts)[i].name, true);
        }
        free(ntfs_mounts);
        ntfs_mounts = NULL;
        ntfs_mount_count = 0;
    }
    return 0;
}
