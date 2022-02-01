#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipList.h"
#include "./PersistentMemoryPoolFixture.h"

TEST_F(PersistentMemoryPoolFixture, SkipListUnit_CreateASkipListAndGetAValueByKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    pair<string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_EQ("Hard disk drive", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListUnit_CreateASkipListAndGetExistenceOfKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    pair<string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_TRUE(existenceByValue.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListUnit_CreateASkipListAndGetAValueByNonExistentKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    pair<string, bool> existenceByValue = skipList -> get("SDD");
    
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListUnit_CreateASkipListAndGetTheExistenceOfNonExistentKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    pair<string, bool> existenceByValue = skipList -> get("SDD");
    
    ASSERT_FALSE(existenceByValue.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListUnit_CreateASkipListAndUpdateAValue) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> update("HDD", "Hard disk");
    pair<string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_EQ("Hard disk", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListUnit_CreateASkipListAndUpdateAValueForNonExistingKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> update("SDD", "Solid state drive");
    pair<string, bool> existenceByValue = skipList -> get("SDD");
    ASSERT_EQ("", existenceByValue.first);
}

/*
TEST_F(PersistentMemoryPoolFixture, SkipListUnit_CreateASkipListAndDeleteByAKeyInTheBeginning) {
    SkipList* skipList = new SkipList(1);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> deleteBy("HDD");
    pair<string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListUnit_CreateASkipListAndDeleteByAKeyInBetween) {
    SkipList* skipList = new SkipList(1);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");
    skipList -> put("SDD", "Solid state drive");

    skipList -> deleteBy("Pmem");
    pair<string, bool> existenceByValue = skipList -> get("Pmem");
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListUnit_CreateASkipListAndDeleteByAKeyInTheEnd) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> deleteBy("Pmem");
    pair<string, bool> existenceByValue = skipList -> get("Pmem");
    ASSERT_EQ("", existenceByValue.first);
}
*/