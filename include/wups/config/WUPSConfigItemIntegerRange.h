#include <wups.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemIntegerRange {
    WUPSConfigItemHandle handle;
    int defaultValue;
    int value;
    int minValue;
    int maxValue;
    void *callback;
} ConfigItemIntegerRange;

typedef void (*IntegerRangeValueChangedCallback)(ConfigItemIntegerRange *, int32_t);

WUPSConfigItemHandle WUPSConfigItemIntegerRange_Create(const char *configID, const char *displayName, int32_t defaultValue, int32_t minValue, int32_t maxValue, IntegerRangeValueChangedCallback callback);

void WUPSConfigItemIntegerRange_CleanUp();

#ifdef __cplusplus
}
#endif