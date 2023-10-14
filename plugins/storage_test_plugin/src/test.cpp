#include "catch2/catch_test_macros.hpp"
#include "utils/logger.h"
#include "utils/utils.h"
#include <coreinit/ios.h>
#include <vector>
#include <wups.h>

extern const char wups_meta_storage_id[];

static std::string pluginPathCached;
static std::string getPluginConfigPath() {
    if (!pluginPathCached.empty()) {
        return pluginPathCached;
    }
    char environmentPath[0x100];
    memset(environmentPath, 0, sizeof(environmentPath));

    auto handle = IOS_Open("/dev/mcp", IOS_OPEN_READ);
    if (handle >= 0) {
        int in = 0xF9; // IPC_CUSTOM_COPY_ENVIRONMENT_PATH
        if (IOS_Ioctl(handle, 100, &in, sizeof(in), environmentPath, sizeof(environmentPath)) != IOS_ERROR_OK) {
            return "fs:/vol/external01/wiiu/plugins/config";
        }

        IOS_Close(handle);
    }
    pluginPathCached = std::string(environmentPath).append("/plugins/config");
    return pluginPathCached;
};

struct CleanUpStorage {
    CleanUpStorage() {
        WUPS_CloseStorage();
        auto path = string_format("%s/%s.json", getPluginConfigPath().c_str(), &wups_meta_storage_id[0] + strlen("storage_id="));
        struct stat s {};
        if (stat(path.c_str(), &s) != 0) {
            // File doesn't exist
            return;
        }
        remove(path.c_str());
    }
    ~CleanUpStorage() = default;
};

template<typename T>
WUPSStorageError WUPS_Get(wups_storage_item parent, const char *key, T &outValue);

template<>
WUPSStorageError WUPS_Get<int32_t>(wups_storage_item parent, const char *key, int32_t &outValue) {
    return WUPS_GetS32(parent, key, &outValue);
}

template<>
WUPSStorageError WUPS_Get<int64_t>(wups_storage_item parent, const char *key, int64_t &outValue) {
    return WUPS_GetS64(parent, key, &outValue);
}

template<>
WUPSStorageError WUPS_Get<uint32_t>(wups_storage_item parent, const char *key, uint32_t &outValue) {
    return WUPS_GetU32(parent, key, &outValue);
}

template<>
WUPSStorageError WUPS_Get<uint64_t>(wups_storage_item parent, const char *key, uint64_t &outValue) {
    return WUPS_GetU64(parent, key, &outValue);
}

template<>
WUPSStorageError WUPS_Get<bool>(wups_storage_item parent, const char *key, bool &outValue) {
    return WUPS_GetBool(parent, key, &outValue);
}

template<>
WUPSStorageError WUPS_Get<float>(wups_storage_item parent, const char *key, float &outValue) {
    return WUPS_GetFloat(parent, key, &outValue);
}

template<>
WUPSStorageError WUPS_Get<double>(wups_storage_item parent, const char *key, double &outValue) {
    return WUPS_GetDouble(parent, key, &outValue);
}

template<>
WUPSStorageError WUPS_Get<std::vector<uint8_t>>(wups_storage_item parent, const char *key, std::vector<uint8_t> &outValue) {
    uint32_t outSize = 0;

    if (outValue.empty()) {
        uint32_t resizeToSize = 0;
        auto r                = WUPS_GetItemSize(parent, key, &resizeToSize);
        if (r == WUPS_STORAGE_ERROR_SUCCESS) {
            outValue.resize(resizeToSize);
        } else {
            return r;
        }
    }

    auto res = WUPS_GetBinary(parent, key, outValue.data(), outValue.size(), &outSize);
    if (res == WUPS_STORAGE_ERROR_SUCCESS) {
        outValue.resize(outSize);
    } else {
        outValue.resize(0);
    }
    return res;
}

template<>
WUPSStorageError WUPS_Get<std::string>(wups_storage_item parent, const char *key, std::string &outValue) {
    uint32_t outSize = 0;
    if (outValue.size() == 0) {
        uint32_t resizeToSize = 0;
        auto r                = WUPS_GetItemSize(parent, key, &resizeToSize);
        if (r == WUPS_STORAGE_ERROR_SUCCESS) {
            outValue.resize(resizeToSize);
        } else {
            return r;
        }
    }

    auto res = WUPS_GetString(parent, key, outValue.data(), outValue.size(), &outSize);
    if (res == WUPS_STORAGE_ERROR_SUCCESS) {
        // outSize does count the null terminator as well, std::string's size() doesn't include a null terminator.
        outValue.resize(strlen(outValue.c_str()));
    } else {
        outValue.resize(0);
    }
    return res;
}

