#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file wups_storage_api.h
 * @brief API for handling persistent storage in the WUPS library.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/**
 * @enum WUPSStorageError
 * @brief Represents error codes returned by storage API functions.
 */
typedef enum {
    WUPS_STORAGE_ERROR_SUCCESS                  = 0,     /**< Success. */
    WUPS_STORAGE_ERROR_INVALID_ARGS             = -0x01, /**< Invalid arguments passed to the function. */
    WUPS_STORAGE_ERROR_MALLOC_FAILED            = -0x02, /**< Memory allocation failed. */
    WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE     = -0x03, /**< Unexpected data type encountered. */
    WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL         = -0x04, /**< Provided buffer is too small. */
    WUPS_STORAGE_ERROR_ALREADY_EXISTS           = -0x05, /**< Item already exists. */
    WUPS_STORAGE_ERROR_IO_ERROR                 = -0x06, /**< Generic IO error during saving or loading. */
    WUPS_STORAGE_ERROR_NOT_FOUND                = -0x10, /**< Item not found. */
    WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED = -0xF0, /**< Library not initialized properly. */
    WUPS_STORAGE_ERROR_INTERNAL_INVALID_VERSION = -0xF1, /**< Invalid API version. */
    WUPS_STORAGE_ERROR_UNKNOWN_ERROR            = -0x100 /**< Unknown error. */
} WUPSStorageError;

/**
 * @enum WUPSStorageItemTypes
 * @brief Represents the types of items that can be stored in the persistent storage.
 */
typedef enum {
    WUPS_STORAGE_ITEM_S32    = 0, /**< 32-bit signed integer. */
    WUPS_STORAGE_ITEM_S64    = 1, /**< 64-bit signed integer. */
    WUPS_STORAGE_ITEM_U32    = 2, /**< 32-bit unsigned integer. */
    WUPS_STORAGE_ITEM_U64    = 3, /**< 64-bit unsigned integer. */
    WUPS_STORAGE_ITEM_STRING = 4, /**< String. */
    WUPS_STORAGE_ITEM_BINARY = 5, /**< Binary data. */
    WUPS_STORAGE_ITEM_BOOL   = 6, /**< Boolean. */
    WUPS_STORAGE_ITEM_FLOAT  = 7, /**< 32-bit floating-point number. */
    WUPS_STORAGE_ITEM_DOUBLE = 8  /**< 64-bit floating-point number. */
} WUPSStorageItemTypes;
/**
 * @typedef WUPSStorageItemType
 * @brief Type alias for the storage item type.
 */
typedef uint32_t WUPSStorageItemType;

/**
 * @typedef wups_storage_root_item
 * @brief Type alias for the root storage item.
 */
typedef void *wups_storage_root_item;

/**
 * @typedef wups_storage_item
 * @brief Type alias for a storage item.
 */
typedef void *wups_storage_item;

/**
 * @typedef WUPSStorage_SaveFunction
 * @brief Type alias for the function pointer to save storage data. For internal usage only.
 */
typedef WUPSStorageError (*WUPSStorage_SaveFunction)(wups_storage_root_item root, bool force);

/**
 * @typedef WUPSStorage_ForceReloadFunction
 * @brief Type alias for the function pointer to force a reload of storage data. For internal usage only.
 */
typedef WUPSStorageError (*WUPSStorage_ForceReloadFunction)(wups_storage_root_item root);

/**
 * @typedef WUPSStorage_WipeStorageFunction
 * @brief Type alias for the function pointer to wipe all storage data. For internal usage only.
 */
typedef WUPSStorageError (*WUPSStorage_WipeStorageFunction)(wups_storage_root_item root);

/**
 * @typedef WUPSStorage_DeleteItemFunction
 * @brief Type alias for the function pointer to delete an item from storage. For internal usage only.
 */
typedef WUPSStorageError (*WUPSStorage_DeleteItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key);

/**
 * @typedef WUPSStorage_CreateSubItemFunction
 * @brief Type alias for the function pointer to create a sub-item in storage. For internal usage only.
 */
typedef WUPSStorageError (*WUPSStorage_CreateSubItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, wups_storage_item *outItem);

/**
 * @typedef WUPSStorage_GetSubItemFunction
 * @brief Type alias for the function pointer to get a sub-item from storage. For internal usage only.
 */
typedef WUPSStorageError (*WUPSStorage_GetSubItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, wups_storage_item *outItem);

/**
 * @typedef WUPSStorage_StoreItemFunction
 * @brief Type alias for the function pointer to store an item in storage. For internal usage only.
 */
typedef WUPSStorageError (*WUPSStorage_StoreItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, WUPSStorageItemType itemType, void *data, uint32_t length);

/**
 * @typedef WUPSStorage_GetItemFunction
 * @brief Type alias for the function pointer to retrieve an item from storage.  For internal usage only.
 */
typedef WUPSStorageError (*WUPSStorage_GetItemFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, WUPSStorageItemType itemType, void *data, uint32_t maxSize, uint32_t *outSize);

/**
 * @typedef WUPSStorage_GetItemSizeFunction
 * @brief Type alias for the function pointer to get the size of an item in storage.  For internal usage only.
 */
typedef WUPSStorageError (*WUPSStorage_GetItemSizeFunction)(wups_storage_root_item root, wups_storage_item parent, const char *key, WUPSStorageItemType itemType, uint32_t *outSize);

/**
 * @typedef WUPS_STORAGE_API_VERSION
 * @brief Type alias for the API version.  For internal usage only.
 */
typedef uint32_t WUPS_STORAGE_API_VERSION;

/**
 * @def WUPS_STORAGE_CUR_API_VERSION
 * @brief Current version of the storage API.  For internal usage only.
 */
#define WUPS_STORAGE_CUR_API_VERSION 0x02

/**
 * @struct wups_loader_init_storage_args_t_
 * @brief Structure containing initialization arguments for the storage API. For internal usage only.
 */
typedef struct wups_loader_init_storage_args_t_ {
    WUPS_STORAGE_API_VERSION version;                               /**< API version. */
    wups_storage_root_item root_item;                               /**< Root storage item. */
    WUPSStorage_SaveFunction save_function_ptr;                     /**< Save function pointer. */
    WUPSStorage_ForceReloadFunction force_reload_function_ptr;      /**< Force reload function pointer. */
    WUPSStorage_WipeStorageFunction wipe_storage_function_ptr;      /**< Wipe storage function pointer. */
    WUPSStorage_DeleteItemFunction delete_item_function_ptr;        /**< Delete item function pointer. */
    WUPSStorage_CreateSubItemFunction create_sub_item_function_ptr; /**< Create sub-item function pointer. */
    WUPSStorage_GetSubItemFunction get_sub_item_function_ptr;       /**< Get sub-item function pointer. */
    WUPSStorage_StoreItemFunction store_item_function_ptr;          /**< Store item function pointer. */
    WUPSStorage_GetItemFunction get_item_function_ptr;              /**< Get item function pointer. */
    WUPSStorage_GetItemSizeFunction get_item_size_function_ptr;     /**< Get item size function pointer. */
} wups_loader_init_storage_args_t;
/* called by backend */
WUPSStorageError WUPSStorageAPI_InitInternal(wups_loader_init_storage_args_t args);

/**
 * @brief Get a string representation of the specified storage status.
 *
 * This function returns a string representation of the provided storage status.
 *
 * @param status The storage status to get the string representation for.
 * @return A pointer to a string describing the provided storage status.
 **/
const char *WUPSStorageAPI_GetStatusStr(WUPSStorageError status);

/**
 * @brief Save storage data.
 *
 * This function saves the storage data. The 'forceSave' parameter determines whether to
 * force saving even if there are no changes.
 *
 * @param forceSave If true, forces saving data even if there are no changes.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - WUPS_STORAGE_ERROR_SUCCESS: The saving was successful.
 *         - WUPS_STORAGE_ERROR_MALLOC_FAILED: Failed to allocate memory.
 *         - WUPS_STORAGE_ERROR_IO_ERROR: Generic IO-Error that happened while saving or loading the storage
 *         - WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED: The library is not initialized properly, make sure use to the WUPS_USE_STORAGE macro
 *
 */
