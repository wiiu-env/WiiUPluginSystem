#pragma once

#ifdef DEBUG
#include <coreinit/debug.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern const char wups_meta_info_dump[];
#ifdef __cplusplus
}
#endif

#ifdef DEBUG
#define WUPS_DEBUG_REPORT(fmt, ...) OSReport("[%s] " fmt, wups_meta_info_dump, ##__VA_ARGS__)
#else
#define WUPS_DEBUG_REPORT(fmt, ...)
#endif

#define WUPS_DEBUG_WARN(fmt, ...) OSReport("\033[33m[%s] " fmt "\033[0m", wups_meta_info_dump, ##__VA_ARGS__)