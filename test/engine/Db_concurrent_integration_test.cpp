#include <gtest/gtest.h>
#include <string>
#include <thread>

#include "./DbFixture.h"

using namespace pmem::storage;

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsReadingTheSameKey) {
    DbFixture::getDb()->put("HDD", "Hard disk drive");
    DbFixture::getDb()->put("Pmem", "Persistent Memory");

    std::thread reader1([&]() {
        ASSERT_EQ("Hard disk drive", DbFixture::getDb() -> get("HDD").first);
    });

    std::thread reader2([&]() {
        ASSERT_EQ("Hard disk drive", DbFixture::getDb() -> get("HDD").first);
    });

    reader1.join();
    reader2.join();
}


TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsReadingDifferentKeys) {
    DbFixture::getDb() -> put("HDD", "Hard disk drive");
    DbFixture::getDb() -> put("Pmem", "Persistent Memory");

    std::thread reader1([&]() {
        ASSERT_EQ("Hard disk drive", DbFixture::getDb() -> get("HDD").first);
    });

    std::thread reader2([&]() {
        ASSERT_EQ("Persistent Memory", DbFixture::getDb() -> get("Pmem").first);
    });

    reader1.join();
    reader2.join();
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingMultiGet) {
    DbFixture::getDb() -> put("HDD", "Hard disk drive");
    DbFixture::getDb() -> put("SDD", "Solid state drive");
    DbFixture::getDb() -> put("Pmem", "Persistent Memory");
    DbFixture::getDb() -> put("Nvm", "Non volatile memory");

    std::thread reader1([&]() {
        std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk drive", true), 
                        std::make_pair("Solid state drive", true)
        };
        std::vector<const char*> keys = {"HDD", "SDD"};
        ASSERT_EQ(expected, DbFixture::getDb() -> multiGet(keys));
    });

    std::thread reader2([&]() {
        std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Non volatile memory", true), 
                        std::make_pair("Persistent Memory", true)
        };
        std::vector<const char*> keys = {"Pmem", "Nvm"};
        ASSERT_EQ(expected, DbFixture::getDb() -> multiGet(keys));
    });

    reader1.join();
    reader2.join();
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingScan) {
    DbFixture::getDb() -> put("A", "A");
    DbFixture::getDb() -> put("B", "B");
    DbFixture::getDb() -> put("C", "C");
    DbFixture::getDb() -> put("D", "D");
    DbFixture::getDb() -> put("E", "E");

    std::thread reader1([&]() {
        std::vector<KeyValuePair> expected = {
            KeyValuePair("B", "B"), 
            KeyValuePair("C", "C"), 
            KeyValuePair("D", "D"), 
            KeyValuePair("E", "E")
        };
        ASSERT_EQ(expected, DbFixture::getDb() -> scan("B", "F", 10));
    });

    std::thread reader2([&]() {
         std::vector<KeyValuePair> expected = {
            KeyValuePair("A", "A"), 
            KeyValuePair("B", "B")
        };
        ASSERT_EQ(expected, DbFixture::getDb() -> scan("A", "C", 10));
    });

    reader1.join();
    reader2.join();
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingPutOnDifferentKeyValuePairs) {
    std::thread writer1([&]() {
        DbFixture::getDb() -> put("HDD", "Hard disk drive");
        DbFixture::getDb() -> put("SDD", "Solid state drive");
    });

    std::thread writer2([&]() {
        DbFixture::getDb() -> put("Pmem", "Persistent Memory");
        DbFixture::getDb() -> put("Nvm", "Non volatile memory");
    });

    writer1.join();
    writer2.join();

    std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk drive", true),
                        std::make_pair("Non volatile memory", true),
                        std::make_pair("Persistent Memory", true),
                        std::make_pair("Solid state drive", true)
    };
    std::vector<const char*> keys = {"Pmem", "Nvm", "HDD", "SDD"};
    ASSERT_EQ(expected, DbFixture::getDb() -> multiGet(keys));
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingPutOnSameKeyValuePairs) {
    std::thread writer1([&]() {
        DbFixture::getDb() -> put("HDD", "Hard disk drive");
        DbFixture::getDb() -> put("SDD", "Solid state drive");
    });

    std::thread writer2([&]() {
        DbFixture::getDb() -> put("HDD", "Hard disk drive");
        DbFixture::getDb() -> put("SDD", "Solid state drive");
    });

    writer1.join();
    writer2.join();

    std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk drive", true),
                        std::make_pair("Solid state drive", true)
    };
    std::vector<const char*> keys = {"HDD", "SDD"};
    ASSERT_EQ(expected, DbFixture::getDb() -> multiGet(keys));
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingPutAndUpdate) {
    std::thread writer1([&]() {
        DbFixture::getDb() -> put("HDD", "Hard disk drive");
        DbFixture::getDb() -> put("SDD", "Solid state drive");
    });
    
    writer1.join();

    std::thread writer2([&]() {
        DbFixture::getDb() -> update("HDD", "Hard disk");
    });

    writer2.join();

    std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk", true),
                        std::make_pair("Solid state drive", true)
    };
    std::vector<const char*> keys = {"HDD", "SDD"};
    ASSERT_EQ(expected, DbFixture::getDb() -> multiGet(keys));
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingUpdateOnSameKey) {
    DbFixture::getDb() -> put("HDD", "Hard disk drive");

    std::thread writer1([&]() {
        DbFixture::getDb() -> update("HDD", "Hard disk");
    });
    
    std::thread writer2([&]() {
        DbFixture::getDb() -> update("HDD", "HDD");
    });

    writer1.join();
    writer2.join();

    std::string value = DbFixture::getDb() -> get("HDD").first;
    ASSERT_TRUE(value == "Hard disk" || value == "HDD");
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingPutAndDeleteOnSameKey) {
    std::thread writer1([&]() {
        DbFixture::getDb() -> put("HDD", "Hard disk drive");
    });
    
    std::thread writer2([&]() {
        DbFixture::getDb() -> deleteBy("HDD");
    });

    writer1.join();
    writer2.join();

    std::string value = DbFixture::getDb() -> get("HDD").first;
    ASSERT_TRUE(value == "Hard disk drive" || value == "");
}