WUPSStorageError WUPSStorageAPI_SaveStorage(bool forceSave);

/**
 * @brief Force a reload of storage data.
 *
 * This function forces a reload of the storage data.
 *
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - WUPS_STORAGE_ERROR_SUCCESS: The reloading the storage was successful.
 *         - WUPS_STORAGE_ERROR_MALLOC_FAILED: Failed to allocate memory.
 *         - WUPS_STORAGE_ERROR_IO_ERROR: Generic IO-Error that happened while saving or loading the storage
 *         - WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED: The library is not initialized properly, make sure use to the WUPS_USE_STORAGE macro
 *
 */
WUPSStorageError WUPSStorageAPI_ForceReloadStorage();

/**
 * @brief Wipe all storage data.
 *
 * This function wipes all data in the storage.
 *
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - WUPS_STORAGE_ERROR_SUCCESS: Wiping the storage was successful
 *         - WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED: The library is not initialized properly, make sure use to the WUPS_USE_STORAGE macro
 */
WUPSStorageError WUPSStorageAPI_WipeStorage();

/**
 * @brief Delete an item from storage.
 *
 * This function deletes an item or sub item specified by the parent and key from the storage.
 * Deleting a sub item will also delete all it's child items.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage
 * @param key The key of the item to be deleted.
 * @return The storage error status after attempting to delete the item.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - WUPS_STORAGE_ERROR_SUCCESS: Deleting the (sub) item was successful.
 *         - WUPS_STORAGE_ERROR_NOT_FOUND: No item with the given parent/key combination has been found.
 *         - WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED: The library is not initialized properly, make sure use to the WUPS_USE_STORAGE macro
*/
WUPSStorageError WUPSStorageAPI_DeleteItem(wups_storage_item parent, const char *key);

/**
 * @brief Creates a sub-item in storage.
 *
 * This function creates a sub-item with the specified key under the given parent item.
 * If 'parent' is NULL, the sub-item will be added to the root item of the storage.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage
 * @param key The key for the new sub-item.
 * @param outItem Pointer to store the created sub-item.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - WUPS_STORAGE_ERROR_SUCCESS: The creating the sub item was successful.
 *         - WUPS_STORAGE_ERROR_INVALID_ARGS: 'outItem' was NULL
 *         - WUPS_STORAGE_ERROR_NOT_FOUND: No parent with the given handle has been found.
 *         - WUPS_STORAGE_ERROR_MALLOC_FAILED: Failed to allocate memory.
 *         - WUPS_STORAGE_ERROR_ALREADY_EXISTS: An item with the given key already exists inside the given parent.
 *         - WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED: The library is not initialized properly, make sure use to the WUPS_USE_STORAGE macro
*/
WUPSStorageError WUPSStorageAPI_CreateSubItem(wups_storage_item parent, const char *key, wups_storage_item *outItem);

/**
 * @brief Get a sub-item from storage.
 *
 * This function retrieves a sub-item with the specified key under the given parent item.
 * If 'parent' is NULL, the sub-item will be searched in the root item of the storage.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage
 * @param key The key of the sub-item to retrieve.
 * @param outItem Pointer to store the retrieved sub-item.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - WUPS_STORAGE_ERROR_SUCCESS: The reloading the storage was successful.
 *         - WUPS_STORAGE_ERROR_INVALID_ARGS: 'outItem' was NULL
 *         - WUPS_STORAGE_ERROR_NOT_FOUND: No item with the given parent/key combination has been found.
 *         - WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED: The library is not initialized properly, make sure use to the WUPS_USE_STORAGE macro
*/
WUPSStorageError WUPSStorageAPI_GetSubItem(wups_storage_item parent, const char *key, wups_storage_item *outItem);

/**
 * @brief Store data in storage.
 *
 * This function stores data of the specified type under the given key in the provided parent item.
 * - If a **item** for the given parent exists, this function will overwrite it.
 * - If a **sub item** for the given parent exists, this function will fail.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage
 * @param key The key under which to store the data.
 * @param type The type of the data to store.
 * @param data A pointer to the data to store.
 * @param size The size of the data to store.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - WUPS_STORAGE_ERROR_SUCCESS: The creating the item was successful.
 *         - WUPS_STORAGE_ERROR_INVALID_ARGS: 'data' was NULL or 'size' was unexpected
 *         - WUPS_STORAGE_ERROR_NOT_FOUND: No parent with the given handle has been found.
 *         - WUPS_STORAGE_ERROR_MALLOC_FAILED: Failed to allocate memory.
 *         - WUPS_STORAGE_ERROR_ALREADY_EXISTS: An sub item with the given key already exists inside the given parent.
 *         - WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED: The library is not initialized properly, make sure use to the WUPS_USE_STORAGE macro
*/
WUPSStorageError WUPSStorageAPI_StoreItem(wups_storage_item parent, const char *key, WUPSStorageItemType type, void *data, uint32_t size);

/**
 * @brief Store a string in storage.
 *
 * This is a convenience function to store a string in the storage.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which to store the string.
 * @param value The string value to store. Must be null-terminated. For non-nulltermianted strings use WUPSStorageAPI_StoreItem directly.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - WUPS_STORAGE_ERROR_INVALID_ARGS: 'value' was NULL.
 *         - Refer to WUPSStorageAPI_StoreItem for other possible error codes.
 * \see WUPSStorageAPI_StoreItem
 */
inline WUPSStorageError WUPSStorageAPI_StoreString(wups_storage_item parent, const char *key, const char *value) {
    if (value == NULL) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_STRING, (void *) value, strlen(value));
}

/**
 * @brief Store a boolean value in storage.
 *
 * This is a convenience function to store a boolean value in the storage.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which to store the value.
 * @param value The boolean value to store.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - Refer to WUPSStorageAPI_StoreItem for possible error codes.
* \see WUPSStorageAPI_StoreItem
*/
inline WUPSStorageError WUPSStorageAPI_StoreBool(wups_storage_item parent, const char *key, bool value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_BOOL, (void *) &value, sizeof(bool));
}

/**
* @brief Store a int value in storage.
*
* This is a convenience function to store a int value in the storage.
*
* @param parent The parent storage item. Can be NULL to refer to the root of the storage.
* @param key The key under which to store the value.
* @param value The boolean value to store.
* @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
*         - Refer to WUPSStorageAPI_StoreItem for possible error codes.
* \see WUPSStorageAPI_StoreItem
*/
inline WUPSStorageError WUPSStorageAPI_StoreInt(wups_storage_item parent, const char *key, int32_t value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_S32, (void *) &value, sizeof(int32_t));
}

/**
* @brief Store a signed 32-bit integer value in storage.
*
* This is a convenience function to store a signed 32-bit integer value in the storage.
*
* @param parent The parent storage item. Can be NULL to refer to the root of the storage.
* @param key The key under which to store the value.
* @param value The boolean value to store.
* @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
*         - Refer to WUPSStorageAPI_StoreItem for possible error codes.
* \see WUPSStorageAPI_StoreItem
*/
inline WUPSStorageError WUPSStorageAPI_StoreS32(wups_storage_item parent, const char *key, int32_t value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_S32, (void *) &value, sizeof(int32_t));
}

/**
* @brief Store a signed 64-bit integer value in storage.
*
* This is a convenience function to store a signed 64-bit integer value in the storage.
*
* @param parent The parent storage item. Can be NULL to refer to the root of the storage.
* @param key The key under which to store the value.
* @param value The boolean value to store.
* @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
*         - Refer to WUPSStorageAPI_StoreItem for possible error codes.
* \see WUPSStorageAPI_StoreItem
*/
inline WUPSStorageError WUPSStorageAPI_StoreS64(wups_storage_item parent, const char *key, int64_t value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_S64, (void *) &value, sizeof(int64_t));
}

