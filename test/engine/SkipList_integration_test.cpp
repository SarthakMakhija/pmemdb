#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipList.h"
#include "./PersistentMemoryPoolFixture.h"

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndAdd500KeyValuePairsInSkipListWithTowerSize10) {
    SkipList* skipList = new SkipList(10);
    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count);

        skipList -> put(key, value);
    }
    for (int count = 1; count <= 500; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        std::string expectedValue = "Value-" + std::to_string(count);
        
        std::pair<std::string, bool> existenceByValue = skipList -> get(key);
        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndAdd500KeyValuePairsInSkipListWithTowerSize1) {
    SkipList* skipList = new SkipList(1);
    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count);

        skipList -> put(key, value);
    }
    for (int count = 1; count <= 500; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        std::string expectedValue = "Value-" + std::to_string(count);
        
        std::pair<std::string, bool> existenceByValue = skipList -> get(key);
        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndUpdate500KeyValuePairsUsingUpdateInSkipListWithTowerSize10) {
    SkipList* skipList = new SkipList(10);
    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count);

        skipList -> put(key, value);
    }
    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count*2);

        skipList -> update(key, value);
    }
    for (int count = 1; count <= 500; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        std::string expectedValue = "Value-" + std::to_string(count*2);
        
        std::pair<std::string, bool> existenceByValue = skipList -> get(key);
        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndDeleteKeys) {
    SkipList* skipList = new SkipList(10);
    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count);

        skipList -> put(key, value);
    }

    skipList -> deleteBy("Key-"    + std::to_string(1));
    for (int count = 500; count <= 400; count++) {
        std::string key   = "Key-"    + std::to_string(count);

        skipList -> deleteBy(key);
    }

    for (int count = 2; count < 400; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        std::string expectedValue = "Value-" + std::to_string(count);
        
        std::pair<std::string, bool> existenceByValue = skipList -> get(key);

        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }

    ASSERT_FALSE(skipList -> get("Key-"    + std::to_string(1)).second);
    for (int count = 500; count <= 400; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        
        std::pair<std::string, bool> existenceByValue = skipList -> get(key);

        ASSERT_EQ("", existenceByValue.first);
        ASSERT_FALSE(existenceByValue.second);
    }
}