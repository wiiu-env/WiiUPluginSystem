#include <wups.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemBoolean {
    WUPSConfigItemHandle handle;
    bool defaultValue;
    bool value;
    char trueValue[32];
    char falseValue[32];
    void *callback;
} ConfigItemBoolean;

typedef void (*BooleanValueChangedCallback)(ConfigItemBoolean *, bool);

bool WUPSConfigItemBoolean_AddToCategory(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName, bool defaultValue, BooleanValueChangedCallback callback);
bool WUPSConfigItemBoolean_AddToCategoryEx(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName, bool defaultValue, BooleanValueChangedCallback callback, const char *trueValue,
                                           const char *falseValue);

#define WUPSConfigItemBoolean_AddToCategoryHandled(__config__, __cat__, __configID__, __displayName__, __defaultValue__, __callback__) \
    do {                                                                                                                               \
        if (!WUPSConfigItemBoolean_AddToCategory(__cat__, __configID__, __displayName__, __defaultValue__, __callback__)) {            \
            WUPSConfig_Destroy(__config__);                                                                                            \
            return 0;                                                                                                                  \
        }                                                                                                                              \
    } while (0)

#define WUPSConfigItemBoolean_AddToCategoryHandledEx(__config__, __cat__, __configID__, __displayName__, __defaultValue__, __callback__, __trueValue__, __falseValue__) \
    do {                                                                                                                                                                \
        if (!WUPSConfigItemBoolean_AddToCategoryEx(__cat__, __configID__, __displayName__, __defaultValue__, __callback__, __trueValue__, __falseValue__)) {            \
            WUPSConfig_Destroy(__config__);                                                                                                                             \
            return 0;                                                                                                                                                   \
        }                                                                                                                                                               \
    } while (0)

#ifdef __cplusplus
}
#endif