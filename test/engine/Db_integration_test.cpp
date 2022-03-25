#include <gtest/gtest.h>
#include <string>
#include "./DbFixture.h"
#include "../../src/engine/Db.h"

using namespace pmem::storage;

TEST_F(DbFixture, DbIntegration_GetAValueByKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    std::pair<std::string, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_EQ("Hard disk drive", existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_GetExistenceOfKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    std::pair<std::string, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_TRUE(existenceByValue.second);
}

TEST_F(DbFixture, DbIntegration_GetAValueByNonExistentKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    std::pair<std::string, bool> existenceByValue = DbFixture::getDb() -> get("SDD");

    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_GetTheExistenceOfNonExistingKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    std::pair<std::string, bool> existenceByValue = DbFixture::getDb() -> get("SDD");

    ASSERT_FALSE(existenceByValue.second);
}

TEST_F(DbFixture, DbIntegration_DoesMultiGet) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");
    DbFixture::getDb()->put("SDD", "Solid state drive");

    std::vector<std::string> keys = {"HDD", "SDD", "Pmem", "DoesNotExist"};
    std::vector<std::pair<std::string, bool>> result = DbFixture::getDb() -> multiGet(keys);
    std::vector<std::pair<std::string, bool>> expected = {
                            std::make_pair("", false),
                            std::make_pair("Hard disk drive", true),
                            std::make_pair("Persistent Memory", true),
                            std::make_pair("Solid state drive", true)
    };

    ASSERT_EQ(expected, result);
}

TEST_F(DbFixture, DbIntegration_ScanWithBeginKeyPresent) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");
    DbFixture::getDb()->put("SDD", "Solid state drive");
    DbFixture::getDb() -> put("RAM", "Random access memory");

    std::string beginKey = "Pmem";
    std::string endKey = "SDD";

    std::vector<KeyValuePair> pairs = DbFixture::getDb() -> scan(beginKey.c_str(), endKey.c_str(), 10);
    std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent Memory"), KeyValuePair("RAM", "Random access memory")};

    ASSERT_EQ(expected, pairs);
}

TEST_F(DbFixture, DbIntegration_ScanWithBeginKeyNotPresent) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("SDD", "Solid state drive");
    DbFixture::getDb() -> put("RAM", "Random access memory");

    std::string beginKey = "Pmem";
    std::string endKey = "SDD";

    std::vector<KeyValuePair> pairs = DbFixture::getDb() -> scan(beginKey.c_str(), endKey.c_str(), 10);
    std::vector<KeyValuePair> expected = {KeyValuePair("RAM", "Random access memory")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(DbFixture, DbIntegration_UpdateAValue) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    DbFixture::getDb() -> update("HDD", "Hard disk");
    std::pair<std::string, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_EQ("Hard disk", existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_UpdateAValueForNonExistingKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    DbFixture::getDb() -> update("SDD", "Solid state drive");

    std::pair<std::string, bool> existenceByValue = DbFixture::getDb() -> get("SDD");
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInTheBeginning) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    DbFixture::getDb() -> deleteBy("HDD");

    std::pair<std::string, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInBetween) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");
    DbFixture::getDb()->put("SDD", "Solid state drive");

    DbFixture::getDb() -> deleteBy("Pmem");

    std::pair<std::string, bool> existenceByValue = DbFixture::getDb() -> get("Pmem");
    ASSERT_EQ("", existenceByValue.first);
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInTheEnd) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    DbFixture::getDb() -> deleteBy("Pmem");

    std::pair<std::string, bool> existenceByValue = DbFixture::getDb() -> get("Pmem");
    ASSERT_EQ("", existenceByValue.first);
}