#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipList.h"
#include "./PersistentMemoryPoolFixture.h"

TEST_F(PersistentMemoryPoolFixture, ThrowInvalidArgumentExceptionGivenTowerSizeIsZero) {
    ASSERT_THROW(new SkipList(0), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, ThrowInvalidArgumentExceptionGivenTowerSizeIsLessThanZero) {
    ASSERT_THROW(new SkipList(-1), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, ThrowInvalidArgumentExceptionGivenKeyIsBlankWhilePutting) {
    SkipList* skipList = new SkipList(5);
    
    ASSERT_THROW(skipList -> put("", "Hard disk drive"), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, ThrowInvalidArgumentExceptionGivenValueIsBlankWhilePutting) {
    SkipList* skipList = new SkipList(5);
    
    ASSERT_THROW(skipList -> put("HDD", ""), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, ThrowInvalidArgumentExceptionGivenKeyIsBlankWhileUpdating) {
    SkipList* skipList = new SkipList(5);
    
    ASSERT_THROW(skipList -> update("", "Hard disk drive"), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, ThrowInvalidArgumentExceptionGivenValueIsBlank) {
    SkipList* skipList = new SkipList(5);
    
    ASSERT_THROW(skipList -> update("HDD", ""), std::invalid_argument);
}


TEST_F(PersistentMemoryPoolFixture, ThrowAnExceptionWhilePuttingAnExistingKey) {
    SkipList* skipList = new SkipList(5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    ASSERT_THROW(skipList -> put("HDD", "Hard disk"), std::invalid_argument);
}