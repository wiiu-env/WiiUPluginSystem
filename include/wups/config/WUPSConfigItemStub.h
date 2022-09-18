#include <wups.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemStub {
    WUPSConfigItemHandle handle;
} ConfigItemStub;

bool WUPSConfigItemStub_AddToCategory(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName);

#define WUPSConfigItemStub_AddToCategoryHandled(__config__, __cat__, __configID__, __displayName__) \
    do {                                                                                            \
        if (!WUPSConfigItemStub_AddToCategory(__cat__, __configID__, __displayName__)) {            \
            WUPSConfig_Destroy(__config__);                                                         \
            return 0;                                                                               \
        }                                                                                           \
    } while (0)

#ifdef __cplusplus
}
#endif