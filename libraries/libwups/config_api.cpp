#include "wups/config_api.h"
#include "wups/wups_debug.h"
#include <coreinit/dynload.h>
#include <wups/config.h>

static OSDynLoad_Module sModuleHandle = nullptr;

static WUPSConfigAPIStatus (*sAPIGetVersion)(WUPSConfigAPIVersion *out)                                                                                                                             = nullptr;
static WUPSConfigAPIStatus (*sAPIInitEx)(uint32_t pluginIdentifier, WUPSConfigAPIOptions options, WUPSConfigAPI_MenuOpenedCallback openedCallback, WUPSConfigAPI_MenuClosedCallback closedCallback) = nullptr;
static WUPSConfigAPIStatus (*sAPICategoryCreateEx)(WUPSConfigAPICreateCategoryOptions options, WUPSConfigCategoryHandle *out)                                                                       = nullptr;
static WUPSConfigAPIStatus (*sAPICategoryDestroy)(WUPSConfigCategoryHandle handle)                                                                                                                  = nullptr;
static WUPSConfigAPIStatus (*sAPICategoryAddCategory)(WUPSConfigCategoryHandle parentHandle, WUPSConfigCategoryHandle categoryHandle)                                                               = nullptr;
static WUPSConfigAPIStatus (*sAPICategoryAddItem)(WUPSConfigCategoryHandle parentHandle, WUPSConfigItemHandle itemHandle)                                                                           = nullptr;
static WUPSConfigAPIStatus (*sAPIItemCreateEx)(WUPSConfigAPICreateItemOptions options, WUPSConfigItemHandle *out)                                                                                   = nullptr;
static WUPSConfigAPIStatus (*sAPIItemDestroy)(WUPSConfigItemHandle handle)                                                                                                                          = nullptr;
static WUPSConfigAPIStatus (*sAPIMenuGetStatus)(WUPSConfigAPIMenuStatus *status)                                                                                                                    = nullptr;

static WUPSConfigAPIVersion sConfigAPIVersion = WUPS_CONFIG_API_VERSION_ERROR;

static bool sConfigLibInitDone          = false;
static uint32_t sConfigPluginIdentifier = 0xFFFFFFFF;

const char *WUPSConfigAPI_GetStatusStr(WUPSConfigAPIStatus status) {
    switch (status) {
        case WUPSCONFIG_API_RESULT_SUCCESS:
            return "WUPSCONFIG_API_RESULT_SUCCESS";
        case WUPSCONFIG_API_RESULT_INVALID_ARGUMENT:
            return "WUPSCONFIG_API_RESULT_INVALID_ARGUMENT";
        case WUPSCONFIG_API_RESULT_OUT_OF_MEMORY:
            return "WUPSCONFIG_API_RESULT_OUT_OF_MEMORY";
        case WUPSCONFIG_API_RESULT_NOT_FOUND:
            return "WUPSCONFIG_API_RESULT_NOT_FOUND";
        case WUPSCONFIG_API_RESULT_MODULE_NOT_FOUND:
            return "WUPSCONFIG_API_RESULT_MODULE_NOT_FOUND";
        case WUPSCONFIG_API_RESULT_INVALID_PLUGIN_IDENTIFIER:
            return "WUPSCONFIG_API_RESULT_INVALID_PLUGIN_IDENTIFIER";
        case WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT:
            return "WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT";
        case WUPSCONFIG_API_RESULT_UNSUPPORTED_VERSION:
            return "WUPSCONFIG_API_RESULT_UNSUPPORTED_VERSION";
        case WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND:
            return "WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND";
        case WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED:
            return "WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED";
        case WUPSCONFIG_API_RESULT_UNKNOWN_ERROR:
            return "WUPSCONFIG_API_RESULT_UNKNOWN_ERROR";
        case WUPSCONFIG_API_RESULT_MISSING_CALLBACK:
            return "WUPSCONFIG_API_RESULT_MISSING_CALLBACK";
    }
    return "WUPSCONFIG_API_RESULT_UNKNOWN_ERROR";
}

