#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <algorithm>

#include "../DbFixture.h"
#include "../DbTestsHelper.h"

using namespace pmem::storage;

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsReadingTheSameKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    std::thread reader1([&]() {
        ASSERT_EQ("Hard disk drive", std::string(DbFixture::getDb() -> get("HDD").first));
    });

    std::thread reader2([&]() {
        ASSERT_EQ("Hard disk drive", std::string(DbFixture::getDb() -> get("HDD").first));
    });

    reader1.join();
    reader2.join();
}


TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsReadingDifferentKeys) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    std::thread reader1([&]() {
        ASSERT_EQ("Hard disk drive", std::string(DbFixture::getDb() -> get("HDD").first));
    });

    std::thread reader2([&]() {
        ASSERT_EQ("Persistent Memory", std::string(DbFixture::getDb() -> get("Pmem").first));
    });

    reader1.join();
    reader2.join();
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingMultiGet) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "SDD", "Solid state drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "Nvm", "Non volatile memory");

    std::thread reader1([&]() {
        std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk drive", true),
                        std::make_pair("Solid state drive", true)
        };

        std::vector<Slice> keys = {Slice("HDD"), Slice("SDD")};
        std::vector<std::pair<const char*, bool>> result = DbFixture::getDb() -> multiGet(keys);
        std::vector<std::pair<std::string, bool>> resultTransformed(result.size());
        std::transform(result.begin(), result.end(), resultTransformed.begin(), [](std::pair<const char*, bool> pair) {
            return std::make_pair(std::string(pair.first), pair.second);
        });
        ASSERT_EQ(expected, resultTransformed);
    });

    std::thread reader2([&]() {
        std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Non volatile memory", true),
                        std::make_pair("Persistent Memory", true)
        };
        std::vector<Slice> keys = {Slice("Pmem"), Slice("Nvm")};
        std::vector<std::pair<const char*, bool>> result = DbFixture::getDb() -> multiGet(keys);
        std::vector<std::pair<std::string, bool>> resultTransformed(result.size());
        std::transform(result.begin(), result.end(), resultTransformed.begin(), [](std::pair<const char*, bool> pair) {
            return std::make_pair(std::string(pair.first), pair.second);
        });

        ASSERT_EQ(expected, resultTransformed);
    });

    reader1.join();
    reader2.join();
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingScan) {
    put(DbFixture::getDb(), "A", "A");
    put(DbFixture::getDb(), "B", "B");
    put(DbFixture::getDb(), "C", "C");
    put(DbFixture::getDb(), "D", "D");
    put(DbFixture::getDb(), "E", "E");

    std::thread reader1([&]() {
        std::vector<KeyValuePair> expected = {
            KeyValuePair("B", "B"), 
            KeyValuePair("C", "C"), 
            KeyValuePair("D", "D"), 
            KeyValuePair("E", "E")
        };
        ASSERT_EQ(expected, DbFixture::getDb() -> scan(Slice("B"), Slice("F"), 10));
    });

    std::thread reader2([&]() {
         std::vector<KeyValuePair> expected = {
            KeyValuePair("A", "A"), 
            KeyValuePair("B", "B")
        };
        ASSERT_EQ(expected, DbFixture::getDb() -> scan(Slice("A"), Slice("C"), 10));
    });

    reader1.join();
    reader2.join();
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingPutOnDifferentKeyValuePairs) {
    std::thread writer1([&]() {
        put(DbFixture::getDb(), "HDD", "Hard disk drive");
        put(DbFixture::getDb(), "SDD", "Solid state drive");
    });

    std::thread writer2([&]() {
        put(DbFixture::getDb(), "Pmem", "Persistent Memory");
        put(DbFixture::getDb(), "Nvm", "Non volatile memory");
    });

    writer1.join();
    writer2.join();

    std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk drive", true),
                        std::make_pair("Non volatile memory", true),
                        std::make_pair("Persistent Memory", true),
                        std::make_pair("Solid state drive", true)
    };
    std::vector<Slice> keys = {Slice("Pmem"), Slice("Nvm"), Slice("HDD"), Slice("SDD")};
    std::vector<std::pair<const char*, bool>> result = DbFixture::getDb() -> multiGet(keys);
    std::vector<std::pair<std::string, bool>> resultTransformed(result.size());
    std::transform(result.begin(), result.end(), resultTransformed.begin(), [](std::pair<const char*, bool> pair) {
        return std::make_pair(std::string(pair.first), pair.second);
    });

    ASSERT_EQ(expected, resultTransformed);
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingPutOnSameKeyValuePairs) {
    std::thread writer1([&]() {
        put(DbFixture::getDb(), "HDD", "Hard disk drive");
        put(DbFixture::getDb(), "SDD", "Solid state drive");
    });

    std::thread writer2([&]() {
        put(DbFixture::getDb(), "HDD", "Hard disk drive");
        put(DbFixture::getDb(), "SDD", "Solid state drive");
    });

    writer1.join();
    writer2.join();

    std::vector<std::pair<std::string, bool>> expected = {
                        std::make_pair("Hard disk drive", true),
                        std::make_pair("Solid state drive", true)
    };
    std::vector<Slice> keys = {Slice("HDD"), Slice("SDD")};
    std::vector<std::pair<const char*, bool>> result = DbFixture::getDb() -> multiGet(keys);
    std::vector<std::pair<std::string, bool>> resultTransformed(result.size());
    std::transform(result.begin(), result.end(), resultTransformed.begin(), [](std::pair<const char*, bool> pair) {
        return std::make_pair(std::string(pair.first), pair.second);
    });

    ASSERT_EQ(expected, resultTransformed);
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingPutAndUpdate) {
    std::thread writer1([&]() {
        put(DbFixture::getDb(), "HDD", "Hard disk drive");
        put(DbFixture::getDb(), "SDD", "Solid state drive");
    });
    
    writer1.join();

    std::thread writer2([&]() {
        update(DbFixture::getDb(), "HDD", "Hard disk");
    });

    writer2.join();

    std::vector<std::pair<std::string , bool>> expected = {
                        std::make_pair("Hard disk", true),
                        std::make_pair("Solid state drive", true)
    };
    std::vector<Slice> keys = {Slice("HDD"), Slice("SDD")};
    std::vector<std::pair<const char*, bool>> result = DbFixture::getDb() -> multiGet(keys);
    std::vector<std::pair<std::string, bool>> resultTransformed(result.size());
    std::transform(result.begin(), result.end(), resultTransformed.begin(), [](std::pair<const char*, bool> pair) {
        return std::make_pair(std::string(pair.first), pair.second);
    });

    ASSERT_EQ(expected, resultTransformed);
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingUpdateOnSameKey) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");

    std::thread writer1([&]() {
        update(DbFixture::getDb(), "HDD", "Hard disk");
    });
    
    std::thread writer2([&]() {
        update(DbFixture::getDb(), "HDD", "HDD");
    });

    writer1.join();
    writer2.join();

    const char* value = DbFixture::getDb() -> get("HDD").first;
    ASSERT_TRUE(std::string(value) == "Hard disk" || std::string(value) == "HDD");
}

TEST_F(DbFixture, DbConcurrentIntegration_TwoThreadsPerformingPutAndDeleteOnSameKey) {
    std::thread writer1([&]() {
        put(DbFixture::getDb(), "HDD", "Hard disk drive");
    });
    
    std::thread writer2([&]() {
        deleteBy(DbFixture::getDb(), "HDD");
    });

    writer1.join();
    writer2.join();

    const char* value = DbFixture::getDb() -> get("HDD").first;
    ASSERT_TRUE(std::string(value) == "Hard disk drive" || std::string(value) == "");
}