#include <gtest/gtest.h>
#include <string>
#include "../DbFixture.h"
#include "../DbTestsHelper.h"

using namespace pmem::storage;

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenKeyIsBlankWhilePutting) {
    GTEST_SKIP();
    ASSERT_THROW(put(DbFixture::getDb(), "", "Hard disk drive"), std::invalid_argument);
}

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenValueIsBlankWhilePutting) {
    GTEST_SKIP();
    ASSERT_THROW(put(DbFixture::getDb(), "HDD", ""), std::invalid_argument);
}

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenKeyIsBlankWhileUpdating) {
    GTEST_SKIP();
    ASSERT_THROW(update(DbFixture::getDb(), "", "Hard disk drive"), std::invalid_argument);
}

TEST_F(DbFixture, DbException_ThrowInvalidArgumentExceptionGivenValueIsBlank) {
    GTEST_SKIP();
    ASSERT_THROW(update(DbFixture::getDb(), "HDD", ""), std::invalid_argument);
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