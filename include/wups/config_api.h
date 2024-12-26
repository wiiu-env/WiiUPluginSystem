#pragma once

#include "config.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef WUPSConfigAPICallbackStatus (*WUPSConfigAPI_MenuOpenedCallback)(WUPSConfigCategoryHandle root);
typedef void (*WUPSConfigAPI_MenuClosedCallback)();

/**
 * @brief Initialize the WUPSConfigAPI with extended options. For internal use.
 * @see WUPSConfigAPI_Init instead
 */
WUPSConfigAPIStatus WUPSConfigAPI_InitEx(uint32_t pluginIdentifier, WUPSConfigAPIOptions, WUPSConfigAPI_MenuOpenedCallback, WUPSConfigAPI_MenuClosedCallback);

/**
 * @brief Initializes the WUPSConfigAPI with the given options, opened callback, and closed callback.
 *
 * This function initializes the WUPSConfigAPI with the provided options, opened callback, and closed callback.
 * If the initialization is successful, it returns WUPSCONFIG_API_RESULT_SUCCESS. Otherwise, it returns
 * an appropriate error code indicating the reason for failure.
 *
 * @param optionsV1 The options for the WUPSConfigAPI. It contains the name of the plugin.
 * @param openedCallback The callback function to be called when the menu is opened.
 * @param closedCallback The callback function to be called when the menu is closed.
 *
 * @return WUPSConfigAPIStatus WUPSCONFIG_API_RESULT_SUCCESS if initialization is successful,
 *         otherwise an appropriate error code.
 *         - WUPSCONFIG_API_RESULT_SUCCESS: The initialization was successful.
 *         - WUPSCONFIG_API_RESULT_INVALID_ARGUMENT: The `openedCallback` or `closedCallback` parameter is nullptr.
 *         - WUPSCONFIG_API_RESULT_UNSUPPORTED_VERSION: The specified `options.version` is not supported.
 *         - WUPSCONFIG_API_RESULT_NOT_FOUND: The plugin with the given identifier was not found
 *         - WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED: The API has not been initialized.
 *         - WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND: The command is not supported.
 */
WUPSConfigAPIStatus WUPSConfigAPI_Init(WUPSConfigAPIOptionsV1 optionsV1, WUPSConfigAPI_MenuOpenedCallback openedCallback, WUPSConfigAPI_MenuClosedCallback closedCallback);

/**
 * @brief Retrieves the version of the WUPSConfigAPI library.
 *
 * This function retrieves the version of the WUPSConfigAPI library and stores it in the provided output variable.
 *
 * @param[out] outVariable Pointer to a WUPSConfigAPIVersion variable to store the library version.
 * @return Returns the status of the API call.
 *          - WUPSCONFIG_API_RESULT_SUCCESS: The version was retrieved successfully.
 *          - WUPSCONFIG_API_RESULT_MODULE_NOT_FOUND: The module containing the WUPSConfigAPI_GetVersion function was not found.
 *          - WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT: The WUPSConfigAPI_GetVersion function was not found in the module.
 */
WUPSConfigAPIStatus WUPSConfigAPI_GetVersion(WUPSConfigAPIVersion *outVariable);

/**
 * @brief Create a new category with extended options. Internal use.
 * @see WUPSConfigAPI_Category_Create
 **/
WUPSConfigAPIStatus WUPSConfigAPI_Category_CreateEx(WUPSConfigAPICreateCategoryOptions options, WUPSConfigCategoryHandle *out);

/**
 * @brief Create a new category.
 *
 * This function creates a new category using the given options. The options struct contains the category name.
 * If the category creation is successful, the category handle is returned through the *out parameter.
 *
 * @param options The options for creating the category.
 * @param[out] out A pointer to a WUPSConfigCategoryHandle variable that will receive the category handle.
 * @return WUPSConfigAPIStatus The status of the category creation operation.
 *         - WUPSCONFIG_API_RESULT_SUCCESS: Success.
 *         - WUPSCONFIG_API_RESULT_INVALID_ARGUMENT: Invalid parameter, `out` or `name` is NULL.
 *         - WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED: The API has not been initialized.
 *         - WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND: The command is not supported.
 *
 * @note The function internally calls WUPSConfigAPI_Category_CreateEx().
 * @note The caller is responsible for deleting the WUPSConfigCategoryHandle instance unless it has been transferred to
 * a different category
 */
