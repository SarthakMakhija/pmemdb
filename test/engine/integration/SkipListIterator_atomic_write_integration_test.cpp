#include <gtest/gtest.h>
#include <string>
#include "../../../src/engine/SkipListInternalNode.h"
#include "../../../src/engine/SkipListLeafNode.h"
#include "../../../src/engine/SkipListIterator.h"
#include "PersistentMemoryPoolFixture.h"
#include "../SkipListNodeTestUtils.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

void put(SkipListIterator iterator,
         const char* key,
         const char* value,
         std::function<void(void)> postPutHook = [] {}) {

    iterator.put(key, value, KeyValueSize(strlen(key) + 1, strlen(value) + 1), postPutHook);
}

void update(SkipListIterator iterator,
            const char* key,
            const char* value,
            std::function<void(void)> postUpdateHook = [] {}) {

    iterator.update(key, value, KeyValueSize(strlen(key) + 1, strlen(value) + 1), postUpdateHook);
}

void deleteBy(SkipListIterator iterator,
              const char* key,
              std::function<void(void)> postDeleteHook = [] {}) {

    iterator.deleteBy(key, postDeleteHook);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhilePuttingAKeyValue) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    put(iterator, "HDD", "Hard disk drive", [] {throw std::runtime_error("FailsWhilePuttingAKeyValue");});

    ASSERT_FALSE(iterator.getBy("HDD").second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhilePuttingAKeyValueSucceedsWithOther) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    put(iterator, "SDD", "Solid state drive");
    put(iterator, "HDD", "Hard disk drive", [] {throw std::runtime_error("FailsWhilePuttingAKeyValueSucceedsWithOther");});

    ASSERT_FALSE(iterator.getBy("HDD").second);
    ASSERT_EQ("Solid state drive", std::string(iterator.getBy("SDD").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhileUpdatingAKeyValue) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    put(iterator, "HDD", "Hard disk drive");
    update(iterator, "HDD", "HDD", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});

    ASSERT_EQ("Hard disk drive", std::string(iterator.getBy("HDD").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhileUpdatingAKeyValueSucceedsWithOther) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    put(iterator, "SDD", "Solid state drive");
    put(iterator, "HDD", "Hard disk drive");

    update(iterator, "HDD", "HDD", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValueSucceedsWithOther");});
    update(iterator, "SDD", "SDD");

    ASSERT_EQ("Hard disk drive", std::string(iterator.getBy("HDD").first));
    ASSERT_EQ("SDD", std::string(iterator.getBy("SDD").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhileDeletingAKeyValue) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    put(iterator, "HDD", "Hard disk drive");
    deleteBy(iterator, "HDD", [] {throw std::runtime_error("FailsWhileDeletingAKeyValue");});

    ASSERT_EQ("Hard disk drive", std::string(iterator.getBy("HDD").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhileDeletingAKeyValueSucceedsWithOther) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    put(iterator, "SDD", "Solid state drive");
    put(iterator, "HDD", "Hard disk drive");

    deleteBy(iterator, "HDD", [] {throw std::runtime_error("FailsWhileDeletingAKeyValueSucceedsWithOther");});
    deleteBy(iterator, "SDD");

    ASSERT_EQ("Hard disk drive", std::string(iterator.getBy("HDD").first));
    ASSERT_FALSE(iterator.getBy("SDD").second);
}
