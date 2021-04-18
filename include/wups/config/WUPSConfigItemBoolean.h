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
    void* callback;
} ConfigItemBoolean;

typedef void (*BooleanValueChangedCallback)(ConfigItemBoolean *, bool);

WUPSConfigItemHandle WUPSConfigItemBoolean_Create(const char *configID, const char *displayName, bool defaultValue, BooleanValueChangedCallback callback);
WUPSConfigItemHandle WUPSConfigItemBoolean_CreateEx(const char *configID, const char *displayName, bool defaultValue, BooleanValueChangedCallback callback, const char *trueValue, const char *falseValue);

void WUPSConfigItemBoolean_CleanUp();

#ifdef __cplusplus
}
#endif