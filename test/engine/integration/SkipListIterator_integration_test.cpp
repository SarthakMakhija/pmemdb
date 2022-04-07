#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include "../../../src/engine/storage/SkipListInternalNode.h"
#include "../../../src/engine/storage/SkipListLeafNode.h"
#include "../../../src/engine/storage/utils/LevelGenerator.h"
#include "../../../src/engine/db/KeyValuePair.h"
#include "../../../src/engine/db/SkipListIterator.h"
#include "PersistentMemoryPoolFixture.h"
#include "../SkipListNodeTestUtils.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

void put(SkipListIterator iterator, const char* key, const char* value) {
    iterator.put(key, value, KeyValueSize(strlen(key) + 1, strlen(value) + 1), new LevelGenerator(6));
}

void update(SkipListIterator iterator, const char* key, const char* value) {
    iterator.update(key, value, KeyValueSize(strlen(key) + 1, strlen(value) + 1));
}

void deleteBy(SkipListIterator iterator, const char* key) {
    iterator.deleteBy(key);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_PutASingleKeyValuePair) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    put(iterator, "HDD", "Hard disk drive");

    std::pair<const char*, bool> valueByExistence = iterator.getBy("HDD");

    ASSERT_EQ("Hard disk drive", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_PutMultipleKeyValuePairs) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");

  ASSERT_EQ("Hard disk drive", std::string(iterator.getBy("HDD").first));
  ASSERT_EQ("Solid state drive", std::string(iterator.getBy("SDD").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForAnExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");

  ASSERT_EQ("Hard disk drive",   std::string(iterator.getBy("HDD").first));
  ASSERT_EQ("Solid state drive", std::string(iterator.getBy("SDD").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForANonExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");

  ASSERT_EQ("", std::string(iterator.getBy("Pmem").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_MultiGet) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");
  put(iterator, "Pmem", "Persistent memory");

  std::vector<const char*> keys = {"HDD", "SDD", "Pmem", "DoesNotExist"};
  std::vector<std::pair<const char*, bool>> result = iterator.multiGet(keys);
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

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyPresent) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");
  put(iterator, "Pmem", "Persistent memory");

  std::string beginKey = "Pmem";
  std::string endKey = "SDD";

  std::vector<KeyValuePair> pairs = iterator.scan(beginKey.c_str(), endKey.c_str(), 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent memory")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyNotPresent) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");
  put(iterator, "Pmem", "Persistent memory");

  std::string beginKey = "RAM";
  std::string endKey   = "Tuff";

  std::vector<KeyValuePair> pairs = iterator.scan(beginKey.c_str(), endKey.c_str(), 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("SDD", "Solid state drive")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyOutsideTheBounds) {
    SkipListInternalNode* sentinel = newSentinelInternalNode(6);
    SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

    put(iterator, "HDD", "Hard disk drive");
    put(iterator, "SDD", "Solid state drive");
    put(iterator, "Pmem", "Persistent memory");

    std::string beginKey = "Tuff";
    std::string endKey   = "Zero";

    std::vector<KeyValuePair> pairs = iterator.scan(beginKey.c_str(), endKey.c_str(), 10);
    std::vector<KeyValuePair> expected = {};

    ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_UpdateTheValueOfAMatchingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");
  put(iterator, "Pmem", "Persistent memory");

  update(iterator, "HDD", "Hard drive");

  std::pair<const char*, bool> valueByExistence = iterator.getBy("HDD");

  ASSERT_EQ("Hard drive", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_UpdateTheValueOfAMatchingKeyInLeafNode) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");
  put(iterator, "Pmem", "Persistent memory");

  update(iterator, "HDD", "Hard drive");

  const char* actual = static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy("HDD", stringKeyComparator()).first;
  ASSERT_EQ("Hard drive", std::string(actual));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInBetween) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");
  put(iterator, "Pmem", "Persistent memory");

  std::string key = "Pmem";
  deleteBy(iterator, key.c_str());

  ASSERT_FALSE(iterator.getBy(key.c_str()).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key.c_str(), stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInEnd) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");
  put(iterator, "Pmem", "Persistent memory");

  std::string key = "SDD";
  deleteBy(iterator, key.c_str());

  ASSERT_FALSE(iterator.getBy(key.c_str()).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key.c_str(), stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInBeginning) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  put(iterator, "HDD", "Hard disk drive");
  put(iterator, "SDD", "Solid state drive");
  put(iterator, "Pmem", "Persistent memory");

  std::string key = "HDD";
  iterator.deleteBy(key.c_str());

  ASSERT_FALSE(iterator.getBy(key.c_str()).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key.c_str(), stringKeyComparator()).second);
}
