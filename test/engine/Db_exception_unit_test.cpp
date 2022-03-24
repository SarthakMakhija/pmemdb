#include <gtest/gtest.h>
#include <string>
#include "./DbFixture.h"

using namespace pmem::storage;

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenKeyIsBlankWhilePutting) {
    ASSERT_THROW(DbFixture::getDb()->put("", "Hard disk drive"), std::invalid_argument);
}

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenValueIsBlankWhilePutting) {
    ASSERT_THROW(DbFixture::getDb()->put("HDD", ""), std::invalid_argument);
}

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenKeyIsBlankWhileUpdating) {
    ASSERT_THROW(DbFixture::getDb()->update("", "Hard disk drive"), std::invalid_argument);
}

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenValueIsBlank) {
    ASSERT_THROW(DbFixture::getDb()->update("HDD", ""), std::invalid_argument);
}

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenMaxPairsIs0) {
    ASSERT_THROW(DbFixture::getDb()->scan("HDD", "RAM", 0), std::invalid_argument);
}

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenMaxPairsIsLessThan0) {
    ASSERT_THROW(DbFixture::getDb()->scan("HDD", "RAM", -5), std::invalid_argument);
}

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenBeginAndEndKeyIsSameInScan) {
    ASSERT_THROW(DbFixture::getDb()->scan("A", "A", 10), std::invalid_argument);
}

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenBeginKeyIsLessThanEndKeyInScan) {
    ASSERT_THROW(DbFixture::getDb()->scan("B", "A", 10), std::invalid_argument);
}