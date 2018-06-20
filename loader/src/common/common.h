#ifndef COMMON_H
#define    COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <dynamic_libs/os_defs.h>

#define CAFE_OS_SD_PATH             "/vol/external01"
#define WIIU_PATH                   "/wiiu"
#define SD_PATH                     "sd:"
#define USB_PATH                    "usb:"
#define DEFAULT_NAME_PACKAGE        "DEFAULT"
#define NAME_PREFIX_SD              "sd: "
#define NAME_PREFIX_USB             "usb:"

#define ELF_DATA_ADDR               (*(volatile uint32_t*)(MEM_BASE + 0x1300 + 0x00))
#define ELF_DATA_SIZE               (*(volatile uint32_t*)(MEM_BASE + 0x1300 + 0x04))
#define MAIN_ENTRY_ADDR             (*(volatile uint32_t*)(MEM_BASE + 0x1400 + 0x00))

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS                0
#endif
#define EXIT_HBL_EXIT               0xFFFFFFFE
#define EXIT_RELAUNCH_ON_LOAD       0xFFFFFFFD

#define DEFAULT_WUPSLOADER_PATH     SD_PATH WIIU_PATH "/apps/wiiupluginsystem"
#define DEFAULT_LANG_PATH           DEFAULT_WUPSLOADER_PATH "/languages"
#define LANGUAGE_FILE_EXT           ".lang"

#define WUPS_TEMP_PLUGIN_PATH       SD_PATH WIIU_PATH "/plugins/temp"
#define WUPS_TEMP_PLUGIN_FILE       WUPS_TEMP_PLUGIN_PATH "/temp.mod"

#define WUPS_SDUSB_MOUNTED_NONE     0
#define WUPS_SDUSB_MOUNTED_FAKE          (1<<0)
#define WUPS_SDUSB_MOUNTED_OS_SD         (1<<1)
#define WUPS_SDUSB_LIBIOSU_LOADED        (1<<2)
#define WUPS_SD_MOUNTED_LIBFAT           (1<<3)
#define WUPS_USB_MOUNTED_LIBFAT          (1<<4)
#define WUPS_USB_MOUNTED_LIBNTFS         (1<<5)

#define WUPS_SD_MOUNTED                  (WUPS_SDUSB_MOUNTED_OS_SD | WUPS_SD_MOUNTED_LIBFAT)
#define WUPS_USB_MOUNTED                 (WUPS_USB_MOUNTED_LIBFAT)

#ifdef __cplusplus
}
#endif

#endif    /* COMMON_H */

