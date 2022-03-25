#include <gtest/gtest.h>
#include <string>
#include  <cstdio>

#include "../../src/engine/Db.h"

using namespace pmem::storage;

const char* filePath = "./functional.log";

Db* openDb(int skipListTowerSize = 100, double probability = 0.5) {
    Configuration configuration = Configuration(filePath,
                                                8 * 1024 * 1024,
                                                skipListTowerSize,
                                                probability);

    return Db::open(configuration);
}

void closeDb(Db* db) {
    db -> close();
    remove(filePath);
}

TEST(Db_Functional, Add500KeyValuePairs) {
    Db* db = openDb();

    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count);

        db -> put(key.c_str(), value.c_str());
    }
    for (int count = 1; count <= 500; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        std::string expectedValue = "Value-" + std::to_string(count);
        
        std::pair<std::string, bool> existenceByValue = db -> get(key.c_str());
        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }

    closeDb(db);
}

TEST(Db_Functional, DoesAScan) {
    Db* db = openDb();

    for (int count = 1; count <= 500; count+=2) {
        std::string key   =  std::to_string(count);
        std::string value =  std::to_string(count);

        db -> put(key.c_str(), value.c_str());
    }    
    std::string beginKey = "50";
    std::string endKey   = "70";

    std::vector<KeyValuePair> expected;
    for (int count = 51; count < 70; count+=2) {
        expected.push_back(KeyValuePair(std::to_string(count), std::to_string(count)));
    }
    expected.push_back(KeyValuePair("7", "7"));

    std::vector<KeyValuePair> pairs = db -> scan(beginKey.c_str(), endKey.c_str(), 50);
    ASSERT_EQ(expected, pairs);

    closeDb(db);
}

TEST(Db_Functional, DoesAScanWithMaxPairsAs5) {
    Db* db = openDb(100, 0.25);

    for (int count = 1; count <= 500; count+=2) {
        std::string key   =  std::to_string(count);
        std::string value =  std::to_string(count);

        db -> put(key.c_str(), value.c_str());
    }    
    std::string beginKey = "50";
    std::string endKey   = "70";

    std::vector<KeyValuePair> expected;
    for (int count = 51; count <= 59; count+=2) {
        expected.push_back(KeyValuePair(std::to_string(count), std::to_string(count)));
    }

    std::vector<KeyValuePair> pairs = db -> scan(beginKey.c_str(), endKey.c_str(), 5);
    ASSERT_EQ(expected, pairs);

    closeDb(db);
}

TEST(Db_Functional, Update500KeyValuePairs) {
    Db* db = openDb(100, 0.125);

    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count);

        db -> put(key.c_str(), value.c_str());
    }
    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count*2);

        db -> update(key.c_str(), value.c_str());
    }
    for (int count = 1; count <= 500; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        std::string expectedValue = "Value-" + std::to_string(count*2);
        
        std::pair<std::string, bool> existenceByValue = db -> get(key.c_str());
        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }

    closeDb(db);
}

TEST(Db_Functional, DeleteKeys) {
    Db* db = openDb(100, 0.125);

    for (int count = 1; count <= 500; count++) {
        std::string key   = "Key-"    + std::to_string(count);
        std::string value = "Value-"  + std::to_string(count);
        db -> put(key.c_str(), value.c_str());
    }

    std::string deleteKey = "Key-"    + std::to_string(1);
    db -> deleteBy(deleteKey.c_str());

    for (int count = 500; count <= 400; count++) {
        std::string key   = "Key-"    + std::to_string(count);

        db -> deleteBy(key.c_str());
    }

    for (int count = 2; count < 400; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        std::string expectedValue = "Value-" + std::to_string(count);
        
        std::pair<std::string, bool> existenceByValue = db -> get(key.c_str());

        ASSERT_EQ(expectedValue, existenceByValue.first);
        ASSERT_TRUE(existenceByValue.second);
    }

    ASSERT_FALSE(db -> get(deleteKey.c_str()).second);
    for (int count = 500; count <= 400; count++) {
        std::string key           = "Key-"   + std::to_string(count);
        
        std::pair<std::string, bool> existenceByValue = db -> get(key.c_str());

        ASSERT_EQ("", existenceByValue.first);
        ASSERT_FALSE(existenceByValue.second);
    }

    closeDb(db);
}