extern "C" WUPSConfigAPIStatus WUPSConfigAPI_InitLibrary_Internal(wups_loader_init_config_args_t args) {
    if (sConfigLibInitDone) {
        return WUPSCONFIG_API_RESULT_SUCCESS;
    }
    if (args.arg_version != 1) {
        return WUPSCONFIG_API_RESULT_UNSUPPORTED_VERSION;
    }

    if (OSDynLoad_Acquire("homebrew_wupsbackend", &sModuleHandle) != OS_DYNLOAD_OK) {
        WUPS_DEBUG_REPORT("libuwps: OSDynLoad_Acquire \"homebrew_wupsbackend\" failed.\n");
        return WUPSCONFIG_API_RESULT_MODULE_NOT_FOUND;
    }

    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "WUPSConfigAPI_GetVersion", (void **) &sAPIGetVersion) != OS_DYNLOAD_OK) {
        WUPS_DEBUG_REPORT("libuwps: FindExport WUPSConfigAPI_GetVersion failed.\n");
        return WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT;
    }
    auto res = WUPSConfigAPI_GetVersion(&sConfigAPIVersion);
    if (res != WUPSCONFIG_API_RESULT_SUCCESS) {
        return WUPSCONFIG_API_RESULT_UNSUPPORTED_VERSION;
    }

    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "WUPSConfigAPI_InitEx", (void **) &sAPIInitEx) != OS_DYNLOAD_OK) {
        WUPS_DEBUG_REPORT("libwups: FindExport WUPSConfigAPI_InitEx failed.\n");
        return WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT;
    }
    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "WUPSConfigAPI_Category_CreateEx", (void **) &sAPICategoryCreateEx) != OS_DYNLOAD_OK) {
        WUPS_DEBUG_REPORT("libwups: FindExport WUPSConfigAPI_Category_CreateEx failed.\n");
        return WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT;
    }

    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "WUPSConfigAPI_Category_Destroy", (void **) &sAPICategoryDestroy) != OS_DYNLOAD_OK) {
        WUPS_DEBUG_REPORT("libwups: FindExport WUPSConfigAPI_Category_Destroy failed.\n");
        return WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT;
    }
    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "WUPSConfigAPI_Category_AddCategory", (void **) &sAPICategoryAddCategory) != OS_DYNLOAD_OK) {
        WUPS_DEBUG_REPORT("libwups: FindExport WUPSConfigAPI_Category_AddCategory failed.\n");
        return WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT;
    }
    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "WUPSConfigAPI_Category_AddItem", (void **) &sAPICategoryAddItem) != OS_DYNLOAD_OK) {
        WUPS_DEBUG_REPORT("libwups: FindExport WUPSConfigAPI_Category_AddItem failed.\n");
        return WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT;
    }

    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "WUPSConfigAPI_Item_CreateEx", (void **) &sAPIItemCreateEx) != OS_DYNLOAD_OK) {
        WUPS_DEBUG_REPORT("libwups: FindExport WUPSConfigAPI_Item_Create failed.\n");
        return WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT;
    }
    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "WUPSConfigAPI_Item_Destroy", (void **) &sAPIItemDestroy) != OS_DYNLOAD_OK) {
        WUPS_DEBUG_REPORT("libwups: FindExport WUPSConfigAPI_Item_Destroy failed.\n");
        return WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT;
    }
    // This one is allowed to fail.
    OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "WUPSConfigAPI_Menu_GetStatus", (void **) &sAPIMenuGetStatus);

    sConfigLibInitDone      = true;
    sConfigPluginIdentifier = args.plugin_identifier;

    return WUPSCONFIG_API_RESULT_SUCCESS;
}

extern "C" WUPSConfigAPIStatus WUPSConfigAPI_DeInitLibrary_Internal() {
    return WUPSCONFIG_API_RESULT_SUCCESS;
}

WUPSConfigAPIStatus WUPSConfigAPI_GetVersion(WUPSConfigAPIVersion *outVariable) {
    if (sAPIGetVersion == nullptr) {
        if (OSDynLoad_Acquire("homebrew_notifications", &sModuleHandle) != OS_DYNLOAD_OK) {
            WUPS_DEBUG_REPORT("libwups: OSDynLoad_Acquire failed.\n");
            return WUPSCONFIG_API_RESULT_MODULE_NOT_FOUND;
        }

        if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "WUPSConfigAPI_GetVersion", (void **) &sAPIGetVersion) != OS_DYNLOAD_OK) {
            WUPS_DEBUG_REPORT("libwups: FindExport WUPSConfigAPI_GetVersion failed.\n");
            return WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT;
        }
    }

    return sAPIGetVersion(outVariable);
}

WUPSConfigAPIStatus WUPSConfigAPI_InitEx(uint32_t pluginIdentifier, WUPSConfigAPIOptions options, WUPSConfigAPI_MenuOpenedCallback openedCallback, WUPSConfigAPI_MenuClosedCallback closedCallback) {
    if (sConfigAPIVersion == WUPS_CONFIG_API_VERSION_ERROR) {
        return WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED;
    }
    if (sAPIInitEx == nullptr || sConfigAPIVersion < 1) {
        return WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND;
    }

    if (openedCallback == nullptr || closedCallback == nullptr || options.version != 1) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }

    return sAPIInitEx(pluginIdentifier, options, openedCallback, closedCallback);
}