/**
* @brief Store a unsigned 32-bit integer value in storage.
*
* This is a convenience function to store a unsigned 32-bit integer value in the storage.
*
* @param parent The parent storage item. Can be NULL to refer to the root of the storage.
* @param key The key under which to store the value.
* @param value The boolean value to store.
* @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
*         - Refer to WUPSStorageAPI_StoreItem for possible error codes.
* \see WUPSStorageAPI_StoreItem
*/
inline WUPSStorageError WUPSStorageAPI_StoreU32(wups_storage_item parent, const char *key, uint32_t value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_U32, (void *) &value, sizeof(uint32_t));
}

/**
* @brief Store a unsigned 64-bit integer value in storage.
*
* This is a convenience function to store a unsigned 64-bit integer value in the storage.
*
* @param parent The parent storage item. Can be NULL to refer to the root of the storage.
* @param key The key under which to store the value.
* @param value The boolean value to store.
* @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
*         - Refer to WUPSStorageAPI_StoreItem for possible error codes.
* \see WUPSStorageAPI_StoreItem
*/
inline WUPSStorageError WUPSStorageAPI_StoreU64(wups_storage_item parent, const char *key, uint64_t value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_U64, (void *) &value, sizeof(uint64_t));
}

/**
* @brief Store a float value in storage.
*
* This is a convenience function to store a float value in the storage.
*
* @param parent The parent storage item. Can be NULL to refer to the root of the storage.
* @param key The key under which to store the value.
* @param value The boolean value to store.
* @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
*         - Refer to WUPSStorageAPI_StoreItem for possible error codes.
* \see WUPSStorageAPI_StoreItem
*/
inline WUPSStorageError WUPSStorageAPI_StoreFloat(wups_storage_item parent, const char *key, float value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_FLOAT, (void *) &value, sizeof(float));
}

/**
* @brief Store a double value in storage.
*
* This is a convenience function to store a double value in the storage.
*
* @param parent The parent storage item. Can be NULL to refer to the root of the storage.
* @param key The key under which to store the value.
* @param value The boolean value to store.
* @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
*         - Refer to WUPSStorageAPI_StoreItem for possible error codes.
* \see WUPSStorageAPI_StoreItem
*/
inline WUPSStorageError WUPSStorageAPI_StoreDouble(wups_storage_item parent, const char *key, double value) {
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_DOUBLE, (void *) &value, sizeof(double));
}

/**
* @brief Store a binary data in storage.
*
* This is a convenience function to store binary data in the storage.
*
* @param parent The parent storage item. Can be NULL to refer to the root of the storage.
* @param key The key under which to store the value.
* @param data A pointer to the binary data to store.
* @param size The size of the binary data to store.
* @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
*         - Refer to WUPSStorageAPI_StoreItem for possible error codes.
* \see WUPSStorageAPI_StoreItem
*/
inline WUPSStorageError WUPSStorageAPI_StoreBinary(wups_storage_item parent, const char *key, const void *data, uint32_t size) {
    if (data == NULL) {
        return WUPS_STORAGE_ERROR_INVALID_ARGS;
    }
    return WUPSStorageAPI_StoreItem(parent, key, WUPS_STORAGE_ITEM_BINARY, (void *) data, size);
}

/**
 * @brief Retrieve data from storage.
 *
 * This function retrieves data of the specified type stored under the given key in the provided parent item.
 * The retrieved data is stored in the 'data' buffer, and the actual size of the data is returned in 'outSize'.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the data is stored.
 * @param type The expected type of the data to retrieve.
 * @param data A pointer to the buffer where the retrieved data will be stored.
 * @param maxSize The maximum size of the 'data' buffer.
 * @param[out] outSize A pointer to a variable where the size of the retrieved data will be stored. Can be NULL.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - WUPS_STORAGE_ERROR_SUCCESS: The data retrieval was successful.
 *         - WUPS_STORAGE_ERROR_INVALID_ARGS: 'data' was NULL, 'outSize' was NULL, or 'maxSize' was unexpected.
 *         - WUPS_STORAGE_ERROR_NOT_FOUND: No item with the given key and type has been found inside the given parent.
 *         - WUPS_STORAGE_ERROR_MALLOC_FAILED: Failed to allocate memory.
 *         - WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL: The given buffer was too small to receive the full data. See: WUPSStorageAPI_GetItemSize
 *         - WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED: The library is not initialized properly; make sure to use the WUPS_USE_STORAGE macro.
 */
WUPSStorageError WUPSStorageAPI_GetItem(wups_storage_item parent, const char *key, WUPSStorageItemType type, void *data, uint32_t maxSize, uint32_t *outSize);

/**
 * @brief Retrieve a string from storage.
 *
 * This function is a convenience wrapper around WUPSStorageAPI_GetItem for retrieving strings.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the string is stored.
 * @param[out] outString A pointer to the buffer where the retrieved string will be stored.
 * @param maxSize The maximum size of the 'outString' buffer.
 * @param[out] outSize A pointer to a variable where the size of the retrieved string will be stored. Can be NULL.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         Refer to WUPSStorageAPI_GetItem for possible error codes.
* \see WUPSStorageAPI_GetItem
* \see WUPSStorageAPI_GetItemSize
*/
inline WUPSStorageError WUPSStorageAPI_GetString(wups_storage_item parent, const char *key, char *outString, uint32_t maxSize, uint32_t *outSize) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_STRING, outString, maxSize, outSize);
}

/**
 * @brief Retrieve a boolean value from storage.
 *
 * This function is a convenience wrapper around WUPSStorageAPI_GetItem for retrieving boolean values.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the value is stored.
 * @param[out] outValue A pointer to the variable where the retrieved boolean value will be stored
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         Refer to WUPSStorageAPI_GetItem for possible error codes.
 */
inline WUPSStorageError WUPSStorageAPI_GetBool(wups_storage_item parent, const char *key, bool *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_BOOL, outValue, sizeof(*outValue), NULL);
}

/**
 * @brief Retrieve a int value from storage.
 *
 * This function is a convenience wrapper around WUPSStorageAPI_GetItem for retrieving int values.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the value is stored.
 * @param[out] outValue A pointer to the variable where the retrieved boolean value will be stored
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         Refer to WUPSStorageAPI_GetItem for possible error codes.
 */
inline WUPSStorageError WUPSStorageAPI_GetInt(wups_storage_item parent, const char *key, int32_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_S32, outValue, sizeof(*outValue), NULL);
}

/**
 * @brief Retrieve a signed 32-bit integer value from storage.
 *
 * This function is a convenience wrapper around WUPSStorageAPI_GetItem for retrieving signed 32-bit integer values.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the value is stored.
 * @param[out] outValue A pointer to the variable where the retrieved boolean value will be stored
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         Refer to WUPSStorageAPI_GetItem for possible error codes.
 */
inline WUPSStorageError WUPSStorageAPI_GetS32(wups_storage_item parent, const char *key, int32_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_S32, outValue, sizeof(*outValue), NULL);
}

/**
 * @brief Retrieve a signed 64-bit integer value from storage.
 *
 * This function is a convenience wrapper around WUPSStorageAPI_GetItem for retrieving signed 64-bit integer values.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the value is stored.
 * @param[out] outValue A pointer to the variable where the retrieved boolean value will be stored
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         Refer to WUPSStorageAPI_GetItem for possible error codes.
 */
inline WUPSStorageError WUPSStorageAPI_GetS64(wups_storage_item parent, const char *key, int64_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_S64, outValue, sizeof(*outValue), NULL);
}

