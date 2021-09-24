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

bool
WUPSConfigItemIntegerRange_AddToCategory(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName,
                                         int32_t defaultValue, int32_t minValue, int32_t maxValue,
                                         IntegerRangeValueChangedCallback callback);

#define WUPSConfigItemIntegerRange_AddToCategoryHandled(__config__, __cat__, __configID__, __displayName__, __defaultValue__, __minValue__, __maxValue__, __callback__) \
     do { \
        if (!WUPSConfigItemIntegerRange_AddToCategory(__cat__, __configID__, __displayName__, __defaultValue__, __minValue__, __maxValue__, __callback__)) { \
            WUPSConfig_Destroy(__config__); \
            return 0; \
        } \
     } while(0)

#ifdef __cplusplus
}
#endif