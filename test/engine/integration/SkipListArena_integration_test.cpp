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
    arena->put(key, value, KeyValueSize(strlen(key) + 1, strlen(value) + 1), new LevelGenerator(6));
}

void update(SkipListArena* arena, const char* key, const char* value) {
    arena->update(key, value, KeyValueSize(strlen(key) + 1, strlen(value) + 1));
}

void deleteBy(SkipListArena* arena, const char* key) {
    arena->deleteBy(key);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_PutASingleKeyValuePair) {
    SkipListNode* sentinel    = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    SkipListArena* arena = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

    put(arena, "HDD", "Hard disk drive");

    std::pair<const char*, bool> valueByExistence = arena->getBy("HDD");

    ASSERT_EQ("Hard disk drive", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_PutMultipleKeyValuePairs) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");

  ASSERT_EQ("Hard disk drive", std::string(arena->getBy("HDD").first));
  ASSERT_EQ("Solid state drive", std::string(arena->getBy("SDD").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_GetByKeyForAnExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");

  ASSERT_EQ("Hard disk drive",   std::string(arena->getBy("HDD").first));
  ASSERT_EQ("Solid state drive", std::string(arena->getBy("SDD").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_GetByKeyForANonExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");

  ASSERT_EQ("", std::string(arena->getBy("Pmem").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_MultiGet) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  std::vector<const char*> keys = {"HDD", "SDD", "Pmem", "DoesNotExist"};
  std::vector<std::pair<const char*, bool>> result = arena->multiGet(keys);
  std::vector<std::pair<std::string, bool>> resultTransformed(result.size());
  std::transform(result.begin(), result.end(), resultTransformed.begin(), [](std::pair<std::string, bool> pair){
        return std::make_pair(std::string(pair.first), pair.second);
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

  std::string beginKey = "Pmem";
  std::string endKey = "SDD";

  std::vector<KeyValuePair> pairs = arena->scan(beginKey.c_str(), endKey.c_str(), 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent memory")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_ScanWithBeginKeyNotPresent) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  std::string beginKey = "RAM";
  std::string endKey   = "Tuff";

  std::vector<KeyValuePair> pairs = arena->scan(beginKey.c_str(), endKey.c_str(), 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("SDD", "Solid state drive")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_ScanWithBeginKeyOutsideTheBounds) {
    SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

    put(arena, "HDD", "Hard disk drive");
    put(arena, "SDD", "Solid state drive");
    put(arena, "Pmem", "Persistent memory");

    std::string beginKey = "Tuff";
    std::string endKey   = "Zero";

    std::vector<KeyValuePair> pairs = arena->scan(beginKey.c_str(), endKey.c_str(), 10);
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

  std::pair<const char*, bool> valueByExistence = arena->getBy("HDD");

  ASSERT_EQ("Hard drive", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_UpdateTheValueOfAMatchingKeyInLeafNode) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  update(arena, "HDD", "Hard drive");

  const char* actual = static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy("HDD", stringKeyComparator()).first;
  ASSERT_EQ("Hard drive", std::string(actual));
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_DeleteValueOfAMatchingKeyInBetween) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  std::string key = "Pmem";
  deleteBy(arena, key.c_str());

  ASSERT_FALSE(arena->getBy(key.c_str()).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key.c_str(), stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_DeleteValueOfAMatchingKeyInEnd) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  std::string key = "SDD";
  deleteBy(arena, key.c_str());

  ASSERT_FALSE(arena->getBy(key.c_str()).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key.c_str(), stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListArena_DeleteValueOfAMatchingKeyInBeginning) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListArena* arena      = new SkipListArena(sentinel, stringKeyComparator(), PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(arena, "HDD", "Hard disk drive");
  put(arena, "SDD", "Solid state drive");
  put(arena, "Pmem", "Persistent memory");

  std::string key = "HDD";
  arena->deleteBy(key.c_str());

  ASSERT_FALSE(arena->getBy(key.c_str()).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key.c_str(), stringKeyComparator()).second);
}
