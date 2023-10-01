#include <stdbool.h>
#include <stdint.h>
#include <wups/config.h>

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

#ifdef __cplusplus
}
#endif