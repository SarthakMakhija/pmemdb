#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipList.h"

TEST(SkipListException, ThrowInvalidArgumentExceptionGivenTowerSizeIsZero) {
    ASSERT_THROW(new SkipList(0), std::invalid_argument);
}

TEST(SkipListException, ThrowInvalidArgumentExceptionGivenTowerSizeIsLessThanZero) {
    ASSERT_THROW(new SkipList(-1), std::invalid_argument);
}

TEST(SkipListException, ThrowInvalidArgumentExceptionGivenKeyIsBlank) {
    SkipList* skipList = new SkipList(5);
    
    ASSERT_THROW(skipList -> put("", "Hard disk drive"), std::invalid_argument);
}

TEST(SkipListException, ThrowInvalidArgumentExceptionGivenValueIsBlank) {
    SkipList* skipList = new SkipList(5);
    
    ASSERT_THROW(skipList -> put("HDD", ""), std::invalid_argument);
}