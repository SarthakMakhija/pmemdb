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