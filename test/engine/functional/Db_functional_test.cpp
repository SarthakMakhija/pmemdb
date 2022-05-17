#include <gtest/gtest.h>
#include <string>
#include  <cstdio>

#include "../../../src/engine/db/Db.h"
#include "../../../src/engine/comparator/StringKeyComparator.h"
#include "../DbTestsHelper.h"

using namespace pmem::storage;

const char* filePath = "./functional.log";

Db* openDb(int skipListTowerSize = 100) {
    Configuration configuration = Configuration(filePath,
                                                8 * 1024 * 1024,
                                                skipListTowerSize,
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
        Slice key   = Slice(keys.at(count - 1).c_str());
        Slice expectedValue = Slice(values.at(count - 1).c_str());

        std::pair<Slice, bool> existenceByValue = db -> get(key);
        ASSERT_EQ(expectedValue, existenceByValue.first);
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
    Slice beginKey = Slice("50");
    Slice endKey   = Slice("70");

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

    std::vector<KeyValuePair> pairs = db -> scan(beginKey, endKey, 50);

    ASSERT_EQ(expected.at(0), pairs.at(0));

    closeDb(db);
}

TEST(Db_Functional, DoesAScanWithMaxPairsAs5) {
    Db* db = openDb(100);
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

    Slice beginKey = Slice("50");
    Slice endKey   = Slice("70");

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

    std::vector<KeyValuePair> pairs = db -> scan(beginKey, endKey, 5);
    ASSERT_EQ(expected, pairs);

    closeDb(db);
}

TEST(Db_Functional, Update500KeyValuePairs) {
    Db* db = openDb(100);
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
        Slice key           = Slice(keys.at(count - 1).c_str());
        Slice expectedValue = Slice(valuesToUpdate.at(count - 1).c_str());
        
        std::pair<Slice, bool> existenceByValue = db -> get(key);
        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }

    closeDb(db);
}

TEST(Db_Functional, DeleteKeys) {
    Db* db = openDb(100);
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
        Slice key           = Slice(keys.at(count - 1).c_str());
        Slice expectedValue = Slice(values.at(count - 1).c_str());
        
        std::pair<Slice, bool> existenceByValue = db -> get(key);

        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }

    ASSERT_FALSE(db -> get(Slice(deleteKey.c_str())).second);
    for (int count = 500; count <= 400; count++) {
        Slice key = Slice(keys.at(count - 1).c_str());
        
        std::pair<Slice, bool> existenceByValue = db -> get(key);

        ASSERT_EQ(Slice(""), existenceByValue.first);
        ASSERT_FALSE(existenceByValue.second);
    }

    closeDb(db);
}

TEST(Db_Functional, GetTotalKeys) {
    Db* db = openDb(1);
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