template<typename T>
WUPSStorageError WUPS_Store(wups_storage_item parent, const char *key, const T &outValue);

template<>
WUPSStorageError WUPS_Store<int32_t>(wups_storage_item parent, const char *key, const int32_t &outValue) {
    return WUPS_StoreS32(parent, key, outValue);
}

template<>
WUPSStorageError WUPS_Store<int64_t>(wups_storage_item parent, const char *key, const int64_t &outValue) {
    return WUPS_StoreS64(parent, key, outValue);
}

template<>
WUPSStorageError WUPS_Store<uint32_t>(wups_storage_item parent, const char *key, const uint32_t &outValue) {
    return WUPS_StoreU32(parent, key, outValue);
}

template<>
WUPSStorageError WUPS_Store<uint64_t>(wups_storage_item parent, const char *key, const uint64_t &outValue) {
    return WUPS_StoreU64(parent, key, outValue);
}

template<>
WUPSStorageError WUPS_Store<bool>(wups_storage_item parent, const char *key, const bool &outValue) {
    return WUPS_StoreBool(parent, key, outValue);
}

template<>
WUPSStorageError WUPS_Store<float>(wups_storage_item parent, const char *key, const float &outValue) {
    return WUPS_StoreFloat(parent, key, outValue);
}

template<>
WUPSStorageError WUPS_Store<double>(wups_storage_item parent, const char *key, const double &outValue) {
    return WUPS_StoreDouble(parent, key, outValue);
}

template<>
WUPSStorageError WUPS_Store<std::vector<uint8_t>>(wups_storage_item parent, const char *key, const std::vector<uint8_t> &outValue) {
    return WUPS_StoreBinary(parent, key, outValue.data(), outValue.size());
}

template<>
WUPSStorageError WUPS_Store<std::string>(wups_storage_item parent, const char *key, const std::string &outValue) {
    return WUPS_StoreString(parent, key, outValue.c_str());
}

template<typename T>
bool isEqual(const T &v1, const T &v2) {
    return v1 == v2;
}

