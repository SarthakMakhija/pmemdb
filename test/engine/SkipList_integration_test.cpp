#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipList.h"


TEST(SkipListIntegration, CreateASkipListAndAdd1000KeyValuePairsInSkipListWithTowerSize10) {
    SkipList* skipList = new SkipList(10);
    for (int count = 1; count <= 1000; count++) {
        string key   = "Key-"    + std::to_string(count);
        string value = "Value-"  + std::to_string(count);

        skipList -> put(key, value);
    }
    for (int count = 1; count <= 1000; count++) {
        string key           = "Key-"   + std::to_string(count);
        string expectedValue = "Value-" + std::to_string(count);
        
        pair<string, bool> existenceByValue = skipList -> get(key);
        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }
}

TEST(SkipListIntegration, CreateASkipListAndAdd1000KeyValuePairsInSkipListWithTowerSize1) {
    SkipList* skipList = new SkipList(1);
    for (int count = 1; count <= 1000; count++) {
        string key   = "Key-"    + std::to_string(count);
        string value = "Value-"  + std::to_string(count);

        skipList -> put(key, value);
    }
    for (int count = 1; count <= 1000; count++) {
        string key           = "Key-"   + std::to_string(count);
        string expectedValue = "Value-" + std::to_string(count);
        
        pair<string, bool> existenceByValue = skipList -> get(key);
        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }
}

TEST(SkipListIntegration, CreateASkipListAndUpdate1000KeyValuePairsUsingUpdateInSkipListWithTowerSize10) {
    SkipList* skipList = new SkipList(10);
    for (int count = 1; count <= 1000; count++) {
        string key   = "Key-"    + std::to_string(count);
        string value = "Value-"  + std::to_string(count);

        skipList -> put(key, value);
    }
    for (int count = 1; count <= 1000; count++) {
        string key   = "Key-"    + std::to_string(count);
        string value = "Value-"  + std::to_string(count*2);

        skipList -> update(key, value);
    }
    for (int count = 1; count <= 1000; count++) {
        string key           = "Key-"   + std::to_string(count);
        string expectedValue = "Value-" + std::to_string(count*2);
        
        pair<string, bool> existenceByValue = skipList -> get(key);
        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }
}

TEST(SkipListIntegration, CreateASkipListAndDeleteKeys) {
    SkipList* skipList = new SkipList(10);
    for (int count = 1; count <= 1000; count++) {
        string key   = "Key-"    + std::to_string(count);
        string value = "Value-"  + std::to_string(count);

        skipList -> put(key, value);
    }

    skipList -> deleteBy("Key-"    + std::to_string(1));
    for (int count = 900; count <= 1000; count++) {
        string key   = "Key-"    + std::to_string(count);

        skipList -> deleteBy(key);
    }

    for (int count = 2; count < 900; count++) {
        string key           = "Key-"   + std::to_string(count);
        string expectedValue = "Value-" + std::to_string(count);
        
        pair<string, bool> existenceByValue = skipList -> get(key);
        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }

    ASSERT_FALSE(skipList -> get("Key-"    + std::to_string(1)).second);
    for (int count = 900; count <= 1000; count++) {
        string key           = "Key-"   + std::to_string(count);
        
        pair<string, bool> existenceByValue = skipList -> get(key);
        ASSERT_FALSE(existenceByValue.second);
    }
}