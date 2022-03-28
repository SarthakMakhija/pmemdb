#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/SkipListIterator.h"
#include "./PersistentMemoryPoolFixture.h"
#include "./SkipListNodeTestUtils.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhilePuttingAKeyValue) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    iterator.put("HDD", "Hard disk drive", 0.5, [] {throw std::runtime_error("FailsWhilePuttingAKeyValue");});

    ASSERT_FALSE(iterator.getBy("HDD").second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhilePuttingAKeyValueSucceedsWithOther) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    iterator.put("SDD", "Solid state drive", 0.5);
    iterator.put("HDD", "Hard disk drive", 0.5, [] {throw std::runtime_error("FailsWhilePuttingAKeyValueSucceedsWithOther");});

    ASSERT_FALSE(iterator.getBy("HDD").second);
    ASSERT_EQ("Solid state drive", iterator.getBy("SDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhileUpdatingAKeyValue) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    iterator.put("HDD", "Hard disk drive", 0.5);
    iterator.update("HDD", "HDD", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});

    ASSERT_EQ("Hard disk drive", iterator.getBy("HDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhileUpdatingAKeyValueSucceedsWithOther) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    iterator.put("SDD", "Solid state drive", 0.5);
    iterator.put("HDD", "Hard disk drive", 0.5);

    iterator.update("HDD", "HDD", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValueSucceedsWithOther");});
    iterator.update("SDD", "SDD");

    ASSERT_EQ("Hard disk drive", iterator.getBy("HDD").first);
    ASSERT_EQ("SDD", iterator.getBy("SDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhileDeletingAKeyValue) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    iterator.put("HDD", "Hard disk drive", 0.5);
    iterator.deleteBy("HDD", [] {throw std::runtime_error("FailsWhileDeletingAKeyValue");});

    ASSERT_EQ("Hard disk drive", iterator.getBy("HDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_FailsWhileDeletingAKeyValueSucceedsWithOther) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    iterator.put("SDD", "Solid state drive", 0.5);
    iterator.put("HDD", "Hard disk drive", 0.5);

    iterator.deleteBy("HDD", [] {throw std::runtime_error("FailsWhileDeletingAKeyValueSucceedsWithOther");});
    iterator.deleteBy("SDD");

    ASSERT_EQ("Hard disk drive", iterator.getBy("HDD").first);
    ASSERT_FALSE(iterator.getBy("SDD").second);
}
