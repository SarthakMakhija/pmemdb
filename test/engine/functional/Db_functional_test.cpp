#include <gtest/gtest.h>
#include <string>
#include  <cstdio>

#include "../../../src/engine/Db.h"
#include "../../../src/engine/StringKeyComparator.h"
#include "../DbTestsHelper.h"

using namespace pmem::storage;

const char* filePath = "./functional.log";

Db* openDb(int skipListTowerSize = 100, double probability = 0.5) {
    Configuration configuration = Configuration(filePath,
                                                8 * 1024 * 1024,
                                                skipListTowerSize,
                                                probability,
                                                new StringKeyComparator());

    return Db::open(configuration);
}

void closeDb(Db* db) {
    db -> close();
    remove(filePath);
}

TEST(Db_Functional, Add500KeyValuePairs) {
    Db* db = openDb();
    std::vector<std::string> keys;
    std::vector<std::string> values;

    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"   + std::to_string(count);
        std::string value = "Value-" + std::to_string(count);

        keys.push_back(key);
        values.push_back(value);
    }
    for (int index = 0; index < 500; index++) {
        put(db, keys.at(index).c_str(), values.at(index).c_str());
    }
    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"   + std::to_string(count);
        std::string expectedValue = "Value-" + std::to_string(count);

        std::pair<const char*, bool> existenceByValue = db -> get(key.c_str());
        ASSERT_EQ(expectedValue, std::string(existenceByValue.first));
        ASSERT_TRUE(existenceByValue.second);
    }

    closeDb(db);
}

TEST(Db_Functional, DoesAScan) {
    Db* db = openDb();
    std::vector<std::string> keys;
    std::vector<std::string> values;

    for (int count = 1; count <= 500; count+=2) {
        std::string key   =  std::to_string(count);
        std::string value =  std::to_string(count);

        keys.push_back(key);
        values.push_back(value);
    }
    for (int index = 0; index < keys.size(); index++) {
        put(db, keys.at(index).c_str(), values.at(index).c_str());
    }
    std::string beginKey = "50";
    std::string endKey   = "70";

    std::vector<std::string> expectedKeys;
    std::vector<std::string> expectedValues;
    std::vector<KeyValuePair> expected;

    for (int count = 51; count < 70; count+=2) {
        expectedKeys.push_back(std::to_string(count));
        expectedValues.push_back(std::to_string(count));
    }
    for (int index = 0; index < expectedKeys.size(); index++) {
        expected.push_back(KeyValuePair(expectedKeys.at(index).c_str(), expectedValues.at(index).c_str()));
    }
    expected.push_back(KeyValuePair("7", "7"));

    std::vector<KeyValuePair> pairs = db -> scan(beginKey.c_str(), endKey.c_str(), 50);

    ASSERT_EQ(expected.at(0), pairs.at(0));

    closeDb(db);
}

TEST(Db_Functional, DoesAScanWithMaxPairsAs5) {
    Db* db = openDb(100, 0.25);
    std::vector<std::string> keys;
    std::vector<std::string> values;

    for (int count = 1; count <= 500; count+=2) {
        std::string key   =  std::to_string(count);
        std::string value =  std::to_string(count);

        keys.push_back(key);
        values.push_back(value);
    }
    for (int index = 0; index < keys.size(); index++) {
        put(db, keys.at(index).c_str(), values.at(index).c_str());
    }

    std::string beginKey = "50";
    std::string endKey   = "70";

    std::vector<std::string> expectedKeys;
    std::vector<std::string> expectedValues;
    std::vector<KeyValuePair> expected;

    for (int count = 51; count <= 59; count+=2) {
        expectedKeys.push_back(std::to_string(count));
        expectedValues.push_back(std::to_string(count));
    }
    for (int index = 0; index < expectedKeys.size(); index++) {
        expected.push_back(KeyValuePair(expectedKeys.at(index).c_str(), expectedValues.at(index).c_str()));
    }

    std::vector<KeyValuePair> pairs = db -> scan(beginKey.c_str(), endKey.c_str(), 5);
    ASSERT_EQ(expected, pairs);

    closeDb(db);
}

TEST(Db_Functional, Update500KeyValuePairs) {
    Db* db = openDb(100, 0.125);
    std::vector<std::string> keys;
    std::vector<std::string> values;

    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count);

        keys.push_back(key);
        values.push_back(value);
    }
    for (int index = 0; index < 500; index++) {
        put(db, keys.at(index).c_str(), values.at(index).c_str());
    }
    
    std::vector<std::string> valuesToUpdate;
    for (int count = 1; count <= 500; count++) {
        valuesToUpdate.push_back("Value-"  + std::to_string(count*2));
    }

    for (int index = 0; index < 500; index++) {
        update(db, keys.at(index).c_str(), valuesToUpdate.at(index).c_str());
    }
    for (int count = 1; count <= 500; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        std::string expectedValue = "Value-" + std::to_string(count*2);
        
        std::pair<const char*, bool> existenceByValue = db -> get(key.c_str());
        ASSERT_EQ(expectedValue, std::string(existenceByValue.first));
        ASSERT_TRUE(existenceByValue.second);
    }

    closeDb(db);
}

TEST(Db_Functional, DeleteKeys) {
    Db* db = openDb(100, 0.125);
    std::vector<std::string> keys;
    std::vector<std::string> values;

    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count);

        keys.push_back(key);
        values.push_back(value);
    }
    for (int index = 0; index < 500; index++) {
        put(db, keys.at(index).c_str(), values.at(index).c_str());
    }

    std::string deleteKey = "Key-"    + std::to_string(1);
    deleteBy(db, deleteKey.c_str());

    std::vector<std::string> keysToDelete;
    for (int count = 500; count <= 400; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        keysToDelete.push_back(key);
    }

    for (int index = 0; index < keysToDelete.size(); index++) {
        deleteBy(db, keysToDelete.at(index).c_str());
    }

    for (int count = 2; count < 400; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        std::string expectedValue = "Value-" + std::to_string(count);
        
        std::pair<const char*, bool> existenceByValue = db -> get(key.c_str());

        ASSERT_EQ(expectedValue, std::string(existenceByValue.first));
        ASSERT_TRUE(existenceByValue.second);
    }

    ASSERT_FALSE(db -> get(deleteKey.c_str()).second);
    for (int count = 500; count <= 400; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        
        std::pair<const char*, bool> existenceByValue = db -> get(key.c_str());

        ASSERT_EQ("", std::string(existenceByValue.first));
        ASSERT_FALSE(existenceByValue.second);
    }

    closeDb(db);
}


TEST(Db_Functional, GetTotalKeys) {
    Db* db = openDb();
    std::vector<std::string> keys;
    std::vector<std::string> values;

    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"   + std::to_string(count);
        std::string value = "Value-" + std::to_string(count);

        keys.push_back(key);
        values.push_back(value);
    }
    for (int index = 0; index < 500; index++) {
        put(db, keys.at(index).c_str(), values.at(index).c_str());
    }
    unsigned long totalKeys = db->totalKeys();

    ASSERT_EQ(500, totalKeys);
    closeDb(db);
}