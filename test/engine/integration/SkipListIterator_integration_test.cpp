#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include <thread>
#include "../DbFixture.h"
#include "../DbTestsHelper.h"
#include "../../../src/engine/db/Db.h"
#include "../../../src/engine/iterator/SkipListIterator.h"

using namespace pmem::storage;

struct EmployeeId {
    int id;
    std::string lastName;
};
struct Employee {
    int id;
    std::string firstName;
    std::string lastName;
};
class EmployeeIdComparator : public KeyComparator {
public:
    int compare(const Slice& a, const Slice& b) const override {
        const EmployeeId* first = reinterpret_cast<const EmployeeId*>(a.cdata());
        const EmployeeId* other = reinterpret_cast<const EmployeeId*>(b.cdata());

        if (first->id == other->id) {
            return 0;
        }
        if (first->id < other->id) {
            return -1;
        }
        return 1;
    }
};

TEST_F(DbFixture, SkipListIteratorIntegration_SeekToFirst) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
    iterator->seekToFirst();

    ASSERT_EQ(Slice("HDD"), iterator->key());
}

TEST_F(DbFixture, SkipListIteratorIntegration_SeekToLast) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
    iterator->seekToLast();

    ASSERT_EQ(Slice("Pmem"), iterator->key());
}

TEST_F(DbFixture, SkipListIteratorIntegration_Seek1) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
    iterator->seek("SDD");

    ASSERT_EQ(Slice("SDD"), iterator->key());
    ASSERT_EQ(Slice("Solid State drive"), iterator->value());
}

TEST_F(DbFixture, SkipListIteratorIntegration_Seek2) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
    iterator->seek("SDD");

    ASSERT_EQ(Slice("SDD"),               iterator->key());
    ASSERT_EQ(Slice("Solid State drive"), iterator->value());

    iterator->seek("HDD");
    ASSERT_EQ(Slice("HDD"),             iterator->key());
    ASSERT_EQ(Slice("Hard disk drive"), iterator->value());
}

TEST_F(DbFixture, SkipListIteratorIntegration_Next1) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
    iterator->seekToFirst();

    ASSERT_EQ(Slice("HDD"), iterator->key());
}

TEST_F(DbFixture, SkipListIteratorIntegration_Next2) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
    iterator->seekToFirst();
    iterator->next();

    ASSERT_EQ(Slice("Pmem"), iterator->key());
}

TEST_F(DbFixture, SkipListIteratorIntegration_Value) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
    iterator->seekToFirst();

    ASSERT_EQ(Slice("Hard disk drive"), iterator->value());
}

TEST_F(DbFixture, SkipListIteratorIntegration_SerializedValue) {
    auto employee   = new Employee  {11, "Mark", "Johnson"};
    auto employeeId = new EmployeeId{11, "Johnson"};
    Db* db          = DbFixture::getDb();

    char *key = (char*)malloc(sizeof(*employeeId));
    memcpy(key, (char*)employeeId, sizeof(*employeeId));
    
    char *value = (char*)malloc(sizeof(*employee));
    memcpy(value, (char*)employee, sizeof(*employee));

    db->put(Slice(key, sizeof(*employeeId)), Slice(value, sizeof(*employee)));
    
    SkipListIterator* iterator = db->newIterator();
    iterator->seekToFirst();

    Slice savedKey   = iterator->key();
    Slice savedValue = iterator->value();

    ASSERT_EQ(sizeof(*employeeId),  savedKey.size());
    ASSERT_EQ(sizeof(*employee),    savedValue.size());

    const Employee* saved = reinterpret_cast<const Employee*>(savedValue.cdata());
    ASSERT_EQ(11,           saved->id);
    ASSERT_EQ("Mark",       saved->firstName);
    ASSERT_EQ("Johnson",    saved->lastName);
}

TEST_F(DbFixture, SkipListIteratorIntegration_IsValid) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");

    SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
    iterator->seekToFirst();

    ASSERT_TRUE(iterator->isValid());

    iterator->next();
    ASSERT_TRUE(iterator->isValid());

    iterator->next();
    ASSERT_FALSE(iterator->isValid());
}

TEST_F(DbFixture, SkipListIteratorIntegration_IterateAll) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");
    put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    put(DbFixture::getDb(), "SDD", "Solid State drive");

    SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
    std::vector<std::pair<Slice, Slice>> expected = {
                                                        std::make_pair(Slice("HDD"),  Slice("Hard disk drive")),
                                                        std::make_pair(Slice("Pmem"), Slice("Persistent Memory")),
                                                        std::make_pair(Slice("SDD"),  Slice("Solid State drive"))
                                                    };

    std::vector<std::pair<Slice, Slice>> result;
    iterator->seekToFirst();

    while (iterator->isValid()) {
        Slice key   = iterator->key();
        Slice value = iterator->value();

        result.push_back(std::make_pair(key, value));
        iterator->next();
    }

    ASSERT_EQ(expected, result);
}


TEST_F(DbFixture, SkipListIteratorIntegration_SeekWithConcurrentPut) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");

    std::thread writer([&]() {
        put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    });

    std::thread iterator([&]() {
        SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
        iterator->seek("Pmem");
        if (iterator->isValid()) {
            ASSERT_EQ(Slice("Pmem"), iterator->key());
        } else {
            ASSERT_FALSE(iterator->isValid());
        }
    });

    writer.join();
    iterator.join();
}

TEST_F(DbFixture, SkipListIteratorIntegration_IterateAllWithConcurrentPut) {
    put(DbFixture::getDb(), "HDD", "Hard disk drive");

    std::thread writer([&]() {
        put(DbFixture::getDb(), "Pmem", "Persistent Memory");
    });

    std::thread iterator([&]() {
        SkipListIterator* iterator = DbFixture::getDb() -> newIterator();
        std::vector<std::pair<Slice, Slice>> expected1 = {std::make_pair(Slice("HDD"),  Slice("Hard disk drive"))};                                                                  
        std::vector<std::pair<Slice, Slice>> expected2 = {std::make_pair(Slice("HDD"),  Slice("Hard disk drive")),
                                                          std::make_pair(Slice("Pmem"), Slice("Persistent Memory")) };

        std::vector<std::pair<Slice, Slice>> result;
        iterator->seekToFirst();
        
        while (iterator->isValid()) {
            Slice key   = iterator->key();
            Slice value = iterator->value();

            result.push_back(std::make_pair(key, value));
            iterator->next();
        }
        ASSERT_TRUE(expected1 == result || expected2 == result);
    });

    writer.join();
    iterator.join();
}