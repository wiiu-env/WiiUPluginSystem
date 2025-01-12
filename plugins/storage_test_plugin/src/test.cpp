#include "utils/logger.h"
#include "utils/utils.h"

#include <wups/storage.h>

#include <catch2/catch_amalgamated.hpp>

#include <vector>

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
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    T val  = {};
    auto r = WUPSStorageAPI::Get<T>(key, val);
    REQUIRE(r == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(isEqual(val, T()));

    r = WUPSStorageAPI::Store<T>(key, expectedVal);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);

    T readValue = {};
    r           = WUPSStorageAPI::Get<T>(key, readValue);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(readValue, expectedVal));

    // let's close storage to write to sd card
    res = WUPSStorageAPI::SaveStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPSStorageAPI::ForceReloadStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    readValue = {};
    r         = WUPSStorageAPI::Get<T>(key, readValue);
    REQUIRE(r == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(readValue, expectedVal));
}

TEST_CASE("Empty config: Get SubItem fails") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    WUPSStorageError err;
    auto subItem = WUPSStorageAPI::GetSubItem(subItemName, err);
    REQUIRE(err == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(!subItem);
}

TEST_CASE("Empty config: Get getitem fails") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "item";
    int readRes                 = 0;
    auto r                      = WUPSStorageAPI::Get<int32_t>(subItemName, readRes);
    REQUIRE(r == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readRes == 0);
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
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPSStorageAPI::Store(key, expectedVal);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    uint32_t itemSize = 0;
    res               = WUPSStorageAPI::GetItemSize<T>(key, itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(itemSize == expectedItemSize);

    res = WUPSStorageAPI::SaveStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPSStorageAPI::ForceReloadStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    itemSize = 0;
    res      = WUPSStorageAPI::GetItemSize<T>(key, itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(itemSize == expectedItemSize);
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
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *key = "test";
    res                 = WUPSStorageAPI::Store<int32_t>(key, 0);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    uint32_t itemSize = 0;
    res               = WUPSStorageAPI::GetItemSize<std::string>(key, itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE);
    REQUIRE(itemSize == 0);

    itemSize = 0;
    res      = WUPSStorageAPI::GetItemSize<std::vector<uint8_t>>(key, itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE);
    REQUIRE(itemSize == 0);
}

TEST_CASE("Saving Base64 string as string works") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *key  = "test";
    std::string storeStr = "97bb3f3b0ec8755028e5cdf56eaf01b47140a6cd";
    res                  = WUPSStorageAPI::Store<std::string>(key, storeStr);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPSStorageAPI::SaveStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    res = WUPSStorageAPI::ForceReloadStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::string retValue;
    res = WUPSStorageAPI::Get(key, retValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(storeStr, retValue));
}

TEST_CASE("Delete non-existent item should fail") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPSStorageAPI::DeleteItem("non_existent");
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
}

TEST_CASE("Create subitem") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);


    constexpr auto *subItemName = "subItem";
    auto subItem                = WUPSStorageAPI::CreateSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem.has_value());

    auto readItem = WUPSStorageAPI::GetSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(readItem);
    REQUIRE(*readItem == *subItem);

    res = WUPSStorageAPI::SaveStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPSStorageAPI::ForceReloadStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    readItem = WUPSStorageAPI::GetSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(readItem);
}

void storeNestedSubItem(int NEST_DEEP_SIZE) {
    WUPSStorageError res;
    std::optional<WUPSStorageSubItem> subItem = WUPSStorageAPI::GetRootItem();
    REQUIRE(subItem);

    for (int i = 0; i < NEST_DEEP_SIZE; i++) {
        auto subItemKey = string_format("subItem_%d", (i + 1));
        subItem         = subItem->CreateSubItem(subItemKey, res);
        REQUIRE(subItem);
        REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

        res = subItem->Store<int32_t>("value", i);
        REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    }
}

void readNestedSubItem(int NEST_DEEP_SIZE) {
    WUPSStorageError res;
    std::optional<WUPSStorageSubItem> subItem = WUPSStorageAPI::GetRootItem();
    REQUIRE(subItem);

    for (int i = 0; i < NEST_DEEP_SIZE; i++) {
        auto subItemKey = string_format("subItem_%d", (i + 1));
        subItem         = subItem->GetSubItem(subItemKey, res);
        REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
        REQUIRE(subItem);

        int32_t readValue = -1;
        res               = subItem->Get("value", readValue);
        REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
        REQUIRE(readValue == i);
    }
}