static inline WUPSConfigAPIStatus WUPSConfigAPI_Category_Create(WUPSConfigAPICreateCategoryOptionsV1 options, WUPSConfigCategoryHandle *out) {
    const WUPSConfigAPICreateCategoryOptions optionsWrapper = {
            .version = WUPS_API_CATEGORY_OPTION_VERSION_V1,
            .data    = {.v1 = options},
    };
    return WUPSConfigAPI_Category_CreateEx(optionsWrapper, out);
}

/**
 * @brief Destroys a WUPSConfigCategoryHandle.
 *
 * This function is used to destroy a WUPSConfigCategoryHandle object.
 *
 * @param handle The WUPSConfigCategoryHandle to be destroyed.
 * @return WUPSConfigAPIStatus Returns the status of the API call:
 *         - WUPSCONFIG_API_RESULT_SUCCESS: The WUPSConfigCategoryHandle was successfully destroyed.
 *         - WUPSCONFIG_API_RESULT_INVALID_ARGUMENT: The handle is nullptr.
 *         - WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED: The API has not been initialized.
 *         - WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND: The command is not supported.
 *
 * @note The caller is responsible for deleting the WUPSConfigCategoryHandle instance unless it has been transferred to
 * a different category
 */
WUPSConfigAPIStatus WUPSConfigAPI_Category_Destroy(WUPSConfigCategoryHandle handle);

/**
 * @brief Adds a category to the parent category.
 *
 * This function is used to add a category to an existing parent category.
 *
 * @param parentHandle The handle to the parent category.
 * @param categoryHandle The handle to the category to be added.
 *
 * @return WUPSConfigAPIStatus The status code indicating the result of the operation.
 *         Possible values are:
 *         - WUPSCONFIG_API_RESULT_SUCCESS: The category was added successfully.
 *         - WUPSCONFIG_API_RESULT_INVALID_ARGUMENT: The parentHandle or categoryHandle is null.
 *         - WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED: The library is not initialized.
 *         - WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND: The command is not supported.
 *         - WUPSCONFIG_API_RESULT_UNKNOWN_ERROR: An unknown error occurred.
 *
 * @note On success the ownership of the category will be transferred to the Category and the categoryHandle WUPSConfigCategoryHandle
 * will be invalid.
 */
WUPSConfigAPIStatus WUPSConfigAPI_Category_AddCategory(WUPSConfigCategoryHandle parentHandle, WUPSConfigCategoryHandle categoryHandle);

/**
 * @brief Adds an item to the given category.
 *
 * This function adds the specified item to the parent category. The parent
 * category and item handles must be valid and non-zero.
 *
 * @param parentHandle The handle of the parent category.
 * @param[out] itemHandle The handle of the item to be added.
 * @return WUPSConfigAPIStatus The status code indicating the result of the operation.
 *         - WUPSCONFIG_API_RESULT_SUCCESS: If the item was added successfully.
 *         - WUPSCONFIG_API_RESULT_INVALID_ARGUMENT: If either the parentHandle or itemHandle is invalid.
 *         - WUPSCONFIG_API_RESULT_NOT_FOUND: If the parent category was not found.
 *         - WUPSCONFIG_API_RESULT_UNKNOWN_ERROR: If an unknown error occurred while adding the item.
 *         - WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND: The provided command is not supported.
 *         - WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED: The library is not initialized properly.
 * @see WUPSConfigCategoryHandle
 * @see WUPSConfigItemHandle
 * @see WUPSConfigAPIStatus
 *
 * @note On success the ownership of the item will be transferred to the Category and the itemHandle WUPSConfigItemHandle
 * will be invalid.
 */
WUPSConfigAPIStatus WUPSConfigAPI_Category_AddItem(WUPSConfigCategoryHandle parentHandle, WUPSConfigItemHandle itemHandle);

/**
 * @brief Create a WUPSConfigAPI item with extended options. Internal use.
 * @see WUPSConfigAPI_Item_Create
 **/
WUPSConfigAPIStatus WUPSConfigAPI_Item_CreateEx(WUPSConfigAPICreateItemOptions options, WUPSConfigItemHandle *out);

