#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "../../src/engine/SkipListIterator.h"
#include "./PersistentMemoryPoolFixture.h"
#include "./SkipListNodeTestUtils.h"

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_PutASingleKeyValuePair) {
    SkipListNode* sentinel    = newSentinelInternalNode(6);
    SkipListIterator iterator = SkipListIterator(sentinel);

    iterator.put("HDD", "Hard disk drive", 0.5);

    std::pair<std::string, bool> valueByExistence = iterator.getBy("HDD");

    ASSERT_EQ("Hard disk drive", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_PutMultipleKeyValuePairs) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);

  ASSERT_EQ("Hard disk drive", iterator.getBy("HDD").first);
  ASSERT_EQ("Solid state drive", iterator.getBy("SDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForAnExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);

  ASSERT_EQ("Hard disk drive",   iterator.getBy("HDD").first);
  ASSERT_EQ("Solid state drive", iterator.getBy("SDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForANonExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);

  ASSERT_EQ("", iterator.getBy("Pmem").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_MultiGet) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::vector<std::string> keys = {"HDD", "SDD", "Pmem", "DoesNotExist"};
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
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::string beginKey = "Pmem";
  std::string endKey = "SDD";

  std::vector<KeyValuePair> pairs = iterator.scan(beginKey, endKey, 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent memory")};
  
  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyNotPresent) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);
 
  std::string beginKey = "RAM";
  std::string endKey   = "Tuff";

  std::vector<KeyValuePair> pairs = iterator.scan(beginKey, endKey, 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("SDD", "Solid state drive")};
  
  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_UpdateTheValueOfAMatchingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  iterator.update("HDD", "Hard drive");

  std::pair<std::string, bool> valueByExistence = iterator.getBy("HDD");

  ASSERT_EQ("Hard drive", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_UpdateTheValueOfAMatchingKeyInLeafNode) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.6);
  iterator.put("SDD", "Solid state drive", 0.6);
  iterator.put("Pmem", "Persistent memory", 0.6);

  iterator.update("HDD", "Hard drive");

  ASSERT_EQ("Hard drive", static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy("HDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInBetween) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::string key = "Pmem";
  iterator.deleteBy(key);

  ASSERT_FALSE(iterator.getBy(key).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInEnd) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::string key = "SDD";
  iterator.deleteBy(key);

  ASSERT_FALSE(iterator.getBy(key).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInBeginning) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive", 0.5);
  iterator.put("SDD", "Solid state drive", 0.5);
  iterator.put("Pmem", "Persistent memory", 0.5);

  std::string key = "HDD";
  iterator.deleteBy(key);

  ASSERT_FALSE(iterator.getBy(key).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteRange1) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("A", "A", 0.5);
  iterator.put("B", "B", 0.5);
  iterator.put("C", "C", 0.5);

  iterator.deleteRange("B", "D");

  std::vector<std::string> missingKeys = {"B", "C"};
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(iterator.getBy(missingKey).second);
  }
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(missingKey).second);
  }

  std::vector<std::string> presentKeys = {"A"};
  for (auto presentKey: presentKeys) {
    ASSERT_EQ(presentKey,  iterator.getBy(presentKey).first);
  }
  for (auto presentKey: presentKeys) {
    ASSERT_EQ(presentKey,  static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(presentKey).first);
  }
}