/**
 * 1. Store "test" and make sure reading works
 * 2. Make sure get "test" failes
 * 3. Save storage
 * 4. Store "test2" and make sure reading both "test" and "test2" works
 * 5. Force reload
 * 6. Make sure reading "test2" failes, but "test" still works
 */
TEST_CASE("Test ForceReload") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemKey  = "test";
    constexpr auto *itemKey2 = "test2";

    // Write value and read it back
    constexpr int writeValue  = 0x13371337;
    constexpr int writeValue2 = 0x13371338;
    res                       = WUPSStorageAPI::Store(itemKey, writeValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int readValue = 0;
    res           = WUPSStorageAPI::Get(itemKey, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(readValue == writeValue);

    // Try to read a non-existing item
    readValue = 0;
    res       = WUPSStorageAPI::Get(itemKey2, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readValue == 0);

    // Force save
    res = WUPSStorageAPI::SaveStorage();

    // Write test 2 value and read it back
    res = WUPSStorageAPI::Store(itemKey2, writeValue2);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    readValue = 0;
    res       = WUPSStorageAPI::Get(itemKey2, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(readValue == writeValue2);

    res = WUPSStorageAPI::ForceReloadStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    readValue = 0;
    res       = WUPSStorageAPI::Get(itemKey, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(readValue == writeValue);

    readValue = 0;
    res       = WUPSStorageAPI::Get(itemKey2, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readValue == 0);
}

TEST_CASE("Create nested subitems") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto NEST_DEEP_SIZE = 10;

    storeNestedSubItem(NEST_DEEP_SIZE);

    readNestedSubItem(NEST_DEEP_SIZE);

    res = WUPSStorageAPI::SaveStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPSStorageAPI::ForceReloadStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    readNestedSubItem(NEST_DEEP_SIZE);
}

TEST_CASE("Check SaveStorage") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    WUPSStorageAPI::Store("test", 1);
    OSReport("Save after change\n");
    WUPSStorageAPI::SaveStorage(false);
    OSReport("Save after no change\n");
    WUPSStorageAPI::SaveStorage(false);
    WUPSStorageAPI::Store("test", 2);
    WUPSStorageAPI::Store("test", 1);
    OSReport("Save after no change\n");
    WUPSStorageAPI::SaveStorage(false);
}

TEST_CASE("Create and then delete subitem") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    auto subItem                = WUPSStorageAPI::CreateSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem);

    res = WUPSStorageAPI::DeleteItem(subItemName);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    subItem = WUPSStorageAPI::GetSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(!subItem);
}

TEST_CASE("Create delete subitem with writes") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    auto subItem                = WUPSStorageAPI::CreateSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem);

    res = WUPSStorageAPI::SaveStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPSStorageAPI::ForceReloadStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPSStorageAPI::DeleteItem(subItemName);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    auto readSubItem = WUPSStorageAPI::GetSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(!readSubItem);

    res = WUPSStorageAPI::SaveStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    res = WUPSStorageAPI::ForceReloadStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    readSubItem = WUPSStorageAPI::GetSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(!readSubItem);
}

TEST_CASE("Create two sub item with same key fails") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    auto subItem                = WUPSStorageAPI::CreateSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem);

    subItem = WUPSStorageAPI::CreateSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_ALREADY_EXISTS);
    REQUIRE(!subItem);
}