/**
 * @brief Retrieve a unsigned 32-bit integer value from storage.
 *
 * This function is a convenience wrapper around WUPSStorageAPI_GetItem for retrieving unsigned 32-bit integer values.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the value is stored.
 * @param[out] outValue A pointer to the variable where the retrieved boolean value will be stored
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         Refer to WUPSStorageAPI_GetItem for possible error codes.
 */
inline WUPSStorageError WUPSStorageAPI_GetU32(wups_storage_item parent, const char *key, uint32_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_U32, outValue, sizeof(*outValue), NULL);
}

/**
 * @brief Retrieve a unsigned 64-bit integer value from storage.
 *
 * This function is a convenience wrapper around WUPSStorageAPI_GetItem for retrieving unsigned 64-bit integer values.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the value is stored.
 * @param[out] outValue A pointer to the variable where the retrieved boolean value will be stored
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         Refer to WUPSStorageAPI_GetItem for possible error codes.
 */
inline WUPSStorageError WUPSStorageAPI_GetU64(wups_storage_item parent, const char *key, uint64_t *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_U64, outValue, sizeof(*outValue), NULL);
}

/**
 * @brief Retrieve a float value from storage.
 *
 * This function is a convenience wrapper around WUPSStorageAPI_GetItem for retrieving unsigned float values.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the value is stored.
 * @param[out] outValue A pointer to the variable where the retrieved boolean value will be stored
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         Refer to WUPSStorageAPI_GetItem for possible error codes.
 */
inline WUPSStorageError WUPSStorageAPI_GetFloat(wups_storage_item parent, const char *key, float *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_FLOAT, outValue, sizeof(*outValue), NULL);
}

/**
 * @brief Retrieve a unsigned double value from storage.
 *
 * This function is a convenience wrapper around WUPSStorageAPI_GetItem for retrieving unsigned double values.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the value is stored.
 * @param[out] outValue A pointer to the variable where the retrieved boolean value will be stored
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         Refer to WUPSStorageAPI_GetItem for possible error codes.
 */
inline WUPSStorageError WUPSStorageAPI_GetDouble(wups_storage_item parent, const char *key, double *outValue) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_DOUBLE, outValue, sizeof(*outValue), NULL);
}

/**
 * @brief Retrieve binary data from storage.
 *
 * This function is a convenience wrapper around WUPSStorageAPI_GetItem for retrieving binary data.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the string is stored.
 * @param[out] outData A pointer to the buffer where the retrieved data will be stored.
 * @param maxSize The maximum size of the 'outData' buffer.
 * @param[out] outSize A pointer to a variable where the size of the retrieved data will be stored. Can be NULL.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         Refer to WUPSStorageAPI_GetItem for possible error codes.
* \see WUPSStorageAPI_GetItem
* \see WUPSStorageAPI_GetItemSize
*/
inline WUPSStorageError WUPSStorageAPI_GetBinary(wups_storage_item parent, const char *key, void *outData, uint32_t maxSize, uint32_t *outSize) {
    return WUPSStorageAPI_GetItem(parent, key, WUPS_STORAGE_ITEM_BINARY, outData, maxSize, outSize);
}

/**
 * @brief Get the size of binary or string data stored under a key in the provided storage item.
 *
 * This function retrieves the size of the data stored under the given key in the provided parent item.
 * The size is returned in the 'outSize' variable.
 * Only compatible with strings and binary data.
 *
 * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
 * @param key The key under which the data size is to be retrieved.
 * @param[out] outSize A pointer to a variable where the size of the stored data will be stored.
 * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
 *         - WUPS_STORAGE_ERROR_SUCCESS: The size retrieval was successful.
 *         - WUPS_STORAGE_ERROR_INVALID_ARGS: 'outSize' was NULL.
 *         - WUPS_STORAGE_ERROR_MALLOC_FAILED: Failed to allocate memory
 *         - WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE: "key" is refering to a non-string and non-binary item.
 *         - WUPS_STORAGE_ERROR_NOT_FOUND: No item with the given key has been found inside the given parent.
 *         - WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED: The library is not initialized properly; make sure to use the WUPS_USE_STORAGE macro.
 */
WUPSStorageError WUPSStorageAPI_GetItemSize(wups_storage_item parent, const char *key, WUPSStorageItemType itemType, uint32_t *outSize);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <cstring>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

class WUPSStorageSubItem;

/**
 * @namespace WUPSStorageAPI
 * @brief C++ wrapper for the WUPS storage API. See the C-API for detailed information about the possible return values.
 */
namespace WUPSStorageAPI {
    template<typename T>
    struct GetStorageItemType;

    template<>
    struct GetStorageItemType<std::string> {
        static constexpr WUPSStorageItemTypes value = WUPS_STORAGE_ITEM_STRING;
    };

    template<>
    struct GetStorageItemType<std::vector<uint8_t>> {
        static constexpr WUPSStorageItemTypes value = WUPS_STORAGE_ITEM_BINARY;
    };

    /**
     * @brief Gets a string representation of the specified storage status.
     * @param err The storage error status.
     * @return A string describing the provided storage status.
     * \see WUPSStorageAPI_GetStatusStr
     */
    std::string_view GetStatusStr(const WUPSStorageError &err) noexcept;

    /**
     * @brief Saves storage data.
     * @param forceSave If true, forces saving data even if there are no changes.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_SaveStorage
     */
    WUPSStorageError SaveStorage(bool forceSave = false);

    /**
     * @brief Forces a reload of storage data.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_ForceReloadStorage
     */
    WUPSStorageError ForceReloadStorage();

    /**
     * @brief Wipes all storage data.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_WipeStorage
    */
    WUPSStorageError WipeStorage();

    /**
     * @brief Get the root item of the WUPS Storage.
     *
     * This function retrieves the root item as WUPSStorageSubItem.
     *
     * @return The root item of the WUPS Storage.
     *
     * @see WUPSStorageSubItem
     */
    WUPSStorageSubItem GetRootItem() noexcept;

    /**
     * @brief Deletes an item from the root of the storage.
     * @param key The key of the item to be deleted.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_DeleteItem
     */
    WUPSStorageError DeleteItem(std::string_view key) noexcept;

    /**
     * @brief Creates a new sub-item in root of the storage with the given key.
     *
     * This function creates a new sub-item in root the storage with the specified key.
     * The function returns an optional that contains the created sub-item if the operation was successful,
     * or an empty optional if an error occurred. The error code is stored in the provided WUPSStorageError object.
     *
     * @param key The key for the new sub-item.
     * @param err The error code variable to store the result of the operation.
     * @return An optional that contains the created sub-item if the operation was successful, or an empty optional if an error occurred.
     *
     * @see WUPSStorageSubItem
     * @see WUPSStorageAPI_CreateSubItem
     *
     * @note Callers should check the value of the returned optional to determine if the operation was successful.
     * @note There is a variant of the CreateSubItem function which throws a std::runtime_error exception if an error occurs during sub-item creation.
     */
    std::optional<WUPSStorageSubItem> CreateSubItem(std::string_view key, WUPSStorageError &err) noexcept;

    /**
     * @brief Creates a subitem in the root of the storage.
     *
     * This function is used to create a subitem in the WUPSStorage. The subitem is associated with the specified key.
     *
     * @param key The key for the subitem.
     *
     * @throw std::runtime_error if an error occurs during the creation of the subitem.
     *
     * @return The created WUPSStorageSubItem object.
     *
     * @see WUPSStorageSubItem
     * @see WUPSStorageAPI_CreateSubItem
     *
     * @note This function has a non-throwing variation. In the event of an error during the creation of the subitem,
     * the non-throwing variation of this function will return a std::null_opt rather than throwing a std::runtime_error exception.
     */
    WUPSStorageSubItem CreateSubItem(std::string_view key);

