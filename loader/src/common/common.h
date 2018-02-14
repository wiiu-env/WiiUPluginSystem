#ifndef COMMON_H
#define    COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <dynamic_libs/os_defs.h>

#define CAFE_OS_SD_PATH             "/vol/external01"
#define GAME_MOD_FOLDER             "/sdcafiine"
#define SD_PATH                     "sd:"
#define USB_PATH                    "usb:"
#define DEFAULT_NAME_PACKAGE        "DEFAULT"
#define NAME_PREFIX_SD              "sd: "
#define NAME_PREFIX_USB             "usb:"

#define ELF_DATA_ADDR               (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x00))
#define ELF_DATA_SIZE               (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x04))
#define MAIN_ENTRY_ADDR             (*(volatile unsigned int*)(MEM_BASE + 0x1400 + 0x00))

#define SDUSB_MOUNTED_NONE          0
#define SDUSB_MOUNTED_FAKE          (1<<0)
#define SDUSB_MOUNTED_OS_SD         (1<<1)
#define SDUSB_LIBIOSU_LOADED        (1<<2)
#define SD_MOUNTED_LIBFAT           (1<<3)
#define USB_MOUNTED_LIBFAT          (1<<4)
#define USB_MOUNTED_LIBNTFS         (1<<5)

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS                0
#endif
#define EXIT_HBL_EXIT               0xFFFFFFFE
#define EXIT_RELAUNCH_ON_LOAD       0xFFFFFFFD

#ifdef __cplusplus
}
#endif

#endif    /* COMMON_H */

