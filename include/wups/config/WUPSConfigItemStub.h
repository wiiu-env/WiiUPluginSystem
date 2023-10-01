#include <wups.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfigItemStub {
    WUPSConfigItemHandle handle;
} ConfigItemStub;

bool WUPSConfigItemStub_AddToCategory(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName);

#ifdef __cplusplus
}
#endif