    /**
     * @brief Retrieves a sub-item from the root of the storage with the given key.
     *
     * This function retrieves a sub-item from the root the storage with the specified key.
     * The function returns an optional that contains the sub-item if the operation was successful, or an empty optional on error.
     * The error code is stored in the provided WUPSStorageError object.
     *
     * @param key The key for the new sub-item.
     * @param err The error code variable to store the result of the operation.
     * @return An optional that sub-item if the operation was successful, or an empty optional if the sub-item was not found.
     *
     * @see WUPSStorageSubItem
     * @see WUPSStorageAPI_GetSubItem
     *
     * @note Callers should check the value of the returned optional to determine if the operation was successful.
     * @note There is a variant of the CreateSubItem function which throws a std::runtime_error exception on error.
     */
    std::optional<WUPSStorageSubItem> GetSubItem(std::string_view key, WUPSStorageError &err) noexcept;

    /**
     * @brief Retrieves a subitem in the root of the storage.
     *
     * This function is used to retrieve a subitem from the WUPSStorage by key.
     *
     * @param key The key of the subitem.
     *
     * @throw std::runtime_error if an error occurs during the retrieving of the subitem or a subitem with the given key was not found
     *
     * @return The WUPSStorageSubItem object if found
     *
     * @see WUPSStorageSubItem
     * @see WUPSStorageAPI_GetSubItem
     *
     * @note This function has a non-throwing variation. In the event of an error during the retrieving of the subitem,
     * the non-throwing variation of this function will return a std::null_opt rather than throwing a std::runtime_error exception.
     */
    WUPSStorageSubItem GetSubItem(std::string_view key);

    /**
     * @brief Retrieves or creates a sub-item with the given key in the root of the storage.. (non-throwing)
     *
     * This function retrieves an existing sub-item with the specified key if it exists.
     * If the sub-item does not exist in the root of the storage, it creates a new sub-item with the given key.
     *
     * @param key The key of the sub-item to retrieve or create.
     * @param[out] err The error code variable to store the result of the operation on error.
     *      *
     * @return An optional containing the retrieved or newly created sub-item. Returns std::nullopt on error and sets err.
     *
     * @see WUPSStorageSubItem
     *
     * @note There is a variant of the GetOrCreateSubItem function which throws a std::runtime_error exception if an error
     */
    std::optional<WUPSStorageSubItem> GetOrCreateSubItem(std::string_view key, WUPSStorageError &err) noexcept;

    /**
     * @brief Retrieves or creates a sub-item with the given key in the root of the storage.
     *
     * This function retrieves an existing sub-item with the specified key if it exists.
     * If the sub-item does not exist in the root of the storage, it creates a new sub-item with the given key.
     *
     * @param key The key of the sub-item to retrieve or create.
     *
     * @throw std::runtime_error if creating or retrieving the sub-item for the given key failed.
     *
     * @return Returns the retrieved or newly created sub-item, throws exception on error.
     *
     * @see WUPSStorageAPI_GetOrCreateSubItem
     * @note This function has a non-throwing variation. This function will return a std::null_opt rather than throwing
     * a std::runtime_error exception on error.
    **/
    WUPSStorageSubItem GetOrCreateSubItem(std::string_view key);

    /**
     * @brief Get the size of an item associated with the given key.
     *
     * This function retrieves the size of an item associated with the specified key
     * from the root of the storage. The item size is stored in the 'outSize' parameter.
     *
     * @param key The key of the item.
     * @param outSize The variable to store the size of the item.
     * @return WUPSStorageError Returns WUPS_STORAGE_ERROR_SUCCESS on success.
     *         Returns an error code if an error occurs.
     * \see WUPSStorageAPI_GetItemSize
     */
    template<typename T>
    inline WUPSStorageError GetItemSize(std::string_view key, uint32_t &outSize) noexcept {
        return WUPSStorageAPI_GetItemSize(nullptr, key.data(), WUPSStorageAPI::GetStorageItemType<T>::value, &outSize);
    }

    /**
     * @brief Retrieve a value from the storage.
     *
     * This function retrieves a value from the storage based on the given parent storage item and key.
     * The value is returned using the outValue parameter.
     *
     * @tparam T The type of the value to retrieve.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved value will be stored.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_GetItem
     */
    template<typename T>
    inline WUPSStorageError GetEx(wups_storage_item parent, std::string_view key, T &outValue) noexcept;

    /**
     * @brief Retrieves a signed 32-bit integer value from a storage item.
     *
     * This function is a specialization of the template function `GetEx` for retrieving `int32_t` values from a storage item.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved value will be stored.
     * @return WUPSStorageError Returns `WUPS_STORAGE_ERROR_SUCCESS` on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_GetS32
     * \see WUPSStorageAPI_GetItem
     */
    template<>
    inline WUPSStorageError GetEx<int32_t>(wups_storage_item parent, std::string_view key, int32_t &outValue) noexcept {
        return WUPSStorageAPI_GetS32(parent, key.data(), &outValue);
    }

    /**
     * @brief Retrieves a signed 64-bit integer value from a storage item.
     *
     * This function is a specialization of the template function `GetEx` for retrieving `int64_t` values from a storage item.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved value will be stored.
     * @return WUPSStorageError Returns `WUPS_STORAGE_ERROR_SUCCESS` on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_GetS64
     * \see WUPSStorageAPI_GetItem
     */
    template<>
    inline WUPSStorageError GetEx<int64_t>(wups_storage_item parent, std::string_view key, int64_t &outValue) noexcept {
        return WUPSStorageAPI_GetS64(parent, key.data(), &outValue);
    }

    /**
     * @brief Retrieves an unsigned 32-bit integer value from a storage item.
     *
     * This function is a specialization of the template function `GetEx` for retrieving `uint32_t` values from a storage item.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved value will be stored.
     * @return WUPSStorageError Returns `WUPS_STORAGE_ERROR_SUCCESS` on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_GetU32
     * \see WUPSStorageAPI_GetItem
     */
    template<>
    inline WUPSStorageError GetEx<uint32_t>(wups_storage_item parent, std::string_view key, uint32_t &outValue) noexcept {
        return WUPSStorageAPI_GetU32(parent, key.data(), &outValue);
    }

    /**
     * @brief Retrieves an unsigned 64-bit integer value from a storage item.
     *
     * This function is a specialization of the template function `GetEx` for retrieving `uint64_t` values from a storage item.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved value will be stored.
     * @return WUPSStorageError Returns `WUPS_STORAGE_ERROR_SUCCESS` on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_GetU64
     * \see WUPSStorageAPI_GetItem
     */
    template<>
    inline WUPSStorageError GetEx<uint64_t>(wups_storage_item parent, std::string_view key, uint64_t &outValue) noexcept {
        return WUPSStorageAPI_GetU64(parent, key.data(), &outValue);
    }

    /**
     * @brief Retrieves a bool value from a storage item.
     *
     * This function is a specialization of the template function `GetEx` for retrieving `bool` values from a storage item.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved value will be stored.
     * @return WUPSStorageError Returns `WUPS_STORAGE_ERROR_SUCCESS` on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_GetBool
     * \see WUPSStorageAPI_GetItem
     */
    template<>
    inline WUPSStorageError GetEx<bool>(wups_storage_item parent, std::string_view key, bool &outValue) noexcept {
        return WUPSStorageAPI_GetBool(parent, key.data(), &outValue);
    }

    /**
     * @brief Retrieves a float value from a storage item.
     *
     * This function is a specialization of the template function `GetEx` for retrieving `float` values from a storage item.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved value will be stored.
     * @return WUPSStorageError Returns `WUPS_STORAGE_ERROR_SUCCESS` on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_GetFloat
     * \see WUPSStorageAPI_GetItem
     */
    template<>
    inline WUPSStorageError GetEx<float>(wups_storage_item parent, std::string_view key, float &outValue) noexcept {
        return WUPSStorageAPI_GetFloat(parent, key.data(), &outValue);
    }

