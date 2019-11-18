#ifndef __LOGGER_H_
#define __LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

void log_init_();
//void log_deinit_(void);
void log_print_(const char *str);
void log_printf_(const char *format, ...);
void OSFatal_printf(const char *format, ...);

#define __FILENAME_X__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILENAME_X__)

#define OSFATAL_FUNCTION_LINE(FMT, ARGS...)do { \
    OSFatal_printf("[%s]%s@L%04d: " FMT "",__FILENAME__,__FUNCTION__, __LINE__, ## ARGS); \
    } while (0)

#define log_init() log_init_()
//#define log_deinit() log_deinit_()
#define log_print(str) log_print_(str)
#define log_printf(FMT, ARGS...)  log_printf_(FMT, ## ARGS);

#define DEBUG_FUNCTION_LINE(FMT, ARGS...)do { \
    log_printf("[%23s]%30s@L%04d: " FMT "",__FILENAME__,__FUNCTION__, __LINE__, ## ARGS); \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif
