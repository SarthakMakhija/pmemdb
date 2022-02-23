#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipList.h"
#include "./PersistentMemoryPoolFixture.h"

TEST_F(PersistentMemoryPoolFixture, SkipListFunctional_CreateASkipListAndAdd500KeyValuePairsInSkipList) {
    SkipList* skipList = new SkipList(10, 0.5);
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

TEST_F(PersistentMemoryPoolFixture, SkipListFunctional_CreateASkipListAndDoesAScan) {
    SkipList* skipList = new SkipList(10, 0.5);
    for (int count = 1; count <= 500; count+=2) {
        std::string key   =  std::to_string(count);
        std::string value =  std::to_string(count);

        skipList -> put(key, value);
    }    
    std::string beginKey = "50";
    std::string endKey   = "70";

    std::vector<KeyValuePair> expected;
    for (int count = 51; count < 70; count+=2) {
        expected.push_back(KeyValuePair(std::to_string(count), std::to_string(count)));
    }
    expected.push_back(KeyValuePair("7", "7"));

    std::vector<KeyValuePair> pairs = skipList -> scan(beginKey, endKey, 50);
    ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListFunctional_CreateASkipListAndDoesAScanWithMaxPairsAs5) {
    SkipList* skipList = new SkipList(10, 0.5);
    for (int count = 1; count <= 500; count+=2) {
        std::string key   =  std::to_string(count);
        std::string value =  std::to_string(count);

        skipList -> put(key, value);
    }    
    std::string beginKey = "50";
    std::string endKey   = "70";

    std::vector<KeyValuePair> expected;
    for (int count = 51; count <= 59; count+=2) {
        expected.push_back(KeyValuePair(std::to_string(count), std::to_string(count)));
    }

    std::vector<KeyValuePair> pairs = skipList -> scan(beginKey, endKey, 5);
    ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListFunctional_CreateASkipListAndUpdate500KeyValuePairsUsingUpdateInSkipList) {
    SkipList* skipList = new SkipList(10, 0.5);
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

TEST_F(PersistentMemoryPoolFixture, SkipListFunctional_CreateASkipListAndDeleteKeys) {
    SkipList* skipList = new SkipList(10, 0.5);
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

/*
TEST_F(PersistentMemoryPoolFixture, SkipListFunctional_CreateASkipListAndDeleteAllKeysByRange) {
    SkipList* skipList = new SkipList(10, 0.5);
    for (int count = 1; count <= 8; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count);

        skipList -> put(key, value);
    }

    std::string beginKey = "Key-"    + std::to_string(1);
    std::string endKey   = "Key-"    + std::to_string(9);

    skipList -> deleteRange(beginKey, endKey);

    for (int count = 1; count <= 8; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::pair<std::string, bool> existenceByValue = skipList -> get(key);

        ASSERT_EQ("", existenceByValue.first);
        ASSERT_FALSE(existenceByValue.second);
    }
}
*/