    /**
     * @brief Retrieves a double value from a storage item.
     *
     * This function is a specialization of the template function `GetEx` for retrieving `double` values from a storage item.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved value will be stored.
     * @return WUPSStorageError Returns `WUPS_STORAGE_ERROR_SUCCESS` on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_GetFloat
     * \see WUPSStorageAPI_GetItem
     */
    template<>
    inline WUPSStorageError GetEx<double>(wups_storage_item parent, std::string_view key, double &outValue) noexcept {
        return WUPSStorageAPI_GetDouble(parent, key.data(), &outValue);
    }

    /**
     * @brief Retrieves an enum value from storage.
     *
     * This function is a specialization of the template function `GetEx` for retrieving enum values from a storage item.
     * The enum must be of size sizeof(uint32_t).
     *
     * @tparam T The enum type.
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved enum value will be stored.
     * @return WUPSStorageError Returns WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @note This function assumes that the enum type is of size sizeof(uint32_t).
     *       If the enum type does not comply with this requirement, a static_assert will be triggered.
     * \see WUPSStorageAPI_GetU32
     * \see WUPSStorageAPI_GetItem
     */
    template<typename T>
    inline WUPSStorageError GetEx(wups_storage_item parent, std::string_view key, T &outValue) noexcept {
        static_assert(sizeof(T) == sizeof(uint32_t) && std::is_enum<T>::value, "T must be an enum of size sizeof(uint32_t)");
        return WUPSStorageAPI_GetU32(parent, key.data(), (uint32_t *) &outValue);
    }

    /**
     * @brief Retrieves a vector of uint8_t from storage.
     *
     * This template specialization of the GetEx function retrieves a vector of uint8_t from the storage.
     * It first checks if the outValue vector is empty (size == 0).
     * If it is, it retrieves the size of the data and resizes the outValue vector to that size. Otherwise the existing vector (size) will the be used.
     * Then, it retrieves the binary data from the storage using the WUPSStorageAPI_GetBinary function.
     * If the retrieval is successful, the outValue vector is resized to the size of the retrieved data.
     * If the retrieval fails, the outValue vector is resized to 0.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which the data is stored.
     * @param[out] outValue A reference to the vector where the retrieved data will be stored. Will be resized to fit the data only if it's empty.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_GetBinary
     * @see WUPSStorageAPI_GetItemSize
     */
    template<>
    inline WUPSStorageError GetEx<std::vector<uint8_t>>(wups_storage_item parent, std::string_view key, std::vector<uint8_t> &outValue) noexcept {
        uint32_t outSize = 0;

        if (outValue.empty()) {
            uint32_t resizeToSize = 0;
            auto r                = WUPSStorageAPI_GetItemSize(parent, key.data(), WUPS_STORAGE_ITEM_BINARY, &resizeToSize);
            if (r == WUPS_STORAGE_ERROR_SUCCESS) {
                outValue.resize(resizeToSize);
            } else {
                return r;
            }
        }

        auto res = WUPSStorageAPI_GetBinary(parent, key.data(), outValue.data(), outValue.size(), &outSize);
        if (res == WUPS_STORAGE_ERROR_SUCCESS) {
            outValue.resize(outSize);
        } else {
            outValue.resize(0);
        }
        return res;
    }

    /**
     * @brief Retrieves a string value from a storage item with the given key.
     *
     * If the input `outValue` is empty, the function first retrieves the size of the stored data using `WUPSStorageAPI_GetItemSize`.
     * If the retrieval is successful, the function resizes `outValue` to the retrieved size.
     * Then, it calls `WUPSStorageAPI_GetString` to retrieve the string from the storage item.
     * If the retrieval is successful, the function resizes `outValue` to remove the null terminator.
     *
     * @param parent The parent storage item. Can be `NULL` to refer to the root of the storage.
     * @param key The key under which the string is stored.
     * @param[out] outValue A reference to a `std::string` object where the retrieved string will be stored. Will be resized to fit the data only if it's empty.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_GetBinary
     * @see WUPSStorageAPI_GetString
     */
    template<>
    inline WUPSStorageError GetEx<std::string>(wups_storage_item parent, std::string_view key, std::string &outValue) noexcept {
        uint32_t outSize = 0;
        if (outValue.empty()) {
            uint32_t resizeToSize = 0;
            auto r                = WUPSStorageAPI_GetItemSize(parent, key.data(), WUPS_STORAGE_ITEM_STRING, &resizeToSize);
            if (r == WUPS_STORAGE_ERROR_SUCCESS) {
                outValue.resize(resizeToSize);
            } else {
                return r;
            }
        }

        auto res = WUPSStorageAPI_GetString(parent, key.data(), outValue.data(), outValue.size(), &outSize);
        if (res == WUPS_STORAGE_ERROR_SUCCESS) {
            // outSize does count the null terminator as well, std::string's size() doesn't include a null terminator.
            outValue.resize(strlen(outValue.c_str()));
        } else {
            outValue.resize(0);
        }
        return res;
    }

    /**
     * @brief Retrieve a value from the storage using a key.
     *
     * This function retrieves a value from the storage based on the given key. The retrieved value is stored in the outValue parameter.
     * This function will always attempt find the key in the root of the storage.
     *
     * @tparam T The type of the value to retrieve.
     *
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved value will be stored.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_GetItem
     * @see GetEx
     */
    template<typename T>
    inline WUPSStorageError Get(std::string_view key, T &outValue) noexcept {
        return GetEx(nullptr, key.data(), outValue);
    }

    /**
     * @brief Store an item in the storage.
     *
     * This function stores an item of type T in the storage. Wrapper for WUPSStorageAPI_StoreItem
     *
     * @tparam T The type of the value to store.
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which to store the value.
     * @param value The value to store.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreItem()
     */
    template<typename T>
    inline WUPSStorageError StoreEx(wups_storage_item parent, std::string_view key, const T &value) noexcept;

    /**
     * @brief Stores a signed 32-bit integer value in the storage.
     *
     * This function is a specialization of the template function `StoreEx` for the `int32_t` data type.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which to store the value.
     * @param value The value to store.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreS32
     * @see WUPSStorageAPI_StoreItem
     */
    template<>
    inline WUPSStorageError StoreEx<int32_t>(wups_storage_item parent, std::string_view key, const int32_t &value) noexcept {
        return WUPSStorageAPI_StoreS32(parent, key.data(), value);
    }

    /**
     * @brief Stores a signed 64-bit integer value in the storage.
     *
     * This function is a specialization of the template function `StoreEx` for the `int64_t` data type.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which to store the value.
     * @param value The value to store.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreS64
     * @see WUPSStorageAPI_StoreItem
     */
    template<>
    inline WUPSStorageError StoreEx<int64_t>(wups_storage_item parent, std::string_view key, const int64_t &value) noexcept {
        return WUPSStorageAPI_StoreS64(parent, key.data(), value);
    }

    /**
     * @brief Stores an unsigned 32-bit integer value in the storage.
     *
     * This function is a specialization of the template function `StoreEx` for the `uint32_t` data type.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which to store the value.
     * @param value The value to store.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreU32
     * @see WUPSStorageAPI_StoreItem
     */
    template<>
    inline WUPSStorageError StoreEx<uint32_t>(wups_storage_item parent, std::string_view key, const uint32_t &value) noexcept {
        return WUPSStorageAPI_StoreU32(parent, key.data(), value);
    }

    /**
     * @brief Stores an unsigned 64-bit integer value in the storage.
     *
     * This function is a specialization of the template function `StoreEx` for the `uint64_t` data type.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which to store the value.
     * @param value The value to store.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreU64
     * @see WUPSStorageAPI_StoreItem
     */
    template<>
    inline WUPSStorageError StoreEx<uint64_t>(wups_storage_item parent, std::string_view key, const uint64_t &value) noexcept {
        return WUPSStorageAPI_StoreU64(parent, key.data(), value);
    }

