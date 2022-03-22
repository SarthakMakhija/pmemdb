#include <gtest/gtest.h>
#include <string>
#include <thread>
#include "../../src/engine/SkipList.h"
#include "./PersistentMemoryPoolFixture.h"

using namespace pmem::storage;

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsReadingTheSameKey) {
    SkipList* skipList = new SkipList(8, 0.5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    std::thread reader1([&]() {
        ASSERT_EQ("Hard disk drive", skipList -> get("HDD").first);
    });

    std::thread reader2([&]() {
        ASSERT_EQ("Hard disk drive", skipList -> get("HDD").first);
    });

    reader1.join();
    reader2.join();
}


TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsReadingDifferentKeys) {
    SkipList* skipList = new SkipList(8, 0.5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("Pmem", "Persistent Memory");

    std::thread reader1([&]() {
        ASSERT_EQ("Hard disk drive", skipList -> get("HDD").first);
    });

    std::thread reader2([&]() {
        ASSERT_EQ("Persistent Memory", skipList -> get("Pmem").first);
    });

    reader1.join();
    reader2.join();
}

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsPerformingMultiGet) {
    SkipList* skipList = new SkipList(8, 0.5);
    skipList -> put("HDD", "Hard disk drive");
    skipList -> put("SDD", "Solid state drive");
    skipList -> put("Pmem", "Persistent Memory");
    skipList -> put("Nvm", "Non volatile memory");

    std::thread reader1([&]() {
        std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk drive", true), 
                        std::make_pair("Solid state drive", true)
        };
        std::vector<std::string> keys = {"HDD", "SDD"};
        ASSERT_EQ(expected, skipList -> multiGet(keys));
    });

    std::thread reader2([&]() {
        std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Non volatile memory", true), 
                        std::make_pair("Persistent Memory", true)
        };
        std::vector<std::string> keys = {"Pmem", "Nvm"};
        ASSERT_EQ(expected, skipList -> multiGet(keys));
    });

    reader1.join();
    reader2.join();
}

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsPerformingScan) {
    SkipList* skipList = new SkipList(8, 0.5);
    skipList -> put("A", "A");
    skipList -> put("B", "B");
    skipList -> put("C", "C");
    skipList -> put("D", "D");
    skipList -> put("E", "E");

    std::thread reader1([&]() {
        std::vector<KeyValuePair> expected = {
            KeyValuePair("B", "B"), 
            KeyValuePair("C", "C"), 
            KeyValuePair("D", "D"), 
            KeyValuePair("E", "E")
        };
        ASSERT_EQ(expected, skipList -> scan("B", "F", 10));
    });

    std::thread reader2([&]() {
         std::vector<KeyValuePair> expected = {
            KeyValuePair("A", "A"), 
            KeyValuePair("B", "B")
        };
        ASSERT_EQ(expected, skipList -> scan("A", "C", 10));
    });

    reader1.join();
    reader2.join();
}

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsPerformingPutOnDifferentKeyValuePairs) {
    SkipList* skipList = new SkipList(8, 0.5);

    std::thread writer1([&]() {
        skipList -> put("HDD", "Hard disk drive");
        skipList -> put("SDD", "Solid state drive");
    });

    std::thread writer2([&]() {
        skipList -> put("Pmem", "Persistent Memory");
        skipList -> put("Nvm", "Non volatile memory");
    });

    writer1.join();
    writer2.join();

    std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk drive", true),
                        std::make_pair("Non volatile memory", true),
                        std::make_pair("Persistent Memory", true),
                        std::make_pair("Solid state drive", true)
    };
    std::vector<std::string> keys = {"Pmem", "Nvm", "HDD", "SDD"};
    ASSERT_EQ(expected, skipList -> multiGet(keys));
}

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsPerformingPutOnSameKeyValuePairs) {
    SkipList* skipList = new SkipList(8, 0.5);

    std::thread writer1([&]() {
        skipList -> put("HDD", "Hard disk drive");
        skipList -> put("SDD", "Solid state drive");
    });

    std::thread writer2([&]() {
        skipList -> put("HDD", "Hard disk drive");
        skipList -> put("SDD", "Solid state drive");
    });

    writer1.join();
    writer2.join();

    std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk drive", true),
                        std::make_pair("Solid state drive", true)
    };
    std::vector<std::string> keys = {"HDD", "SDD"};
    ASSERT_EQ(expected, skipList -> multiGet(keys));
}

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsPerformingPutAndUpdate) {
    SkipList* skipList = new SkipList(8, 0.5);

    std::thread writer1([&]() {
        skipList -> put("HDD", "Hard disk drive");
        skipList -> put("SDD", "Solid state drive");
    });
    
    writer1.join();

    std::thread writer2([&]() {
        skipList -> update("HDD", "Hard disk");
    });

    writer2.join();

    std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk", true),
                        std::make_pair("Solid state drive", true)
    };
    std::vector<std::string> keys = {"HDD", "SDD"};
    ASSERT_EQ(expected, skipList -> multiGet(keys));
}

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsPerformingUpdateOnSameKey) {
    SkipList* skipList = new SkipList(8, 0.5);
    skipList -> put("HDD", "Hard disk drive");

    std::thread writer1([&]() {
        skipList -> update("HDD", "Hard disk");
    });
    
    std::thread writer2([&]() {
        skipList -> update("HDD", "HDD");
    });

    writer1.join();
    writer2.join();

    std::string value = skipList -> get("HDD").first;
    ASSERT_TRUE(value == "Hard disk" || value == "HDD");
}

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsPerformingPutAndDeleteOnSameKey) {
    SkipList* skipList = new SkipList(8, 0.5);

    std::thread writer1([&]() {
        skipList -> put("HDD", "Hard disk drive");
    });
    
    std::thread writer2([&]() {
        skipList -> deleteBy("HDD");
    });

    writer1.join();
    writer2.join();

    std::string value = skipList -> get("HDD").first;
    ASSERT_TRUE(value == "Hard disk drive" || value == "");
}

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsPerformingPutAndDeleteRange) {
    SkipList* skipList = new SkipList(8, 0.5);

    std::thread writer1([&]() {
        skipList -> put("A", "A");
        skipList -> put("B", "B");
        skipList -> put("C", "C");
        skipList -> put("D", "D");
        skipList -> put("E", "E");
    });
    
    std::thread writer2([&]() {
        skipList -> deleteRange("B", "E");
    });

    writer1.join();
    writer2.join();

    std::vector<std::pair<std::string, bool>> expectedAll = {
                        std::make_pair("A", true), 
                        std::make_pair("B", true),
                        std::make_pair("C", true),
                        std::make_pair("D", true),
                        std::make_pair("E", true)
    };
    std::vector<std::pair<std::string, bool>> expectedDeleted = {
                        std::make_pair("A", true), 
                        std::make_pair("", false),
                        std::make_pair("", false),
                        std::make_pair("", false),
                        std::make_pair("E", true)
    };

    std::vector<std::string> keys = {"A", "B", "C", "D", "E"};
    std::vector<std::pair<std::string, bool>>  result = skipList -> multiGet(keys);

    ASSERT_TRUE(result == expectedAll || result == expectedDeleted);
}

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsPerformingUpdateAndDeleteRange) {
    SkipList* skipList = new SkipList(8, 0.5);
    skipList -> put("A", "A");
    skipList -> put("B", "B");
    skipList -> put("C", "C");
    skipList -> put("D", "D");
    skipList -> put("E", "E");

    std::thread writer1([&]() {
        skipList -> update("C", "C-Updated");
        skipList -> update("D", "D-Updated");
        skipList -> update("E", "E-Updated");
    });
    
    std::thread writer2([&]() {
        skipList -> deleteRange("B", "E");
    });

    writer1.join();
    writer2.join();

    std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("A", true), 
                        std::make_pair("", false),
                        std::make_pair("", false),
                        std::make_pair("", false),
                        std::make_pair("E-Updated", true)
    };

    std::vector<std::string> keys = {"A", "B", "C", "D", "E"};
    ASSERT_EQ(expected, skipList -> multiGet(keys));
}