TEST_CASE("Updating value of item by key works") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";
    int32_t value            = 0x42424242;
    res                      = WUPSStorageAPI::Store<int32_t>(itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int32_t newValue = 0x42424243;
    res              = WUPSStorageAPI::Store<int32_t>(itemName, newValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int32_t readValue = 0;
    res               = WUPSStorageAPI::Get<int32_t>(itemName, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(readValue == newValue);
}

TEST_CASE("Changing type of item works") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::string value = "This is just a random string and no binary data";
    res               = WUPSStorageAPI::Store<std::string>(itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    float floatValue = 1337.4242f;
    res              = WUPSStorageAPI::Store<float>(itemName, floatValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    float readValue = 0.0f;
    res             = WUPSStorageAPI::Get<float>(itemName, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(readValue == floatValue);
}

TEST_CASE("Get (size) works after changing type to binary") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);


    constexpr auto *itemName = "item";

    int32_t value = 12345678;
    res           = WUPSStorageAPI::Store<int32_t>(itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    uint32_t itemSize = 0;
    res               = WUPSStorageAPI::GetItemSize<std::string>(itemName, itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE);
    REQUIRE(itemSize == 0);

    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    uint32_t binaryDataSize         = binaryData.size();
    res                             = WUPSStorageAPI::Store<std::vector<uint8_t>>(itemName, binaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    itemSize = 0;
    res      = WUPSStorageAPI::GetItemSize<std::vector<uint8_t>>(itemName, itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(itemSize == binaryDataSize);

    std::vector<uint8_t> readBinary;
    res = WUPSStorageAPI::Get<std::vector<uint8_t>>(itemName, readBinary);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(binaryData, readBinary));
}

TEST_CASE("Change type from binary to int causes get size to fail") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);


    constexpr auto *itemName = "item";

    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    res                             = WUPSStorageAPI::Store<std::vector<uint8_t>>(itemName, binaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int32_t value = 12345678;
    res           = WUPSStorageAPI::Store<int32_t>(itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    uint32_t itemSize = 0;
    res               = WUPSStorageAPI::GetItemSize<std::vector<uint8_t>>(itemName, itemSize);
    REQUIRE(res == WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE);
    REQUIRE(itemSize == 0);
}

TEST_CASE("Create item with same key as sub-item fails") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);


    constexpr auto *itemName = "item";
    auto subItem             = WUPSStorageAPI::CreateSubItem(itemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem);

    int32_t value = 0;
    res           = WUPSStorageAPI::Store(itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_ALREADY_EXISTS);
}

TEST_CASE("Create sub-item with same key as item fails") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    int32_t value = 0;
    res           = WUPSStorageAPI::Store(itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    auto subItem = WUPSStorageAPI::CreateSubItem(itemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_ALREADY_EXISTS);
}

TEST_CASE("Make sure WUPS_GetSubItem only checks parent") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    auto subItem                = WUPSStorageAPI::CreateSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem);

    constexpr auto *subItemName2 = "subItem2";
    auto subItem2                = subItem->CreateSubItem(subItemName2, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem2);

    // Make sure we can't get subItem2 from root.
    auto subItem2FromRoot = WUPSStorageAPI::GetSubItem(subItemName2, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(!subItem2FromRoot);
}

TEST_CASE("Make sure WUPS_GetItem only checks parent") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *subItemName = "subItem";
    auto subItem                = WUPSStorageAPI::CreateSubItem(subItemName, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem);

    int32_t value            = 1337;
    constexpr auto *itemName = "item";
    res                      = subItem->Store(itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // Make sure we can't get item from root.
    int32_t readValue = 0;
    res               = WUPSStorageAPI::Get<int32_t>(itemName, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readValue == 0);
}

TEST_CASE("Store string and load it as binary fails") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::string value = "This is just a random string and no binary data";
    res               = WUPSStorageAPI::Store<std::string>(itemName, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::vector<uint8_t> readBinary;
    res = WUPSStorageAPI::Get<std::vector<uint8_t>>(itemName, readBinary);
    REQUIRE(res == WUPS_STORAGE_ERROR_UNEXPECTED_DATA_TYPE);
}

TEST_CASE("USE_EXISTING_BUFFER: Get binary fails if buffer is too small") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    res                             = WUPSStorageAPI::Store<std::vector<uint8_t>>(itemName, binaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::vector<uint8_t> readBinaryData;
    REQUIRE(!binaryData.empty());
    readBinaryData.resize(binaryData.size() - 1); // Make the buffer just a bit to small
    res = WUPSStorageAPI::Get<std::vector<uint8_t>>(itemName, readBinaryData,
                                                    WUPSStorageAPI::GetOptions::USE_EXISTING_BUFFER);
    REQUIRE(res == WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL);
}

TEST_CASE("USE_EXISTING_BUFFER: Get binary works with exact buffer size") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    res                             = WUPSStorageAPI::Store<std::vector<uint8_t>>(itemName, binaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::vector<uint8_t> readBinaryData;
    REQUIRE(!binaryData.empty());
    readBinaryData.resize(binaryData.size());
    res = WUPSStorageAPI::Get<std::vector<uint8_t>>(itemName, readBinaryData,
                                                    WUPSStorageAPI::GetOptions::USE_EXISTING_BUFFER);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(binaryData, readBinaryData));
}

TEST_CASE("USE_EXISTING_BUFFER: Get binary works if buffer is too big") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    res                             = WUPSStorageAPI::Store<std::vector<uint8_t>>(itemName, binaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::vector<uint8_t> readBinaryData;
    REQUIRE(!binaryData.empty());
    readBinaryData.resize(binaryData.size() * 2);
    res = WUPSStorageAPI::Get<std::vector<uint8_t>>(itemName, readBinaryData,
                                                    WUPSStorageAPI::GetOptions::USE_EXISTING_BUFFER);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(binaryData, readBinaryData));
}

