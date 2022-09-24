#include <wups.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemMultipleValuesPair {
    uint32_t value;
    char *valueName;
} ConfigItemMultipleValuesPair;

typedef struct ConfigItemMultipleValues {
    char *configId;
    WUPSConfigItemHandle handle;
    int32_t defaultValueIndex;
    int32_t valueIndex;
    void *callback;
    ConfigItemMultipleValuesPair *values;
    int valueCount;
} ConfigItemMultipleValues;

typedef void (*MultipleValuesChangedCallback)(ConfigItemMultipleValues *, uint32_t);

bool WUPSConfigItemMultipleValues_AddToCategory(WUPSConfigCategoryHandle cat, const char *configId, const char *displayName, int defaultValueIndex, ConfigItemMultipleValuesPair *possibleValues,
                                                int pairCount, MultipleValuesChangedCallback callback);

#define WUPSConfigItemMultipleValues_AddToCategoryHandled(__config__, __cat__, __configID__, __displayName__, __defaultValueIndex__, __possibleValues__, __pairCount__, __callback__) \
    do {                                                                                                                                                                              \
        if (!WUPSConfigItemMultipleValues_AddToCategory(__cat__, __configID__, __displayName__, __defaultValueIndex__, __possibleValues__, __pairCount__, __callback__)) {            \
            WUPSConfig_Destroy(__config__);                                                                                                                                           \
            return 0;                                                                                                                                                                 \
        }                                                                                                                                                                             \
    } while (0)

#ifdef __cplusplus
}
#endif