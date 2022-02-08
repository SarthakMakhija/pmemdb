#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipList.h"
#include "./PersistentMemoryPoolFixture.h"


TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndGetAValueByKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    std::pair<std::string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_EQ("Hard disk drive", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndGetExistenceOfKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    std::pair<std::string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_TRUE(existenceByValue.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndGetAValueByNonExistentKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    std::pair<std::string, bool> existenceByValue = skipList -> get("SDD");
    
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndGetTheExistenceOfNonExistentKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    std::pair<std::string, bool> existenceByValue = skipList -> get("SDD");
    
    ASSERT_FALSE(existenceByValue.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndDoesMultiGet) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");
    skipList -> put("SDD", "Solid state drive");

    std::vector<std::string> keys = {"HDD", "SDD", "Pmem", "DoesNotExist"};
    std::vector<std::pair<std::string, bool>> result = skipList -> multiGet(keys);
    std::vector<std::pair<std::string, bool>> expected = {
                            std::make_pair("Hard disk drive", true), 
                            std::make_pair("Solid state drive", true),
                            std::make_pair("Persistent Memory", true),
                            std::make_pair("", false)
    };
    
    ASSERT_EQ(expected, result);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_ScanWithBeginKeyPresent) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");
    skipList -> put("SDD", "Solid state drive");
    skipList -> put("RAM", "Random access memory");

    std::string beginKey = "Pmem";
    std::string endKey = "SDD";

    std::vector<KeyValuePair> pairs = skipList -> scan(beginKey, endKey, 10);
    std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent Memory"), KeyValuePair("RAM", "Random access memory")};
    
    ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_ScanWithBeginKeyNotPresent) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("SDD", "Solid state drive");
    skipList -> put("RAM", "Random access memory");

    std::string beginKey = "Pmem";
    std::string endKey = "SDD";

    std::vector<KeyValuePair> pairs = skipList -> scan(beginKey, endKey, 10);
    std::vector<KeyValuePair> expected = {KeyValuePair("RAM", "Random access memory")};
    
  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndUpdateAValue) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> update("HDD", "Hard disk");
    std::pair<std::string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_EQ("Hard disk", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndUpdateAValueForNonExistingKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> update("SDD", "Solid state drive");
    std::pair<std::string, bool> existenceByValue = skipList -> get("SDD");
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndDeleteByAKeyInTheBeginning) {
    SkipList* skipList = new SkipList(1);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> deleteBy("HDD");
    std::pair<std::string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndDeleteByAKeyInBetween) {
    SkipList* skipList = new SkipList(1);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");
    skipList -> put("SDD", "Solid state drive");

    skipList -> deleteBy("Pmem");
    std::pair<std::string, bool> existenceByValue = skipList -> get("Pmem");
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndDeleteByAKeyInTheEnd) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> deleteBy("Pmem");
    std::pair<std::string, bool> existenceByValue = skipList -> get("Pmem");
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndDeleteRangeWithBeginKeyPresent) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("A", "A");
    skipList -> put("B", "B");
    skipList -> put("C", "C");
    skipList -> put("D", "D");
    skipList -> put("E", "E");

    skipList -> deleteRange("B", "D");
    
    std::vector<std::string> missingKeys = {"B", "C"};
    for (auto missingKey: missingKeys) {
        ASSERT_EQ("",  skipList -> get(missingKey).first);
    }

    std::vector<std::string> presentKeys = {"A", "D", "E"};
    for (auto presentKey: presentKeys) {
        ASSERT_EQ(presentKey, skipList -> get(presentKey).first);
    }
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndDeleteRangeWithBeginKeyNotPresent) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("A", "A");
    skipList -> put("B", "B");
    skipList -> put("D", "D");
    skipList -> put("E", "E");

    skipList -> deleteRange("C", "E");
    
    std::vector<std::string> missingKeys = {"D"};
    for (auto missingKey: missingKeys) {
        ASSERT_EQ("",  skipList -> get(missingKey).first);
    }

    std::vector<std::string> presentKeys = {"A", "B", "E"};
    for (auto presentKey: presentKeys) {
        ASSERT_EQ(presentKey, skipList -> get(presentKey).first);
    }
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndDeleteRangeWithEndKeyNotPresent) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("A", "A");
    skipList -> put("B", "B");
    skipList -> put("C", "C");
    skipList -> put("D", "D");
    skipList -> put("E", "E");

    skipList -> deleteRange("B", "F");
    
    std::vector<std::string> missingKeys = {"B", "C", "D", "E"};
    for (auto missingKey: missingKeys) {
        ASSERT_EQ("",  skipList -> get(missingKey).first);
    }

    std::vector<std::string> presentKeys = {"A"};
    for (auto presentKey: presentKeys) {
        ASSERT_EQ(presentKey, skipList -> get(presentKey).first);
    }
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndDeleteRangeWithBeginAndEndKeyNotPresent) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("A", "A");
    skipList -> put("C", "C");
    skipList -> put("D", "D");
    skipList -> put("E", "E");

    skipList -> deleteRange("B", "F");
    
    std::vector<std::string> missingKeys = {"C", "D", "E"};
    for (auto missingKey: missingKeys) {
        ASSERT_EQ("",  skipList -> get(missingKey).first);
    }

    std::vector<std::string> presentKeys = {"A"};
    for (auto presentKey: presentKeys) {
        ASSERT_EQ(presentKey, skipList -> get(presentKey).first);
    }
}

TEST_F(PersistentMemoryPoolFixture, SkipListIntegration_CreateASkipListAndDeleteRangeAll) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("A", "A");
    skipList -> put("B", "B");
    skipList -> put("C", "C");
    skipList -> put("D", "D");
    skipList -> put("E", "E");

    skipList -> deleteRange("A", "F");
    
    std::vector<std::string> missingKeys = {"A", "B", "C", "D", "E"};
    for (auto missingKey: missingKeys) {
        ASSERT_EQ("",  skipList -> get(missingKey).first);
    }
}