    /**
     * @brief Stores a bool value in the storage.
     *
     * This function is a specialization of the template function `StoreEx` for the `bool` data type.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which to store the value.
     * @param value The value to store.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreBool
     * @see WUPSStorageAPI_StoreItem
     */
    template<>
    inline WUPSStorageError StoreEx<bool>(wups_storage_item parent, std::string_view key, const bool &value) noexcept {
        return WUPSStorageAPI_StoreBool(parent, key.data(), value);
    }

    /**
     * @brief Stores a float value in the storage.
     *
     * This function is a specialization of the template function `StoreEx` for the `float` data type.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which to store the value.
     * @param value The value to store.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreFloat
     * @see WUPSStorageAPI_StoreItem
     */
    template<>
    inline WUPSStorageError StoreEx<float>(wups_storage_item parent, std::string_view key, const float &value) noexcept {
        return WUPSStorageAPI_StoreFloat(parent, key.data(), value);
    }

    /**
     * @brief Stores a float double in the storage.
     *
     * This function is a specialization of the template function `StoreEx` for the `double` data type.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which to store the value.
     * @param value The value to store.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreDouble
     * @see WUPSStorageAPI_StoreItem
     */
    template<>
    inline WUPSStorageError StoreEx<double>(wups_storage_item parent, std::string_view key, const double &value) noexcept {
        return WUPSStorageAPI_StoreDouble(parent, key.data(), value);
    }

    /**
     * @brief Stores a std::vector<uint8_t> in the storage.
     *
     * This function is a specialization of the template function `StoreEx` for the `std::vector<uint8_t>` data type.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which to store the value.
     * @param value The std::vector<uint8_t> to store.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreBinary
     * @see WUPSStorageAPI_StoreItem
     */
    template<>
    inline WUPSStorageError StoreEx<std::vector<uint8_t>>(wups_storage_item parent, std::string_view key, const std::vector<uint8_t> &value) noexcept {
        return WUPSStorageAPI_StoreBinary(parent, key.data(), value.data(), value.size());
    }

    /**
     * @brief Stores a string value in the storage.
     *
     * This function is a specialization of the template function `StoreEx` for the `std::string` data type.
     *
     * This function stores a null-terminated string value in the storage under the specified key.
     * It is a template specialization of the StoreEx function for the string type.
     *
     * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
     * @param key The key under which to store the string.
     * @param value The string value to store. Must be null-terminated
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreString
     * @see WUPSStorageAPI_StoreItem
     */
    template<>
    inline WUPSStorageError StoreEx<std::string>(wups_storage_item parent, std::string_view key, const std::string &value) noexcept {
        return WUPSStorageAPI_StoreString(parent, key.data(), value.c_str());
    }

    /**
        * @brief Store an enum value in the storage.
        *
        * This function stores an enum value of type T in the storage. The enum must have a size of sizeof(uint32_t).
        *
        * @tparam T The enum type.
        * @param parent The parent storage item. Can be NULL to refer to the root of the storage.
        * @param key The key under which to store the value.
        * @param value The enum value to store.
        * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
        *         - Possible error codes: WUPS_STORAGE_ERROR_INVALID_ARGS, WUPS_STORAGE_ERROR_MALLOC_FAILED,
        *           WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE, WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL,
        *           WUPS_STORAGE_ERROR_ALREADY_EXISTS, WUPS_STORAGE_ERROR_IO_ERROR,
        *           WUPS_STORAGE_ERROR_NOT_FOUND, WUPS_STORAGE_ERROR_INTERNAL_NOT_INITIALIZED,
        *           WUPS_STORAGE_ERROR_INTERNAL_INVALID_VERSION, WUPS_STORAGE_ERROR_UNKNOWN_ERROR.
        *
        * @see WUPSStorageAPI_StoreItem()
    */
    template<typename T>
    inline WUPSStorageError StoreEx(wups_storage_item parent, std::string_view key, const T &value) noexcept {
        static_assert(sizeof(T) == sizeof(uint32_t) && std::is_enum<T>::value, "T must be an enum of size sizeof(uint32_t)");
        return WUPSStorageAPI_StoreU32(parent, key.data(), (uint32_t) value);
    }

    /**
     * @brief Stores a value with a given key in the storage.
     *
     * This function stores a value of type T in the storage under the specified key.
     * It is a wrapper around the `StoreEx` function, stores into the root item of the storage by default.
     *
     * @tparam T The type of the value to store.
     * @param key The key under which the value will be stored.
     * @param value The value to store.
     * @return WUPSStorageError The error code indicating the result of the operation.
     * @see WUPSStorageAPI_StoreItem()
     * @see WUPSStorageAPI::StoreEx()
     */
    template<typename T>
    inline WUPSStorageError Store(std::string_view key, const T &value) noexcept {
        return StoreEx(nullptr, key, value);
    }

    /**
     * @brief Retrieves the value associated with the specified key from a storage item.
     *        If the key is not found, stores the default value with taht key in the storage item.
     *
     * @tparam T The type of the value to retrieve and store.
     * @param parent The storage item to retrieve and store the value from. Can be NULL to refer to the root of the storage
     * @param key The key associated with the value.
     * @param outValue [out] The output parameter to store the retrieved value.
     * @param defaultValue The default value to store in storage and outValue if the key does not exist.
     * @return The error code indicating the success or failure of the operation.
     *
     * @details This function retrieves the value associated with the specified key from the given parent.
     *          If the key is not found, the default value is stored in the storage item using the StoreEx function.
     *          The retrieved or stored value is then assigned to the output parameter outValue.
     *          The function returns an error code indicating the success or failure of the operation.
     *          The possible error codes are defined in the WUPSStorageError enumeration.
     *          If the retrieval and storage operations are successful, the error code will be WUPS_STORAGE_ERROR_SUCCESS.
     * \see GetEx
     * \see StoreEx
     */
    template<typename T>
    inline WUPSStorageError GetOrStoreDefaultEx(wups_storage_item parent, std::string_view key, T &outValue, const T &defaultValue) noexcept {
        WUPSStorageError err = GetEx(parent, key, outValue);
        if (err == WUPS_STORAGE_ERROR_NOT_FOUND) {
            err = StoreEx(parent, key, defaultValue);
            if (err == WUPS_STORAGE_ERROR_SUCCESS) {
                outValue = defaultValue;
            }
        }
        return err;
    }

    /**
     * @brief Retrieves a value associated with the given key from storage, or stores a default value if the key does not exist.
     *
     * @tparam T The type of the value.
     * @param key The key of the value to retrieve or store.
     * @param outValue The variable to store the retrieved or stored value.
     * @param defaultValue The default value to store in storage and outValue if the key does not exist.
     * @return WUPSStorageError An error code indicating the success or failure of the operation.
     *
     * @details This function retrieves the value associated with the specified key from the root of the storage.
     *          If the key is not found, the default value is stored in the storage item using the StoreEx function.
     *          The retrieved or stored value is then assigned to the output parameter outValue.
     *          The function returns an error code indicating the success or failure of the operation.
     *          The possible error codes are defined in the WUPSStorageError enumeration.
     *          If the retrieval and storage operations are successful, the error code will be WUPS_STORAGE_ERROR_SUCCESS.
     *
     * @note This function internally calls the GetEx and StoreEx functions of the storage API.
     */
    template<typename T>
    inline WUPSStorageError GetOrStoreDefault(std::string_view key, T &outValue, const T &defaultValue) noexcept {
        return GetOrStoreDefaultEx<T>(nullptr, key, outValue, defaultValue);
    }
} // namespace WUPSStorageAPI

class WUPSStorageSubItem {
public:
    explicit WUPSStorageSubItem(wups_storage_item handle) : mHandle(handle) {
    }
    virtual ~WUPSStorageSubItem() = default;

    bool operator==(const WUPSStorageSubItem &rhs) const;
    bool operator!=(const WUPSStorageSubItem &rhs) const;

