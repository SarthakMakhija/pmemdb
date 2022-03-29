#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "../../src/engine/SkipListIterator.h"
#include "./PersistentMemoryPoolFixture.h"
#include "./SkipListNodeTestUtils.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_PutASingleKeyValuePair) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel, stringKeyComparator());

    iterator.put("HDD", "Hard disk drive", 0.5);

    std::pair<const char*, bool> valueByExistence = iterator.getBy("HDD");

    ASSERT_EQ("Hard disk drive", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_PutMultipleKeyValuePairs) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);

  ASSERT_EQ("Hard disk drive", std::string(iterator.getBy("HDD").first));
  ASSERT_EQ("Solid state drive", std::string(iterator.getBy("SDD").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForAnExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);

  ASSERT_EQ("Hard disk drive",   std::string(iterator.getBy("HDD").first));
  ASSERT_EQ("Solid state drive", std::string(iterator.getBy("SDD").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForANonExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);

  ASSERT_EQ("", std::string(iterator.getBy("Pmem").first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_MultiGet) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::vector<const char*> keys = {"HDD", "SDD", "Pmem", "DoesNotExist"};
  std::vector<std::pair<std::string, bool>> result = iterator.multiGet(keys);

  std::vector<std::pair<std::string, bool>> expected = {
                            std::make_pair("", false),
                            std::make_pair("Hard disk drive", true),
                            std::make_pair("Persistent memory", true),
                            std::make_pair("Solid state drive", true)
  };

  ASSERT_EQ(expected, result);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyPresent) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::string beginKey = "Pmem";
  std::string endKey = "SDD";

  std::vector<KeyValuePair> pairs = iterator.scan(beginKey.c_str(), endKey.c_str(), 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent memory")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyNotPresent) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::string beginKey = "RAM";
  std::string endKey   = "Tuff";

  std::vector<KeyValuePair> pairs = iterator.scan(beginKey.c_str(), endKey.c_str(), 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("SDD", "Solid state drive")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyOutsideTheBounds) {
    SkipListInternalNode* sentinel = newSentinelInternalNode(6);
    SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

    iterator.put("HDD", "Hard disk drive", 0.5);
    iterator.put("SDD", "Solid state drive", 0.5);
    iterator.put("Pmem", "Persistent memory", 0.5);

    std::string beginKey = "Tuff";
    std::string endKey   = "Zero";

    std::vector<KeyValuePair> pairs = iterator.scan(beginKey.c_str(), endKey.c_str(), 10);
    std::vector<KeyValuePair> expected = {};

    ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_UpdateTheValueOfAMatchingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  iterator.update("HDD", "Hard drive");

  std::pair<const char*, bool> valueByExistence = iterator.getBy("HDD");

  ASSERT_EQ("Hard drive", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_UpdateTheValueOfAMatchingKeyInLeafNode) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.6);
  iterator.put("SDD", "Solid state drive", 0.6);
  iterator.put("Pmem", "Persistent memory", 0.6);

  iterator.update("HDD", "Hard drive");

  const char* actual = static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy("HDD", stringKeyComparator()).first;
  ASSERT_EQ("Hard drive", std::string(actual));
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInBetween) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::string key = "Pmem";
  iterator.deleteBy(key.c_str());

  ASSERT_FALSE(iterator.getBy(key.c_str()).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key.c_str(), stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInEnd) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::string key = "SDD";
  iterator.deleteBy(key.c_str());

  ASSERT_FALSE(iterator.getBy(key.c_str()).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key.c_str(), stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInBeginning) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel, stringKeyComparator());

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::string key = "HDD";
  iterator.deleteBy(key.c_str());

  ASSERT_FALSE(iterator.getBy(key.c_str()).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key.c_str(), stringKeyComparator()).second);
}