/**
 * @brief Creates a new configuration item using the provided options.
 *
 * This function creates a new configuration item using the provided options.
 *
 * @param options The options for creating the configuration item.
 * @param[out] out The handle to the created configuration item.
 * @return The status of the API call. Possible values are:
 *         - WUPSCONFIG_API_RESULT_SUCCESS if the item was created and added successfully.
 *         - WUPSCONFIG_API_RESULT_INVALID_ARGUMENT if the 'out' parameter is nullptr.
 *         - WUPSCONFIG_API_RESULT_UNSUPPORTED_VERSION if the options version is invalid.
 *         - WUPSCONFIG_API_RESULT_OUT_OF_MEMORY if memory allocation failed.
 *         - WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND: The provided command is not supported.
 *         - WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED: The library is not initialized properly.
 *
 * @note The caller is responsible for deleting the WUPSConfigItem instance unless it has been transferred to
 * a category
 */
static inline WUPSConfigAPIStatus WUPSConfigAPI_Item_Create(WUPSConfigAPIItemOptionsV2 options, WUPSConfigItemHandle *out) {
    const WUPSConfigAPICreateItemOptions itemOptions = {
            .version = WUPS_API_ITEM_OPTION_VERSION_V2,
            .data    = {.v2 = options},
    };
    return WUPSConfigAPI_Item_CreateEx(itemOptions, out);
}

/**
 * @brief Destroy a WUPSConfigItemHandle.
 *
 * This function destroys a WUPSConfigItemHandle. It can only be called if the WUPSConfig API is initialized and the handle is valid.
 * A item must not be destroyed if it's added to a WUPSConfigCategory
 *
 * @param handle The handle to be destroyed.
 * @return WUPSConfigAPIStatus The status code indicating the result of the operation:
 *         - WUPSCONFIG_API_RESULT_SUCCESS: If the handle was successfully destroyed.
 *         - WUPSCONFIG_API_RESULT_NOT_FOUND: The WUPSConfigItem with the given handle was not found.
 *         - WUPSCONFIG_API_RESULT_INVALID_ARGUMENT: If the handle is invalid.
 *         - WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED: If the WUPSConfig API is not initialized.
 *         - WUPSCONFIG_API_RESULT_UNSUPPORTED_COMMAND: If the destroy command is not supported by the API or the API version is too low.
 */
WUPSConfigAPIStatus WUPSConfigAPI_Item_Destroy(WUPSConfigItemHandle handle);

/**
 * @brief Converts a WUPSConfigAPIStatus value to a corresponding string representation.
 *
 * This function takes a WUPSConfigAPIStatus value and returns a string representation of the value.
 * The string representation is determined based on the given status value using a switch statement.
 * If the status value is not recognized, a default string "WUPSCONFIG_API_RESULT_UNKNOWN_ERROR" is returned.
 *
 * @param status The WUPSConfigAPIStatus value to convert to string.
 * @return The string representation of the given WUPSConfigAPIStatus value.
 */
const char *WUPSConfigAPI_GetStatusStr(WUPSConfigAPIStatus status);

/**
 * @brief Checks if the WUPS config menu is open.
 *
 * Use this function if you want to change the behavior of a function replacement while
 * the user is interacting with the WUPS config menu.
 *
 * @param[out] status Pointer to a variable to write the menu status:
 *         - `WUPSCONFIG_API_MENU_STATUS_CLOSED`
 *         - `WUPSCONFIG_API_MENU_STATUS_OPENED`
 * @return WUPSConfigAPIStatus The status code indicating the result of the operation:
 *         - WUPSCONFIG_API_RESULT_SUCCESS: The result was written successfully to the `status` argument.
 *         - WUPSCONFIG_API_RESULT_INVALID_ARGUMENT: The `status` argument is a null pointer.
 *         - WUPSCONFIG_API_RESULT_LIB_UNINITIALIZED: The WUPSConfig API is not initialized.
 *         - WUPSCONFIG_API_RESULT_MODULE_MISSING_EXPORT: The function WUPSConfigAPI_GetMenuOpen was not found in the module.
 */
WUPSConfigAPIStatus WUPSConfigAPI_Menu_GetStatus(WUPSConfigAPIMenuStatus *status);

#ifdef __cplusplus
}
#endif
