#include <wups.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemIntegerRange {
    char *configId;
    WUPSConfigItemHandle handle;
    int defaultValue;
    int value;
    int minValue;
    int maxValue;
    void *callback;
} ConfigItemIntegerRange;

typedef void (*IntegerRangeValueChangedCallback)(ConfigItemIntegerRange *, int32_t);

bool WUPSConfigItemIntegerRange_AddToCategory(WUPSConfigCategoryHandle cat, const char *configId, const char *displayName,
                                              int32_t defaultValue, int32_t minValue, int32_t maxValue,
                                              IntegerRangeValueChangedCallback callback);

#ifdef __cplusplus
}
#endif