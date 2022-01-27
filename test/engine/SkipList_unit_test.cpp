#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListNode.h"
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

TEST(SkipList, CreateASkipListAndUpdateTheValueOfAnExistentingKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    skipList -> put("HDD", "Hard disk");
    pair<string, bool> existenceByValue = skipList -> get("HDD");
    
    ASSERT_EQ("Hard disk", existenceByValue.first);
}

