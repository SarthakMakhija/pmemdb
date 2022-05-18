#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include "../../../src/engine/storage/SkipListInternalNode.h"
#include "../../../src/engine/storage/SkipListLeafNode.h"
#include "../../../src/engine/storage/utils/LevelGenerator.h"
#include "../../../src/engine/db/KeyValuePair.h"
#include "../../../src/engine/db/SkipListArena.h"
#include "PersistentMemoryPoolFixture.h"
#include "../SkipListNodeTestUtils.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

void put(SkipListArena* arena, const char* key, const char* value) {
    arena->put(Slice(key), 
               Slice(value), new LevelGenerator(6));
}

void update(SkipListArena* arena, const char* key, const char* value) {
    arena->update(Slice(key), 
                  Slice(value));
}

void deleteBy(SkipListArena* arena, const char* key) {
    arena->deleteBy(Slice(key));
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_PutASingleKeyValuePair) {
    SkipListNode* sentinel    = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    SkipListArena* arena = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

    put(arena, "HDD", "Hard disk drive");

    std::pair<Slice, bool> valueByExistence = arena->getBy(Slice("HDD"));

    ASSERT_EQ(Slice("Hard disk drive"), valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_PutMultipleKeyValuePairs) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");

  ASSERT_EQ(Slice("Hard disk drive"), arena->getBy(Slice("HDD")).first);
  ASSERT_EQ(Slice("Solid state drive"), arena->getBy(Slice("SDD")).first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_GetByKeyForAnExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");

  ASSERT_EQ(Slice("Hard disk drive"),   arena->getBy(Slice("HDD")).first);
  ASSERT_EQ(Slice("Solid state drive"), arena->getBy(Slice("SDD")).first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_GetByKeyForANonExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");

  ASSERT_EQ(Slice(""), arena->getBy(Slice("Pmem")).first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_MultiGet) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  std::vector<const char*> keys = {"HDD", "SDD", "Pmem", "DoesNotExist"};
  std::vector<Slice> keySlices;
  for (auto key: keys) {
      keySlices.push_back(Slice(key, strlen(key) + 1));
  }

  std::vector<std::pair<Slice, bool>> result = arena->multiGet(keySlices);
  std::vector<std::pair<std::string, bool>> resultTransformed(result.size());
  std::transform(result.begin(), result.end(), resultTransformed.begin(), [](std::pair<Slice, bool> pair){
        return std::make_pair(std::string(pair.first.cdata()), pair.second);
  });

  std::vector<std::pair<std::string, bool>> expected = {
                            std::make_pair("", false),
                            std::make_pair("Hard disk drive", true),
                            std::make_pair("Persistent memory", true),
                            std::make_pair("Solid state drive", true)
  };

  ASSERT_EQ(expected, resultTransformed);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_ScanWithBeginKeyPresent) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  auto beginKey = Slice("Pmem");
  auto endKey = Slice("SDD");

  std::vector<KeyValuePair> pairs = arena->scan(beginKey, endKey, 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent memory")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_ScanWithBeginKeyNotPresent) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  Slice beginKey = Slice("RAM");
  Slice endKey   = Slice("Tuff");

  std::vector<KeyValuePair> pairs = arena->scan(beginKey, endKey, 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("SDD", "Solid state drive")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_ScanWithBeginKeyOutsideTheBounds) {
    SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

    put(arena, "HDD", "Hard disk drive");
    put(arena, "SDD", "Solid state drive");
    put(arena, "Pmem", "Persistent memory");

    Slice beginKey = Slice("Tuff");
    Slice endKey   = Slice("Zero");

    std::vector<KeyValuePair> pairs = arena->scan(beginKey, endKey, 10);
    std::vector<KeyValuePair> expected = {};

    ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_UpdateTheValueOfAMatchingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  update(arena, "HDD", "Hard drive");

  std::pair<Slice, bool> valueByExistence = arena->getBy(Slice("HDD"));

  ASSERT_EQ(Slice("Hard drive"), valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_UpdateTheValueOfAMatchingKeyInLeafNode) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  update(arena, "HDD", "Hard drive");

  Slice actual = static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy("HDD", stringKeyComparator()).first;
  ASSERT_EQ(Slice("Hard drive"), actual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_DeleteValueOfAMatchingKeyInBetween) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  Slice key = Slice("Pmem");
  deleteBy(arena, key.cdata());

  ASSERT_FALSE(arena->getBy(key).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key, stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_DeleteValueOfAMatchingKeyInEnd) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  Slice key = Slice("SDD");
  deleteBy(arena, key.cdata());

  ASSERT_FALSE(arena->getBy(key).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key, stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_DeleteValueOfAMatchingKeyInBeginning) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  Slice key = Slice("HDD");
  arena->deleteBy(key);

  ASSERT_FALSE(arena->getBy(key).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key, stringKeyComparator()).second);
}
