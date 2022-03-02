#include <gtest/gtest.h>
#include <string>
#include <thread>
#include "../../src/engine/SkipList.h"
#include "./PersistentMemoryPoolFixture.h"


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

TEST_F(PersistentMemoryPoolFixture, SkipListConcurrentIntegration_TwoThreadsPerformingPutOnDSameKeyValuePairs) {
    SkipList* skipList = new SkipList(8, 0.5);

    std::thread writer1([&]() {
        try {
            skipList -> put("HDD", "Hard disk drive");
            skipList -> put("SDD", "Solid state drive");
        } catch(const std::invalid_argument &e) {}
    });

    std::thread writer2([&]() {
        try {
            skipList -> put("HDD", "Hard disk drive");
            skipList -> put("SDD", "Solid state drive");
        } catch(const std::invalid_argument &e) {}
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