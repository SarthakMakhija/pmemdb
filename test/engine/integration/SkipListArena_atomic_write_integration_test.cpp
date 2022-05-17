#include <gtest/gtest.h>
#include <string>
#include "../../../src/engine/storage/SkipListInternalNode.h"
#include "../../../src/engine/storage/SkipListLeafNode.h"
#include "../../../src/engine/storage/utils/LevelGenerator.h"
#include "../../../src/engine/db/SkipListArena.h"
#include "PersistentMemoryPoolFixture.h"
#include "../SkipListNodeTestUtils.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

void put(SkipListArena* arena,
         const char* key,
         const char* value,
         std::function<void(void)> postPutHook = [] {}) {

    arena->put(Slice(key, strlen(key) + 1), 
               Slice(value, strlen(value) + 1), new LevelGenerator(6), postPutHook);
}

void update(SkipListArena* arena,
            const char* key,
            const char* value,
            std::function<void(void)> postUpdateHook = [] {}) {

    arena->update(Slice(key, strlen(key) + 1), Slice(value, strlen(value) + 1), postUpdateHook);
}

void deleteBy(SkipListArena* arena,
              const char* key,
              std::function<void(void)> postDeleteHook = [] {}) {

    arena->deleteBy(Slice(key, strlen(key) + 1), postDeleteHook);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_FailsWhilePuttingAKeyValue) {
    SkipListNode* sentinel    = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    SkipListArena* arena = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

    put(arena, "HDD", "Hard disk drive", [] {throw std::runtime_error("FailsWhilePuttingAKeyValue");});

    ASSERT_FALSE(arena->getBy(Slice("HDD")).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_FailsWhilePuttingAKeyValueSucceedsWithOther) {
    SkipListNode* sentinel    = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    SkipListArena* arena = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

    put(arena, "SDD", "Solid state drive");
    put(arena, "HDD", "Hard disk drive", [] {throw std::runtime_error("FailsWhilePuttingAKeyValueSucceedsWithOther");});

    ASSERT_FALSE(arena->getBy(Slice("HDD")).second);
    ASSERT_EQ(Slice("Solid state drive"), arena->getBy(Slice("SDD")).first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_FailsWhileUpdatingAKeyValue) {
    SkipListNode* sentinel    = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    SkipListArena* arena = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

    put(arena, "HDD", "Hard disk drive");
    update(arena, "HDD", "HDD", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});

    ASSERT_EQ(Slice("Hard disk drive"), arena->getBy(Slice("HDD")).first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_FailsWhileUpdatingAKeyValueSucceedsWithOther) {
    SkipListNode* sentinel    = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    SkipListArena* arena = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

    put(arena, "SDD", "Solid state drive");
    put(arena, "HDD", "Hard disk drive");

    update(arena, "HDD", "HDD", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValueSucceedsWithOther");});
    update(arena, "SDD", "SDD");

    ASSERT_EQ(Slice("Hard disk drive"), arena->getBy(Slice("HDD")).first);
    ASSERT_EQ(Slice("SDD"), arena->getBy(Slice("SDD")).first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_FailsWhileDeletingAKeyValue) {
    SkipListNode* sentinel    = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    SkipListArena* arena = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

    put(arena, "HDD", "Hard disk drive");
    deleteBy(arena, "HDD", [] {throw std::runtime_error("FailsWhileDeletingAKeyValue");});

    ASSERT_EQ(Slice("Hard disk drive"), arena->getBy(Slice("HDD")).first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_FailsWhileDeletingAKeyValueSucceedsWithOther) {
    SkipListNode* sentinel    = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    SkipListArena* arena = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

    put(arena, "SDD", "Solid state drive");
    put(arena, "HDD", "Hard disk drive");

    deleteBy(arena, "HDD", [] {throw std::runtime_error("FailsWhileDeletingAKeyValueSucceedsWithOther");});
    deleteBy(arena, "SDD");

    ASSERT_EQ(Slice("Hard disk drive"), arena->getBy(Slice("HDD")).first);
    ASSERT_FALSE(arena->getBy(Slice("SDD")).second);
}