template<typename T>
bool isEqual(const std::vector<T> &v1, const std::vector<T> &v2) {
    if (v1.size() != v2.size()) {
        return false;
    }

    for (size_t i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
void TestStorageItem(const char *key, const T &expectedVal) {
    CleanUpStorage cleanup;
    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    T val  = {};
    auto r = WUPS_Get<T>(nullptr, key, val);
    REQUIRE(r == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(isEqual(val, T()));

    r = WUPS_Store<T>(nullptr, key, expectedVal);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);

    T readValue = {};
    r           = WUPS_Get<T>(nullptr, key, readValue);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(readValue, expectedVal));

    // let's close storage to write to sd card
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // Open it back!
    res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    readValue = {};
    r         = WUPS_Get<T>(nullptr, key, readValue);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(readValue, expectedVal));

    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Empty config: Get SubItem fails") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    wups_storage_item subItem   = nullptr;
    auto r                      = WUPS_GetSubItem(nullptr, subItemName, &subItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(subItem == nullptr);

    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Empty config: Get getitem fails") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "item";
    int readRes                 = 0;
    auto r                      = WUPS_GetS32(nullptr, subItemName, &readRes);
    REQUIRE(r == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readRes == 0);

    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Create S32 item") {
    constexpr auto *key = "int32_t";
    int32_t expectedVal = 0x7FFFFFFF;
    TestStorageItem<int32_t>(key, expectedVal);
}

TEST_CASE("Create S64 item") {
    constexpr auto *key = "int64_t";
    int64_t expectedVal = 0x7FFFFFFFFFFFFFFFL;
    TestStorageItem<int64_t>(key, expectedVal);
}

TEST_CASE("Create negative S32 item") {
    constexpr auto *key = "int32_t_negative";
    int32_t expectedVal = -0x7FFFFFFF;
    TestStorageItem<int32_t>(key, expectedVal);
}

TEST_CASE("Create negative S64 item") {
    constexpr auto *key = "int64_t_negative";
    int64_t expectedVal = -0x7FFFFFFFFFFFFFFF;
    TestStorageItem<int64_t>(key, expectedVal);
}

TEST_CASE("Create U32 item") {
    constexpr auto *key  = "uint32_t";
    uint32_t expectedVal = 0x80000001;
    TestStorageItem<uint32_t>(key, expectedVal);
}

TEST_CASE("Create U64 item") {
    constexpr auto *key  = "uint64_t";
    uint64_t expectedVal = 0x8000000000000001L;
    TestStorageItem<uint64_t>(key, expectedVal);
}

TEST_CASE("Create bool item") {
    constexpr auto *key = "bool";
    bool expectedVal    = true;
    TestStorageItem<bool>(key, expectedVal);
}

TEST_CASE("Create float item") {
    constexpr auto *key = "float";
    float expectedVal   = 0.1234567890123456789f;
    TestStorageItem<float>(key, expectedVal);
}

TEST_CASE("Create double item") {
    constexpr auto *key = "double";
    double expectedVal  = 0.1234567890123456789;
    TestStorageItem<double>(key, expectedVal);
}

TEST_CASE("Create binary") {
    constexpr auto *key              = "binary";
    std::vector<uint8_t> expectedVal = {1, 5, 4, 7, 8, 6};
    TestStorageItem<std::vector<uint8_t>>(key, expectedVal);
}

TEST_CASE("Create string") {
    constexpr auto *key     = "string";
    std::string expectedVal = "This is a test!";
    TestStorageItem<std::string>(key, expectedVal);
}

template<typename T>
void CheckItemSize(const char *key, const T &expectedVal, const uint32_t expectedItemSize) {
    CleanUpStorage cleanup;
    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPS_Store(nullptr, key, expectedVal);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    uint32_t itemSize = 0;
    res               = WUPS_GetItemSize(nullptr, key, &itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(itemSize == expectedItemSize);

    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    itemSize = 0;
    res      = WUPS_GetItemSize(nullptr, key, &itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(itemSize == expectedItemSize);

    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Test get size with string") {
    constexpr auto *key       = "string";
    std::string expectedVal   = "This is a test!";
    uint32_t expectedItemSize = strlen(expectedVal.c_str()) + 1;

    CheckItemSize(key, expectedVal, expectedItemSize);
}

TEST_CASE("Test get size with binary") {
    constexpr auto *key              = "binary";
    std::vector<uint8_t> expectedVal = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint32_t expectedItemSize        = expectedVal.size();

    CheckItemSize(key, expectedVal, expectedItemSize);
}

TEST_CASE("Test getSize fails with other type") {
    CleanUpStorage cleanup;
    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *key = "test";
    res                 = WUPS_Store<int32_t>(nullptr, key, 0);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    uint32_t itemSize = 0;
    res               = WUPS_GetItemSize(nullptr, key, &itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE);
    REQUIRE(itemSize == 0);

    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Delete non-existent item should fail") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPS_DeleteItem(nullptr, "non_existent");
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Create subitem") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    wups_storage_item subItem   = nullptr;
    auto r                      = WUPS_CreateSubItem(nullptr, subItemName, &subItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem != nullptr);

    wups_storage_item readItem = nullptr;
    r                          = WUPS_GetSubItem(nullptr, subItemName, &readItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem == readItem);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // Parse data from json and try to read it
    res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    readItem = nullptr;
    r        = WUPS_GetSubItem(nullptr, subItemName, &readItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(readItem != nullptr);
}

TEST_CASE("Create nested subitems") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto NEST_DEEP_SIZE = 10;

    wups_storage_item parentSubItem = nullptr;
    for (int i = 0; i < NEST_DEEP_SIZE; i++) {
        auto subItemKey           = string_format("subItem_%d", (i + 1));
        wups_storage_item subItem = nullptr;
        auto r                    = WUPS_CreateSubItem(parentSubItem, subItemKey.c_str(), &subItem);
        REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
        REQUIRE(subItem != nullptr);

        r = WUPS_StoreS32(subItem, "value", i);
        REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);

        parentSubItem = subItem;
    }

    parentSubItem = nullptr;
    for (int i = 0; i < NEST_DEEP_SIZE; i++) {
        auto subItemKey           = string_format("subItem_%d", (i + 1));
        wups_storage_item subItem = nullptr;
        auto r                    = WUPS_GetSubItem(parentSubItem, subItemKey.c_str(), &subItem);
        REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
        REQUIRE(subItem != nullptr);

        int readValue = -1;
        r             = WUPS_GetS32(subItem, "value", &readValue);
        REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
        REQUIRE(readValue == i);

        parentSubItem = subItem;
    }

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // Parse data from json and try to read it
    res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    parentSubItem = nullptr;
    for (int i = 0; i < NEST_DEEP_SIZE; i++) {
        auto subItemKey           = string_format("subItem_%d", (i + 1));
        wups_storage_item subItem = nullptr;
        auto r                    = WUPS_GetSubItem(parentSubItem, subItemKey.c_str(), &subItem);
        REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
        REQUIRE(subItem != nullptr);

        int readValue = -1;
        r             = WUPS_GetS32(subItem, "value", &readValue);
        REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
        REQUIRE(readValue == i);

        parentSubItem = subItem;
    }
}

TEST_CASE("Create and then delete subitem") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    wups_storage_item subItem   = nullptr;
    auto r                      = WUPS_CreateSubItem(nullptr, subItemName, &subItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem != nullptr);

    r = WUPS_DeleteItem(nullptr, subItemName);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);

    wups_storage_item readSubItem = nullptr;
    r                             = WUPS_GetSubItem(nullptr, subItemName, &readSubItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readSubItem == nullptr);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Create delete subitem with writes") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    wups_storage_item subItem   = nullptr;
    auto r                      = WUPS_CreateSubItem(nullptr, subItemName, &subItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem != nullptr);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // read from json
    res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    r = WUPS_DeleteItem(nullptr, subItemName);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);

    wups_storage_item readSubItem = nullptr;
    r                             = WUPS_GetSubItem(nullptr, subItemName, &readSubItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readSubItem == nullptr);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // read from json
    res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    readSubItem = nullptr;
    r           = WUPS_GetSubItem(nullptr, subItemName, &readSubItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readSubItem == nullptr);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Create two sub item with same key fails") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    wups_storage_item subItem   = nullptr;
    auto r                      = WUPS_CreateSubItem(nullptr, subItemName, &subItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem != nullptr);

    subItem = nullptr;
    r       = WUPS_CreateSubItem(nullptr, subItemName, &subItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_ALREADY_EXISTS);
    REQUIRE(subItem == nullptr);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Updating value of item by key works") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";
    int32_t value            = 0x42424242;
    res                      = WUPS_Store<int32_t>(nullptr, itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int32_t newValue = 0x42424243;
    res              = WUPS_Store<int32_t>(nullptr, itemName, newValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int32_t readValue = 0;
    res               = WUPS_Get<int32_t>(nullptr, itemName, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(readValue == newValue);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Changing type of item works") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::string value = "This is just a random string and no binary data";
    res               = WUPS_Store<std::string>(nullptr, itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    float floatValue = 1337.4242f;
    res              = WUPS_Store<float>(nullptr, itemName, floatValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    float readValue = 0.0f;
    res             = WUPS_Get<float>(nullptr, itemName, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(readValue == floatValue);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Get (size) works after changing type to binary") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    int32_t value = 12345678;
    res           = WUPS_Store<int32_t>(nullptr, itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    uint32_t itemSize = 0;
    res               = WUPS_GetItemSize(nullptr, itemName, &itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE);
    REQUIRE(itemSize == 0);

    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    uint32_t binaryDataSize         = binaryData.size();
    res                             = WUPS_Store<std::vector<uint8_t>>(nullptr, itemName, binaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    itemSize = 0;
    res      = WUPS_GetItemSize(nullptr, itemName, &itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(itemSize == binaryDataSize);

    std::vector<uint8_t> readBinary;
    res = WUPS_Get<std::vector<uint8_t>>(nullptr, itemName, readBinary);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(binaryData, readBinary));

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Change type from binary to int causes get size to fail") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    res                             = WUPS_Store<std::vector<uint8_t>>(nullptr, itemName, binaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int32_t value = 12345678;
    res           = WUPS_Store<int32_t>(nullptr, itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    uint32_t itemSize = 0;
    res               = WUPS_GetItemSize(nullptr, itemName, &itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE);
    REQUIRE(itemSize == 0);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Create item with same key as sub-item fails") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName  = "item";
    wups_storage_item subItem = nullptr;
    res                       = WUPS_CreateSubItem(nullptr, itemName, &subItem);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int32_t value = 0;
    res           = WUPS_Store<int32_t>(nullptr, itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_ALREADY_EXISTS);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Create sub-item with same key as item fails") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    int32_t value = 0;
    res           = WUPS_Store<int32_t>(nullptr, itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    wups_storage_item subItem = nullptr;
    res                       = WUPS_CreateSubItem(nullptr, itemName, &subItem);
    REQUIRE(res == WUPS_STORAGE_ERROR_ALREADY_EXISTS);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Make sure WUPS_GetSubItem only checks parent") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    wups_storage_item subItem   = nullptr;
    auto r                      = WUPS_CreateSubItem(nullptr, subItemName, &subItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem != nullptr);

    constexpr auto *subItemName2 = "subItem2";
    wups_storage_item subItem2   = nullptr;
    r                            = WUPS_CreateSubItem(subItem, subItemName2, &subItem2);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem2 != nullptr);

    // Make sure we can't get subItem2 from root.
    wups_storage_item subItem2FromRoot = nullptr;
    r                                  = WUPS_GetSubItem(nullptr, subItemName2, &subItem2FromRoot);
    REQUIRE(r == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(subItem2FromRoot == nullptr);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Make sure WUPS_GetItem only checks parent") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    wups_storage_item subItem   = nullptr;
    auto r                      = WUPS_CreateSubItem(nullptr, subItemName, &subItem);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem != nullptr);

    int32_t value            = 1337;
    constexpr auto *itemName = "item";
    r                        = WUPS_Store<int32_t>(subItem, itemName, value);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);

    // Make sure we can't get item from root.
    int32_t readValue = 0;
    r                 = WUPS_Get<int32_t>(nullptr, itemName, readValue);
    REQUIRE(r == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readValue == 0);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Store string and load it as binary fails") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::string value = "This is just a random string and no binary data";
    res               = WUPS_Store<std::string>(nullptr, itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::vector<uint8_t> readBinary;
    res = WUPS_Get<std::vector<uint8_t>>(nullptr, itemName, readBinary);
    REQUIRE(res == WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Get binary fails if buffer is too small") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    res                             = WUPS_Store<std::vector<uint8_t>>(nullptr, itemName, binaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::vector<uint8_t> readBinaryData;
    REQUIRE(binaryData.size() > 0);
    readBinaryData.resize(binaryData.size() - 1); // Make the buffer just a bit to small
    res = WUPS_Get<std::vector<uint8_t>>(nullptr, itemName, readBinaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Get binary works with exact buffer size") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    res                             = WUPS_Store<std::vector<uint8_t>>(nullptr, itemName, binaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::vector<uint8_t> readBinaryData;
    REQUIRE(binaryData.size() > 0);
    readBinaryData.resize(binaryData.size()); // Make the buffer just a bit to small
    res = WUPS_Get<std::vector<uint8_t>>(nullptr, itemName, readBinaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(binaryData, readBinaryData));

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}


TEST_CASE("Get string fails if buffer is too small") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::string strData = "Random string I just need for this test.";
    res                 = WUPS_Store<std::string>(nullptr, itemName, strData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::string readStr;
    REQUIRE(strData.size() > 0);
    // Make the buffer just a bit to small
    readStr.resize(strData.length() - 1);
    res = WUPS_Get<std::string>(nullptr, itemName, readStr);
    REQUIRE(res == WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Get string works with exact buffer size") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::string strData = "Random string I just need for this test.";
    res                 = WUPS_Store<std::string>(nullptr, itemName, strData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::string readStr;
    REQUIRE(strData.size() > 0);
    // We need to add one byte because of the null terminator
    readStr.resize(strData.length() + 1);
    res = WUPS_Get<std::string>(nullptr, itemName, readStr);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(strData, readStr));

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Create two sub-items, storing in first still works") {
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    wups_storage_item subItem1  = nullptr;
    res                         = WUPS_CreateSubItem(nullptr, subItem1Key, &subItem1);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1 != nullptr);

    // create subItem2
    constexpr auto *subItem2Key = "subItem2";
    wups_storage_item subItem2  = nullptr;
    res                         = WUPS_CreateSubItem(nullptr, subItem2Key, &subItem2);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem2 != nullptr);

    constexpr auto *itemIn1Key = "item";
    int32_t value              = 13371337;
    res                        = WUPS_Store(subItem1, itemIn1Key, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int readValue = 0;
    res           = WUPS_Get(subItem1, itemIn1Key, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(value == readValue);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Create sub-item with closed storage fails") {
    // make sure storage is closed
    CleanUpStorage cleanup;

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    wups_storage_item subItem1  = nullptr;
    auto res                    = WUPS_CreateSubItem(nullptr, subItem1Key, &subItem1);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(subItem1 == nullptr);
}

TEST_CASE("Get sub item with closed storage fails") {
    // make sure storage is closed
    CleanUpStorage cleanup;

    // create subItem1
    constexpr auto *itemKey    = "subItem";
    wups_storage_item subItem1 = nullptr;
    auto res                   = WUPS_GetSubItem(nullptr, itemKey, &subItem1);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(subItem1 == 0);
}

TEST_CASE("Get item with closed storage fails") {
    // make sure storage is closed
    CleanUpStorage cleanup;

    // create subItem1
    constexpr auto *itemKey = "item";
    int32_t value           = 0;
    auto res                = WUPS_Get(nullptr, itemKey, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(value == 0);
}

TEST_CASE("Store item with closed storage fails") {
    // make sure storage is closed
    CleanUpStorage cleanup;

    // create subItem1
    constexpr auto *itemKey = "item";
    int32_t value           = 13371337;
    auto res                = WUPS_Store(nullptr, itemKey, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
}

TEST_CASE("get item size with closed storage fails") {
    // make sure storage is closed
    CleanUpStorage cleanup;

    // create subItem1
    constexpr auto *itemKey = "item";
    uint32_t itemSize       = 0;
    auto res                = WUPS_GetItemSize(nullptr, itemKey, &itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
}


TEST_CASE("Create nested sub items, delete top one, accessing items of nested one should fail") {
    // make sure storage is closed
    CleanUpStorage cleanup;


    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    wups_storage_item subItem1  = nullptr;
    res                         = WUPS_CreateSubItem(nullptr, subItem1Key, &subItem1);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1 != nullptr);

    // create subItem2
    constexpr auto *subItem1_1Key = "subItem1.1";
    wups_storage_item subItem1_1  = nullptr;
    res                           = WUPS_CreateSubItem(subItem1, subItem1_1Key, &subItem1_1);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1_1 != nullptr);

    // create subItem1
    constexpr auto *itemKey = "item_in_1_1";
    int32_t value           = 13371337;
    res                     = WUPS_Store(subItem1_1, itemKey, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // delete subItem1
    res = WUPS_DeleteItem(nullptr, subItem1Key);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int readValue = 0;
    res           = WUPS_Get(subItem1_1, itemKey, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("Store item into deleted sub-item should fail") {
    // make sure storage is closed
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    wups_storage_item subItem1  = nullptr;
    res                         = WUPS_CreateSubItem(nullptr, subItem1Key, &subItem1);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1 != nullptr);

    // delete subItem1
    res = WUPS_DeleteItem(nullptr, subItem1Key);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *itemKey = "item_in_1";
    int32_t value           = 13371337;
    res                     = WUPS_Store(subItem1, itemKey, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}

TEST_CASE("create sub-item of deleted sub-item should fail") {
    // make sure storage is closed
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    wups_storage_item subItem1  = nullptr;
    res                         = WUPS_CreateSubItem(nullptr, subItem1Key, &subItem1);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1 != nullptr);

    // delete subItem1
    res = WUPS_DeleteItem(nullptr, subItem1Key);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1_1
    constexpr auto *subItem1_1Key = "subItem1.1";
    wups_storage_item subItem1_1  = nullptr;
    res                           = WUPS_CreateSubItem(subItem1, subItem1_1Key, &subItem1_1);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(subItem1 != nullptr);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}


TEST_CASE("Load stored item from deleted sub-item should fail") {
    // make sure storage is closed
    CleanUpStorage cleanup;

    auto res = WUPS_OpenStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    wups_storage_item subItem1  = nullptr;
    res                         = WUPS_CreateSubItem(nullptr, subItem1Key, &subItem1);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1 != nullptr);

    // create subItem1
    constexpr auto *itemKey = "item_in_1";
    int32_t value           = 13371337;
    res                     = WUPS_Store(subItem1, itemKey, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // delete subItem1
    res = WUPS_DeleteItem(nullptr, subItem1Key);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // try to read value from deleted sub item
    int readValue = 0;
    res           = WUPS_Get(subItem1, itemKey, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);

    // Close to write the data to the storage
    res = WUPS_CloseStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
}