WUPSConfigAPIStatus WUPSConfigAPI_Init(WUPSConfigAPIOptionsV1 optionsV1, WUPSConfigAPI_MenuOpenedCallback openedCallback, WUPSConfigAPI_MenuClosedCallback closedCallback) {
    if (sConfigPluginIdentifier == 0xFFFFFFFF) {
        return WUPSCONFIG_API_RESULT_INVALID_PLUGIN_IDENTIFIER;
    }
    WUPSConfigAPIOptions options = {
            .version = WUPS_API_CONFIG_API_OPTION_VERSION_V1,
            .data    = {.v1 = optionsV1},
    };
    return WUPSConfigAPI_InitEx(sConfigPluginIdentifier, options, openedCallback, closedCallback);
}

WUPSConfigAPIStatus WUPSConfigAPI_Category_CreateEx(WUPSConfigAPICreateCategoryOptions options, WUPSConfigCategoryHandle *out) {
    if (sConfigAPIVersion == WUPS_CONFIG_API_VERSION_ERROR) {
        return WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED;
    }
    if (sAPICategoryCreateEx == nullptr || sConfigAPIVersion < 1) {
        return WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND;
    }

    if (out == nullptr) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }

    return sAPICategoryCreateEx(options, out);
}

WUPSConfigAPIStatus WUPSConfigAPI_Category_Destroy(WUPSConfigCategoryHandle handle) {
    if (sConfigAPIVersion == WUPS_CONFIG_API_VERSION_ERROR) {
        return WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED;
    }
    if (sAPICategoryDestroy == nullptr || sConfigAPIVersion < 1) {
        return WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND;
    }

    if (handle == nullptr) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }

    return sAPICategoryDestroy(handle);
}

WUPSConfigAPIStatus WUPSConfigAPI_Category_AddCategory(WUPSConfigCategoryHandle parentHandle, WUPSConfigCategoryHandle categoryHandle) {
    if (sConfigAPIVersion == WUPS_CONFIG_API_VERSION_ERROR) {
        return WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED;
    }
    if (sAPICategoryAddCategory == nullptr || sConfigAPIVersion < 1) {
        return WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND;
    }

    if (parentHandle == nullptr || categoryHandle == nullptr) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }

    return sAPICategoryAddCategory(parentHandle, categoryHandle);
}

WUPSConfigAPIStatus WUPSConfigAPI_Category_AddItem(WUPSConfigCategoryHandle parentHandle, WUPSConfigItemHandle itemHandle) {
    if (sConfigAPIVersion == WUPS_CONFIG_API_VERSION_ERROR) {
        return WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED;
    }
    if (sAPICategoryAddItem == nullptr || sConfigAPIVersion < 1) {
        return WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND;
    }

    if (parentHandle == 0 || itemHandle == 0) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }

    return sAPICategoryAddItem(parentHandle, itemHandle);
}

WUPSConfigAPIStatus WUPSConfigAPI_Item_CreateEx(WUPSConfigAPICreateItemOptions options, WUPSConfigItemHandle *out) {
    if (sConfigAPIVersion == WUPS_CONFIG_API_VERSION_ERROR) {
        return WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED;
    }
    if (sAPIItemCreateEx == nullptr || sConfigAPIVersion < 1) {
        return WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND;
    }

    if (out == nullptr) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }

    return sAPIItemCreateEx(options, out);
}

WUPSConfigAPIStatus WUPSConfigAPI_Item_Destroy(WUPSConfigItemHandle handle) {
    if (sConfigAPIVersion == WUPS_CONFIG_API_VERSION_ERROR) {
        return WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED;
    }
    if (sAPIItemDestroy == nullptr || sConfigAPIVersion < 1) {
        return WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND;
    }

    if (handle == nullptr) {
        return WUPSCONFIG_API_RESULT_INVALID_ARGUMENT;
    }

    return sAPIItemDestroy(handle);
}

WUPSConfigAPIStatus WUPSConfigAPI_Menu_GetStatus(WUPSConfigAPIMenuStatus *out) {
    if (sConfigAPIVersion == WUPS_CONFIG_API_VERSION_ERROR) {
        return WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED;
    }
    if (sAPIMenuGetStatus == nullptr) {
        return WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT;
    }
    return sAPIMenuGetStatus(out);
}
