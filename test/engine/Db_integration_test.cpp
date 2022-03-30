#include <gtest/gtest.h>
#include <string>
#include "./DbFixture.h"
#include "../../src/engine/Db.h"

using namespace pmem::storage;

void put(Db *db, const char* key, const char* value) {
    db->put(key, value);
}

void update(Db *db, const char* key, const char* value) {
    db->update(key, value);
}

void deleteBy(Db *db, const char* key) {
    db->deleteBy(key);
}

TEST_F(DbFixture, DbIntegration_GetAValueByKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_EQ("Hard disk drive", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_GetExistenceOfKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_TRUE(existenceByValue.second);
}

TEST_F(DbFixture, DbIntegration_GetAValueByNonExistentKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("SDD");

    ASSERT_EQ("", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_GetTheExistenceOfNonExistingKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("SDD");

    ASSERT_FALSE(existenceByValue.second);
}

TEST_F(DbFixture, DbIntegration_DoesMultiGet) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid state drive");

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
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid state drive");
    put(DbFixture::getDb(), "RAM", "Random access memory");

    std::string beginKey = "Pmem";
    std::string endKey = "SDD";

    std::vector<KeyValuePair> pairs = DbFixture::getDb() -> scan(beginKey.c_str(), endKey.c_str(), 10);
    std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent Memory"), KeyValuePair("RAM", "Random access memory")};

    ASSERT_EQ(expected, pairs);
}

TEST_F(DbFixture, DbIntegration_ScanWithBeginKeyNotPresent) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "SDD", "Solid state drive");
    put(DbFixture::getDb(), "RAM", "Random access memory");

    std::string beginKey = "Pmem";
    std::string endKey = "SDD";

    std::vector<KeyValuePair> pairs = DbFixture::getDb() -> scan(beginKey.c_str(), endKey.c_str(), 10);
    std::vector<KeyValuePair> expected = {KeyValuePair("RAM", "Random access memory")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(DbFixture, DbIntegration_UpdateAValue) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    update(DbFixture::getDb(), "HDD", "Hard disk");
    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_EQ("Hard disk", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_UpdateAValueForNonExistingKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    (DbFixture::getDb(), "SDD", "Solid state drive");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("SDD");
    ASSERT_EQ("", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInTheBeginning) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    deleteBy(DbFixture::getDb(), "HDD");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("HDD");
    ASSERT_EQ("", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInBetween) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid state drive");

    deleteBy(DbFixture::getDb(), "Pmem");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("Pmem");
    ASSERT_EQ("", std::string(existenceByValue.first));
}

TEST_F(DbFixture, DbIntegration_DeleteByAKeyInTheEnd) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    deleteBy(DbFixture::getDb(), "Pmem");

    std::pair<const char*, bool> existenceByValue = DbFixture::getDb() -> get("Pmem");
    ASSERT_EQ("", std::string(existenceByValue.first));
}