#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include "../DbFixture.h"
#include "../DbTestsHelper.h"
#include "../../../src/engine/db/Db.h"

using namespace pmem::storage;

TEST_F(DbFixture, DbIntegration_GetAValueByKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    std::pair<Slice, bool> existenceByValue = DbFixture::getDb() -> get(Slice("HDD"));
    ASSERT_EQ(Slice("Hard disk drive"), existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_GetExistenceOfKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    std::pair<Slice, bool> existenceByValue = DbFixture::getDb() -> get(Slice("HDD"));
    ASSERT_TRUE(existenceByValue.second);
}

TEST_F(DbFixture, DbIntegration_GetAValueByNonExistentKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    std::pair<Slice, bool> existenceByValue = DbFixture::getDb() -> get(Slice("SDD"));

    ASSERT_EQ(Slice(""), existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_GetTheExistenceOfNonExistingKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    std::pair<Slice, bool> existenceByValue = DbFixture::getDb() -> get(Slice("SDD"));

    ASSERT_FALSE(existenceByValue.second);
}

TEST_F(DbFixture, DbIntegration_DoesMultiGet) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid state drive");

    std::vector<Slice> keys = {Slice("HDD"), Slice("SDD"), Slice("Pmem"), Slice("DoesNotExist")};
    std::vector<std::pair<std::string, bool>> expected = {
                            std::make_pair("", false),
                            std::make_pair("Hard disk drive", true),
                            std::make_pair("Persistent Memory", true),
                            std::make_pair("Solid state drive", true)
    };
    std::vector<std::pair<const char*, bool>> result = DbFixture::getDb() -> multiGet(keys);
    std::vector<std::pair<std::string, bool>> resultTransformed(result.size());
    std::transform(result.begin(), result.end(), resultTransformed.begin(), [](std::pair<const char*, bool> pair) {
        return std::make_pair(std::string(pair.first), pair.second);
    });

    ASSERT_EQ(expected, resultTransformed);
}

TEST_F(DbFixture, DbIntegration_ScanWithBeginKeyPresent) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid state drive");
    put(DbFixture::getDb(), "RAM", "Random access memory");

    Slice beginKey = Slice("Pmem");
    Slice endKey = Slice("SDD");

    std::vector<KeyValuePair> pairs = DbFixture::getDb() -> scan(beginKey, endKey, 10);
    std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent Memory"), KeyValuePair("RAM", "Random access memory")};

    ASSERT_EQ(expected, pairs);
}

TEST_F(DbFixture, DbIntegration_ScanWithBeginKeyNotPresent) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "SDD", "Solid state drive");
    put(DbFixture::getDb(), "RAM", "Random access memory");

    Slice beginKey = Slice("Pmem");
    Slice endKey = Slice("SDD");

    std::vector<KeyValuePair> pairs = DbFixture::getDb() -> scan(beginKey, endKey, 10);
    std::vector<KeyValuePair> expected = {KeyValuePair("RAM", "Random access memory")};

    ASSERT_EQ(expected, pairs);
}

TEST_F(DbFixture, DbIntegration_UpdateAValue) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    update(DbFixture::getDb(), "HDD", "Hard disk");
    std::pair<Slice, bool> existenceByValue = DbFixture::getDb() -> get(Slice("HDD"));
    ASSERT_EQ(Slice("Hard disk"), existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_UpdateAValueForNonExistingKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    update(DbFixture::getDb(), "SDD", "Solid state drive");

    std::pair<Slice, bool> existenceByValue = DbFixture::getDb() -> get(Slice("SDD"));
    ASSERT_EQ(Slice(""), existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInTheBeginning) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    deleteBy(DbFixture::getDb(), "HDD");

    std::pair<Slice, bool> existenceByValue = DbFixture::getDb() -> get(Slice("HDD"));
    ASSERT_EQ(Slice(""), existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInBetween) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid state drive");

    deleteBy(DbFixture::getDb(), "Pmem");

    std::pair<Slice, bool> existenceByValue = DbFixture::getDb() -> get(Slice("Pmem"));
    ASSERT_EQ(Slice(""), existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInTheEnd) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    deleteBy(DbFixture::getDb(), "Pmem");

    std::pair<Slice, bool> existenceByValue = DbFixture::getDb() -> get(Slice("Pmem"));
    ASSERT_EQ(Slice(""), existenceByValue.first);
}