#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipList.h"
#include "./PersistentMemoryPoolFixture.h"

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenTowerSizeIsZero) {
    ASSERT_THROW(new SkipList(0, 0.5), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenTowerSizeIsLessThanZero) {
    ASSERT_THROW(new SkipList(-1, 0.5), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenKeyIsBlankWhilePutting) {
    SkipList* skipList = new SkipList(5, 0.5);
    
    ASSERT_THROW(skipList -> put("", "Hard disk drive"), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenValueIsBlankWhilePutting) {
    SkipList* skipList = new SkipList(5, 0.5);
    
    ASSERT_THROW(skipList -> put("HDD", ""), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenKeyIsBlankWhileUpdating) {
    SkipList* skipList = new SkipList(5, 0.5);
    
    ASSERT_THROW(skipList -> update("", "Hard disk drive"), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenValueIsBlank) {
    SkipList* skipList = new SkipList(5, 0.5);
    
    ASSERT_THROW(skipList -> update("HDD", ""), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenMaxPairsIs0) {
    SkipList* skipList = new SkipList(5, 0.5);
    
    ASSERT_THROW(skipList -> scan("HDD", "RAM", 0), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenMaxPairsIsLessThan0) {
    SkipList* skipList = new SkipList(5, 0.5);
    
    ASSERT_THROW(skipList -> scan("HDD", "RAM", -5), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenBeginAndEndKeyIsSameInScan) {
    SkipList* skipList = new SkipList(5, 0.5);
    
    ASSERT_THROW(skipList -> scan("A", "A", 10), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenBeginKeyIsLessThanEndKeyInScan) {
    SkipList* skipList = new SkipList(5, 0.5);
    
    ASSERT_THROW(skipList -> scan("B", "A", 10), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenBeginAndEndKeyIsSameInDeleteRange) {
    SkipList* skipList = new SkipList(5,  0.5);
    
    ASSERT_THROW(skipList -> deleteRange("A", "A"), std::invalid_argument);
}

TEST_F(PersistentMemoryPoolFixture, SkipListException_ThrowInvalidArgumentExceptionGivenBeginKeyIsLessThanEndKeyInDeleteRange) {
    SkipList* skipList = new SkipList(5, 0.5);
    
    ASSERT_THROW(skipList -> deleteRange("B", "A"), std::invalid_argument);
}