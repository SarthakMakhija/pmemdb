#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipList.h"

TEST(SkipList, CreateASkipListAndGetAValueByKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    pair<string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_EQ("Hard disk drive", existenceByValue.first);
}

TEST(SkipList, CreateASkipListAndGetExistenceOfKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    pair<string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_TRUE(existenceByValue.second);
}

TEST(SkipList, CreateASkipListAndGetAValueByNonExistentKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    pair<string, bool> existenceByValue = skipList -> get("SDD");
    
    ASSERT_EQ("", existenceByValue.first);
}

TEST(SkipList, CreateASkipListAndGetTheExistenceOfNonExistentKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    pair<string, bool> existenceByValue = skipList -> get("SDD");
    
    ASSERT_FALSE(existenceByValue.second);
}

TEST(SkipList, CreateASkipAndThrowAnExceptionWhilePuttingAnExistingKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    ASSERT_THROW(skipList -> put("HDD", "Hard disk"), std::invalid_argument);
}

TEST(SkipList, CreateASkipListAndUpdateAValue) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> update("HDD", "Hard disk");
    pair<string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_EQ("Hard disk", existenceByValue.first);
}

TEST(SkipList, CreateASkipListAndUpdateAValueForNonExistingKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> update("SDD", "Solid state drive");
    pair<string, bool> existenceByValue = skipList -> get("SDD");
    ASSERT_EQ("", existenceByValue.first);
}

TEST(SkipList, CreateASkipListAndDeleteByAKeyInTheBeginning) {
    SkipList* skipList = new SkipList(1);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> deleteBy("HDD");
    pair<string, bool> existenceByValue = skipList -> get("HDD");
    ASSERT_EQ("", existenceByValue.first);
}

TEST(SkipList, CreateASkipListAndDeleteByAKeyInBetween) {
    SkipList* skipList = new SkipList(1);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");
    skipList -> put("SDD", "Solid state drive");

    skipList -> deleteBy("Pmem");
    pair<string, bool> existenceByValue = skipList -> get("Pmem");
    ASSERT_EQ("", existenceByValue.first);
}

TEST(SkipList, CreateASkipListAndDeleteByAKeyInTheEnd) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> deleteBy("Pmem");
    pair<string, bool> existenceByValue = skipList -> get("Pmem");
    ASSERT_EQ("", existenceByValue.first);
}