void test_binary_get_RESIZE_EXISTING_BUFFER_generic(std::vector<uint8_t> &binaryData, uint32_t buffer_size) {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    res = WUPSStorageAPI::Store<std::vector<uint8_t>>(itemName, binaryData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::vector<uint8_t> readBinaryData;
    REQUIRE(!binaryData.empty());
    readBinaryData.resize(buffer_size);
    res = WUPSStorageAPI::Get<std::vector<uint8_t>>(itemName, readBinaryData,
                                                    WUPSStorageAPI::GetOptions::RESIZE_EXISTING_BUFFER);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(binaryData, readBinaryData));
}

TEST_CASE("RESIZE_EXISTING_BUFFER: Get binary fails if buffer is empty") {
    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    test_binary_get_RESIZE_EXISTING_BUFFER_generic(binaryData, 0);
}

TEST_CASE("RESIZE_EXISTING_BUFFER: Get binary works with exact buffer size") {
    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    test_binary_get_RESIZE_EXISTING_BUFFER_generic(binaryData, binaryData.size());
}

TEST_CASE("RESIZE_EXISTING_BUFFER: Get binary works if buffer is too big") {
    std::vector<uint8_t> binaryData = {8, 4, 5, 4, 12, 4, 2};
    test_binary_get_RESIZE_EXISTING_BUFFER_generic(binaryData, binaryData.size() * 2);
}

TEST_CASE("USE_EXISTING_BUFFER: Get string fails if buffer is too small") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::string strData = "Random string I just need for this test.";
    res                 = WUPSStorageAPI::Store<std::string>(itemName, strData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::string readStr;
    REQUIRE(!strData.empty());
    // Make the buffer just a bit to small
    readStr.resize(strData.length() - 1);
    res = WUPSStorageAPI::Get<std::string>(itemName, readStr, WUPSStorageAPI::GetOptions::USE_EXISTING_BUFFER);
    REQUIRE(res == WUPS_STORAGE_ERROR_BUFFER_TOO_SMALL);
}

TEST_CASE("USE_EXISTING_BUFFER: Get string works with exact buffer size") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::string strData = "Random string I just need for this test.";
    res                 = WUPSStorageAPI::Store<std::string>(itemName, strData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::string readStr;
    REQUIRE(!strData.empty());
    // We need to add one byte because of the null terminator
    readStr.resize(strData.length() + 1);
    res = WUPSStorageAPI::Get<std::string>(itemName, readStr, WUPSStorageAPI::GetOptions::USE_EXISTING_BUFFER);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(strData, readStr));
}

TEST_CASE("USE_EXISTING_BUFFER: Get string works if buffer it too big.") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    std::string strData = "Random string I just need for this test.";
    res                 = WUPSStorageAPI::Store<std::string>(itemName, strData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::string readStr;
    REQUIRE(!strData.empty());
    // We need to add one byte because of the null terminator
    readStr.resize(strData.length() * 2);
    res = WUPSStorageAPI::Get<std::string>(itemName, readStr, WUPSStorageAPI::GetOptions::USE_EXISTING_BUFFER);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(strData, readStr));
}

void test_string_get_RESIZE_EXISTING_BUFFER_generic(std::string &strData, uint32_t buffer_size) {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    constexpr auto *itemName = "item";

    res = WUPSStorageAPI::Store<std::string>(itemName, strData);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    std::string readStr;
    REQUIRE(!strData.empty());

    readStr.resize(buffer_size);
    res = WUPSStorageAPI::Get<std::string>(itemName, readStr, WUPSStorageAPI::GetOptions::RESIZE_EXISTING_BUFFER);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(isEqual(strData, readStr));
}

TEST_CASE("RESIZE_EXISTING_BUFFER: Get string works if buffer is too small") {
    std::string strData = "Random string I just need for this test.";
    test_string_get_RESIZE_EXISTING_BUFFER_generic(strData, 0);
}

TEST_CASE("RESIZE_EXISTING_BUFFER: Get string works if buffer is perfect") {
    std::string strData = "Random string I just need for this test.";
    test_string_get_RESIZE_EXISTING_BUFFER_generic(strData, strData.length() + 1); //add null terminator
}

TEST_CASE("RESIZE_EXISTING_BUFFER: Get string works if buffer is too big") {
    std::string strData = "Random string I just need for this test.";
    test_string_get_RESIZE_EXISTING_BUFFER_generic(strData, strData.length() * 2);
}

