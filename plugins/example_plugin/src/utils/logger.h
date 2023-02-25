#pragma once

#include <coreinit/debug.h>
#include <string.h>
#include <whb/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_APP_TYPE "P"
#define LOG_APP_NAME "ExamplePlugin"

#define __FILENAME__ ({                                \
    const char *__filename = __FILE__;                 \
    const char *__pos      = strrchr(__filename, '/'); \
    if (!__pos) __pos = strrchr(__filename, '\\');     \
    __pos ? __pos + 1 : __filename;                    \
})

#define LOG(LOG_FUNC, FMT, ARGS...)                                 LOG_EX_DEFAULT(LOG_FUNC, "", "", FMT, ##ARGS)

#define LOG_EX_DEFAULT(LOG_FUNC, LOG_LEVEL, LINE_END, FMT, ARGS...) LOG_EX(__FILENAME__, __FUNCTION__, __LINE__, LOG_FUNC, LOG_LEVEL, LINE_END, FMT, ##ARGS)

#define LOG_EX(FILENAME, FUNCTION, LINE, LOG_FUNC, LOG_LEVEL, LINE_END, FMT, ARGS...)                                                        \
    do {                                                                                                                                     \
        LOG_FUNC("[(%s)%18s][%23s]%30s@L%04d: " LOG_LEVEL "" FMT "" LINE_END, LOG_APP_TYPE, LOG_APP_NAME, FILENAME, FUNCTION, LINE, ##ARGS); \
    } while (0)

#ifdef DEBUG

#ifdef VERBOSE_DEBUG
#define DEBUG_FUNCTION_LINE_VERBOSE(FMT, ARGS...)                              LOG(WHBLogPrintf, FMT, ##ARGS)
#define DEBUG_FUNCTION_LINE_VERBOSE_EX(FILENAME, FUNCTION, LINE, FMT, ARGS...) LOG_EX(FILENAME, FUNCTION, LINE, WHBLogPrintf, "", "", FMT, ##ARGS);
#else
#define DEBUG_FUNCTION_LINE_VERBOSE(FMT, ARGS...)    while (0)
#define DEBUG_FUNCTION_LINE_VERBOSE_EX(FMT, ARGS...) while (0)
#endif

#define DEBUG_FUNCTION_LINE(FMT, ARGS...)                                      LOG(WHBLogPrintf, FMT, ##ARGS)

#define DEBUG_FUNCTION_LINE_WRITE(FMT, ARGS...)                                LOG(WHBLogWritef, FMT, ##ARGS)

#define DEBUG_FUNCTION_LINE_ERR(FMT, ARGS...)                                  LOG_EX_DEFAULT(WHBLogPrintf, "##ERROR## ", "", FMT, ##ARGS)
#define DEBUG_FUNCTION_LINE_WARN(FMT, ARGS...)                                 LOG_EX_DEFAULT(WHBLogPrintf, "##WARN ## ", "", FMT, ##ARGS)
#define DEBUG_FUNCTION_LINE_INFO(FMT, ARGS...)                                 LOG_EX_DEFAULT(WHBLogPrintf, "##INFO ## ", "", FMT, ##ARGS)

#define DEBUG_FUNCTION_LINE_ERR_LAMBDA(FILENAME, FUNCTION, LINE, FMT, ARGS...) LOG_EX(FILENAME, FUNCTION, LINE, WHBLogPrintf, "##ERROR## ", "", FMT, ##ARGS);

#else

#define DEBUG_FUNCTION_LINE_VERBOSE_EX(FMT, ARGS...)                           while (0)

#define DEBUG_FUNCTION_LINE_VERBOSE(FMT, ARGS...)                              while (0)

#define DEBUG_FUNCTION_LINE(FMT, ARGS...)                                      while (0)

#define DEBUG_FUNCTION_LINE_WRITE(FMT, ARGS...)                                while (0)

#define DEBUG_FUNCTION_LINE_ERR(FMT, ARGS...)                                  LOG_EX_DEFAULT(OSReport, "##ERROR## ", "\n", FMT, ##ARGS)
#define DEBUG_FUNCTION_LINE_WARN(FMT, ARGS...)                                 LOG_EX_DEFAULT(OSReport, "##WARN ## ", "\n", FMT, ##ARGS)
#define DEBUG_FUNCTION_LINE_INFO(FMT, ARGS...)                                 LOG_EX_DEFAULT(OSReport, "##INFO ## ", "\n", FMT, ##ARGS)

#define DEBUG_FUNCTION_LINE_ERR_LAMBDA(FILENAME, FUNCTION, LINE, FMT, ARGS...) LOG_EX(FILENAME, FUNCTION, LINE, OSReport, "##ERROR## ", "\n", FMT, ##ARGS);

#endif

void initLogging();

void deinitLogging();

#ifdef __cplusplus
}
#endif
