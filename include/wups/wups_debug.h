#pragma once

#ifdef DEBUG
#include <coreinit/debug.h>
#endif

#ifdef DEBUG
#define WUPS_DEBUG_REPORT(fmt, ...) OSReport(fmt, ##__VA_ARGS__)
#else
#define WUPS_DEBUG_REPORT(fmt, ...)
#endif