    /**
     * @brief Store an item in the sub-item.
     *
     * This function stores an item of type T in the sub-item. Wrapper for WUPSStorageAPI_StoreItem
     *
     * @tparam T The type of the value to store.
     * @param key The key under which to store the value.
     * @param value The value to store.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_StoreItem()
     */
    template<typename T>
    inline WUPSStorageError Store(std::string_view key, const T &value) noexcept {
        return WUPSStorageAPI::StoreEx(mHandle, key, value);
    }

    /**
     * @brief Retrieve a value from this sub-item.
     *
     * This function retrieves a value from this sub-item based on the given key.
     * The value is returned using the outValue parameter.
     *
     * @tparam T The type of the value to retrieve.
     *
     * @param key The key under which the value is stored.
     * @param[out] outValue A reference to the variable where the retrieved value will be stored.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_GetItem
     */
    template<typename T>
    inline WUPSStorageError Get(std::string_view key, T &value) const noexcept {
        return WUPSStorageAPI::GetEx(mHandle, key, value);
    }

    /**
     * @brief Retrieves the value associated with the specified key from this sub-item.
     *        If the key is not found, stores the default value in this sub-item.
     *
     * @tparam T The type of the value to retrieve and store.
     *
     * @param key The key associated with the value.
     * @param outValue [out] The output parameter to store the retrieved value.
     * @param defaultValue The default value to store if the key is not found.
     * @return The error code indicating the success or failure of the operation.
     *
     * @details This function retrieves the value associated with the specified key from this sub-item.
     *          If the key is not found, the default value is stored in this sub-item using the StoreEx function.
     *          The retrieved or stored value is then assigned to the output parameter outValue.
     *          The function returns an error code indicating the success or failure of the operation.
     *          The possible error codes are defined in the WUPSStorageError enumeration.
     *          If the retrieval and storage operations are successful, the error code will be WUPS_STORAGE_ERROR_SUCCESS.
     *          If there are any errors, such as invalid arguments, memory allocation failure, I/O errors, or unknown errors,
     *          the error code will indicate the specific type of error occurred.
     * \see WUPSStorageAPI::GetOrStoreDefaultEx
     */
    template<typename T>
    inline WUPSStorageError GetOrStoreDefault(std::string_view key, T &outValue, const T &defaultValue) const noexcept {
        return WUPSStorageAPI::GetOrStoreDefaultEx(mHandle, key.data(), outValue, defaultValue);
    }

    /**
     * @brief Deletes an item from this sub-item
     * @param key The key of the item to be deleted.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     * \see WUPSStorageAPI_DeleteItem
     */
    WUPSStorageError DeleteItem(std::string_view key) noexcept;

    /**
     * @brief Get the size of binary or string data stored under a key in this sub-item
     *
     * This function retrieves the size of the data stored under the given key in this sub-item
     * The size is returned in the 'outSize' variable.
     * Only compatible with strings and binary data.
     *
     * @param key The key under which the data size is to be retrieved.
     * @param[out] outSize A reference to a variable where the size of the stored data will be stored.
     * @return WUPSStorageError WUPS_STORAGE_ERROR_SUCCESS on success, otherwise an appropriate error code.
     *
     * @see WUPSStorageAPI_GetItemSize()
     */
    template<typename T>
    WUPSStorageError GetItemSize(std::string_view key, uint32_t &outSize) noexcept {
        return WUPSStorageAPI_GetItemSize(mHandle, key.data(), WUPSStorageAPI::GetStorageItemType<T>::value, &outSize);
    }

    /**
     * @brief Create a sub-item in this sub-item. (non-throwing)
     *
     * This function is a convenience wrapper around WUPSStorageAPI_CreateSubItem for creating a sub-item in this sub-item
     * @param key The key of the sub item which should be created
     * @param err The error code variable to store the result of the operation on error.
     * @return An optional containing the created sub-item or std::nullopt on error
     *
     * @see WUPSStorageAPI_CreateSubItem
     * @note There is a variant of the CreateSubItem function which throws a std::runtime_error exception if an error occurs during sub-item creation.
     */
    std::optional<WUPSStorageSubItem> CreateSubItem(std::string_view key, WUPSStorageError &err) noexcept;

    /**
     * @brief Creates a sub-item in this sub-item
     *
     * This function is used to create a sub-item in this sub-item
     *
     * @param key The key for the sub-item.
     *
     * @throw std::runtime_error if an error occurs during the creation of the sub-item.
     *
     * @return The created WUPSStorageSubItem object.
     *
     * @see WUPSStorageSubItem
     *
     * @note This function has a non-throwing variation. In the event of an error during the creation of the sub-item,
     * the non-throwing variation of this function will return a std::null_opt rather than throwing a std::runtime_error exception.
     */
    [[nodiscard]] WUPSStorageSubItem CreateSubItem(std::string_view key);

    /**
     * @brief Retrieve a sub-item of this sub-item. (non-throwing)
     *
     * This function is a convenience wrapper around WUPSStorageAPI::GetSubItem for retrieving a sub-item of this sub-item
     *
     * @param key The key of the sub item which should be retrieved
     * @param err The error code variable to store the result of the operation on error.
     *
     * @return An optional containing the retrieved sub-item or std::nullopt on error. Returns std::nullopt on error and sets err.
     *
     * @see WUPSStorageAPI_GetSubItem
     * @note There is a variant of the GetSubItem function which throws a std::runtime_error exception if an error occurs while
     * retrieving a sub-item
    **/
    std::optional<WUPSStorageSubItem> GetSubItem(std::string_view key, WUPSStorageError &err) const noexcept;

    /**
     * @brief Retrieve a sub-item of this sub-item.
     *
     * This function is a convenience wrapper around WUPSStorageAPI::GetSubItem for retrieving a sub-item of this sub-item
     *
     * @param key The key of the sub item which should be retrieved
     *
     * @throw std::runtime_error if no sub-item with the given key was found.
     *
     * @return An optional containing the retrieved sub-item or std::nullopt on error. Returns std::nullopt on error and sets err.
     *
     * @see WUPSStorageAPI_GetSubItem
     *
     * @note This function has a non-throwing variation. This function will return a std::null_opt rather than throwing
     * a std::runtime_error exception when sub-item for a given key was not found.
    **/
    [[nodiscard]] WUPSStorageSubItem GetSubItem(std::string_view key) const;

    /**
     * @brief Retrieves or creates a sub-item with the given key in this sub-item. (non-throwing)
     *
     * This function retrieves an existing sub-item with the specified key if it exists.
     * If the sub-item does not exist, it creates a new sub-item with the given key.
     *
     * @param key The key of the sub-item to retrieve or create.
     * @param[out] err The error code variable to store the result of the operation on error.
     *
     * @return An optional containing the retrieved or newly created sub-item. Returns std::nullopt on error and sets err.
     *
     * @see WUPSStorageSubItem
     *
     * @note There is a variant of the GetOrCreateSubItem function which throws a std::runtime_error exception if an error
     */
    std::optional<WUPSStorageSubItem> GetOrCreateSubItem(std::string_view key, WUPSStorageError &err) noexcept;

    /**
     * @brief Retrieves or creates a sub-item with the given key in this sub-item.
     *
     * This function retrieves an existing sub-item with the specified key if it exists.
     * If the sub-item does not exist, it creates a new sub-item with the given key.
     *
     * @param key The key of the sub-item to retrieve or create.
     *
     * @throw std::runtime_error if creating or retrieving the sub-item for the given key failed.
     *
     * @return Returns the retrieved or newly created sub-item, throws exception on error.
     *
     * @see WUPSStorageSubItem
     *
     * @note This function has a non-throwing variation. This function will return a std::null_opt rather than throwing
     * a std::runtime_error exception on error.
    **/
    [[nodiscard]] WUPSStorageSubItem GetOrCreateSubItem(std::string_view key);

private:
    wups_storage_item mHandle = {};
};

#endif
