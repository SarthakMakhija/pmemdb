#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include "../DbFixture.h"
#include "../DbTestsHelper.h"
#include "../../../src/engine/db/Db.h"
#include "../../../src/engine/iterator/SkipListIterator.h"
#include <iostream>

using namespace pmem::storage;

TEST_F(DbFixture, SkipListIteratorIntegration_SeekToFirst) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    SkipListIterator iterator = DbFixture::getDb() -> newIterator();
    iterator.seekToFirst();
    iterator.next();

    ASSERT_EQ("HDD", std::string(iterator.key()));
}

TEST_F(DbFixture, SkipListIteratorIntegration_SeekToLast) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    SkipListIterator iterator = DbFixture::getDb() -> newIterator();
    iterator.seekToLast();

    ASSERT_EQ("Pmem", std::string(iterator.key()));
}

TEST_F(DbFixture, SkipListIteratorIntegration_Seek1) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator iterator = DbFixture::getDb() -> newIterator();
    iterator.seek("SDD");

    ASSERT_EQ("SDD", std::string(iterator.key()));
    ASSERT_EQ("Solid State drive", std::string(iterator.value()));
}

TEST_F(DbFixture, SkipListIteratorIntegration_Seek2) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator iterator = DbFixture::getDb() -> newIterator();
    iterator.seek("SDD");

    ASSERT_EQ("SDD", std::string(iterator.key()));
    ASSERT_EQ("Solid State drive", std::string(iterator.value()));

    iterator.seek("HDD");
    ASSERT_EQ("HDD", std::string(iterator.key()));
    ASSERT_EQ("Hard disk drive", std::string(iterator.value()));
}

TEST_F(DbFixture, SkipListIteratorIntegration_Next1) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator iterator = DbFixture::getDb() -> newIterator();
    iterator.next();

    ASSERT_EQ("HDD", std::string(iterator.key()));
}

TEST_F(DbFixture, SkipListIteratorIntegration_Next2) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator iterator = DbFixture::getDb() -> newIterator();
    iterator.next();
    iterator.next();

    ASSERT_EQ("Pmem", std::string(iterator.key()));
}

TEST_F(DbFixture, SkipListIteratorIntegration_Value) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator iterator = DbFixture::getDb() -> newIterator();
    iterator.next();

    ASSERT_EQ("Hard disk drive", std::string(iterator.value()));
}

TEST_F(DbFixture, SkipListIteratorIntegration_IsValid) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    SkipListIterator iterator = DbFixture::getDb() -> newIterator();
    ASSERT_TRUE(iterator.isValid());

    iterator.next();
    ASSERT_TRUE(iterator.isValid());

    iterator.next();
    ASSERT_TRUE(iterator.isValid());

    iterator.next();
    ASSERT_FALSE(iterator.isValid());
}