TEST_CASE("Create two sub-items, storing in first still works") {
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    auto subItem1               = WUPSStorageAPI::CreateSubItem(subItem1Key, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1);

    // create subItem2
    constexpr auto *subItem2Key = "subItem2";
    auto subItem2               = WUPSStorageAPI::CreateSubItem(subItem2Key, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem2);

    constexpr auto *itemIn1Key = "item";
    int32_t value              = 13371337;
    res                        = subItem1->Store(itemIn1Key, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int readValue = 0;
    res           = subItem1->Get(itemIn1Key, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(value == readValue);
}

TEST_CASE("Create nested sub items, delete top one, accessing items of nested one should fail") {
    // make sure storage is closed
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    auto subItem1               = WUPSStorageAPI::CreateSubItem(subItem1Key, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1);

    // create subItem2
    constexpr auto *subItem1_1Key = "subItem1_1";
    auto subItem1_1               = subItem1->CreateSubItem(subItem1_1Key, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1_1);

    // Store into subItem1
    constexpr auto *itemKey = "item_in_1_1";
    int32_t value           = 13371337;
    res                     = subItem1_1->Store(itemKey, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // delete subItem1
    res = WUPSStorageAPI::DeleteItem(subItem1Key);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    int readValue = 0;
    res           = subItem1_1->Get(itemKey, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readValue == 0);
}

TEST_CASE("Store item into deleted sub-item should fail") {
    // make sure storage is closed
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    auto subItem1               = WUPSStorageAPI::CreateSubItem(subItem1Key, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1);

    // delete subItem1
    res = WUPSStorageAPI::DeleteItem(subItem1Key);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *itemKey = "item_in_1";
    int32_t value           = 13371337;
    res                     = subItem1->Store(itemKey, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
}

TEST_CASE("create sub-item of deleted sub-item should fail") {
    // make sure storage is closed
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    auto subItem1               = WUPSStorageAPI::CreateSubItem(subItem1Key, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1);

    // delete subItem1
    res = WUPSStorageAPI::DeleteItem(subItem1Key);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1_1
    constexpr auto *subItem1_1Key = "subItem1.1";
    auto subItem1_1               = subItem1->CreateSubItem(subItem1_1Key, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(!subItem1_1);
}

TEST_CASE("Load stored item from deleted sub-item should fail") {
    // make sure storage is closed
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // create subItem1
    constexpr auto *subItem1Key = "subItem1";
    auto subItem1               = WUPSStorageAPI::CreateSubItem(subItem1Key, res);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(subItem1);

    // create item_in_1
    constexpr auto *itemKey = "item_in_1";
    int32_t value           = 13371337;
    res                     = subItem1->Store(itemKey, value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // delete subItem1
    res = WUPSStorageAPI::DeleteItem(subItem1Key);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);

    // try to read value from deleted sub item
    int readValue = 0;
    res           = subItem1->Get(itemKey, readValue);
    REQUIRE(res == WUPS_STORAGE_ERROR_NOT_FOUND);
    REQUIRE(readValue == 0);
}

TEST_CASE("Test GetOrStoreDefault with no existing value") {
    // make sure storage is closed
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    std::string target        = "will be discarded";
    std::string default_value = "new_default";
    res                       = WUPSStorageAPI::GetOrStoreDefault("test", target, default_value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(target == default_value);
}

TEST_CASE("Test GetOrStoreDefault with existing value") {
    // make sure storage is closed
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    std::string original_value = "original_value";
    std::string target         = original_value;
    res                        = WUPSStorageAPI::Store<std::string>("test", target);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    std::string default_value = "new_default";
    res                       = WUPSStorageAPI::GetOrStoreDefault("test", target, default_value);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(target == original_value);
}

TEST_CASE("Store empty string works") {
    // make sure storage is closed
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    std::string empty_string;
    res = WUPSStorageAPI::Store("test", empty_string);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    std::string get_test = "_";
    res                  = WUPSStorageAPI::Get("test", get_test);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(get_test.empty());
}

TEST_CASE("Store empty binary works") {
    // make sure storage is closed
    auto res = WUPSStorageAPI::WipeStorage();
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    std::vector<uint8_t> empty_vector;
    res = WUPSStorageAPI::Store("test", empty_vector);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    std::vector<uint8_t> get_vector{0x13, 0x37};
    res = WUPSStorageAPI::Get("test", get_vector);
    REQUIRE(res == WUPS_STORAGE_ERROR_SUCCESS);
    REQUIRE(get_vector.empty());
}
