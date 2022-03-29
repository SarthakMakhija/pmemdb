#include <gtest/gtest.h>
#include <string>
#include "./DbFixture.h"
#include "../../src/engine/Db.h"

using namespace pmem::storage;

TEST_F(DbFixture, DbIntegration_GetAValueByKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_EQ("Hard disk drive", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_GetExistenceOfKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_TRUE(existenceByValue.second);
}

TEST_F(DbFixture, DbIntegration_GetAValueByNonExistentKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("SDD");

    ASSERT_EQ("", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_GetTheExistenceOfNonExistingKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("SDD");

    ASSERT_FALSE(existenceByValue.second);
}

TEST_F(DbFixture, DbIntegration_DoesMultiGet) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");
    DbFixture::getDb()->put("SDD", "Solid state drive");

    std::vector<const char*> keys = {"HDD", "SDD", "Pmem", "DoesNotExist"};
    std::vector<std::pair<std::string, bool>> expected = {
                            std::make_pair("", false),
                            std::make_pair("Hard disk drive", true),
                            std::make_pair("Persistent Memory", true),
                            std::make_pair("Solid state drive", true)
    };
    std::vector<std::pair<const char*, bool>> result = DbFixture::getDb() -> multiGet(keys);
    std::vector<std::pair<std::string, bool>> resultTransformed;

    for (auto pair : result) {
        resultTransformed.push_back(std::make_pair(std::string(pair.first), pair.second));
    }
    ASSERT_EQ(expected, resultTransformed);
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
    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_EQ("Hard disk", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_UpdateAValueForNonExistingKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    DbFixture::getDb() -> update("SDD", "Solid state drive");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("SDD");
    ASSERT_EQ("", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInTheBeginning) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    DbFixture::getDb() -> deleteBy("HDD");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_EQ("", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInBetween) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");
    DbFixture::getDb()->put("SDD", "Solid state drive");

    DbFixture::getDb() -> deleteBy("Pmem");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("Pmem");
    ASSERT_EQ("", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInTheEnd) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    DbFixture::getDb() -> deleteBy("Pmem");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("Pmem");
    ASSERT_EQ("", std::string(existenceByValue.first));
}