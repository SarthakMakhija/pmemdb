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

    iterator.put("HDD", "Hard disk drive");

    std::pair<std::string, bool> valueByExistence = iterator.getBy("HDD");
    ASSERT_EQ("Hard disk drive", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_PutMultipleKeyValuePairs) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");

  ASSERT_EQ("Hard disk drive", iterator.getBy("HDD").first);
  ASSERT_EQ("Solid state drive", iterator.getBy("SDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForAnExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");

  ASSERT_EQ("Hard disk drive",   iterator.getBy("HDD").first);
  ASSERT_EQ("Solid state drive", iterator.getBy("SDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForANonExistingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");

  ASSERT_EQ("", iterator.getBy("Pmem").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_MultiGet) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");
  iterator.put("Pmem", "Persistent memory");

  std::vector<std::string> keys = {"HDD", "SDD", "Pmem", "DoesNotExist"};
  std::vector<std::pair<std::string, bool>> result = iterator.multiGet(keys);
  
  std::vector<std::pair<std::string, bool>> expected = {
                            std::make_pair("Hard disk drive", true), 
                            std::make_pair("Solid state drive", true),
                            std::make_pair("Persistent memory", true),
                            std::make_pair("", false)
  };

  ASSERT_EQ(expected, result);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyPresent) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");
  iterator.put("Pmem", "Persistent memory");

  std::string beginKey = "Pmem";
  std::string endKey = "SDD";

  std::vector<KeyValuePair> pairs = iterator.scan(beginKey, endKey, 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent memory")};
  
  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyNotPresent) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");
  iterator.put("Pmem", "Persistent memory");
 
  std::string beginKey = "RAM";
  std::string endKey   = "Tuff";

  std::vector<KeyValuePair> pairs = iterator.scan(beginKey, endKey, 10);
  std::vector<KeyValuePair> expected = {KeyValuePair("SDD", "Solid state drive")};
  
  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_UpdateTheValueOfAMatchingKey) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");
  iterator.put("Pmem", "Persistent memory");

  iterator.update("HDD", "Hard drive");

  std::pair<std::string, bool> valueByExistence = iterator.getBy("HDD");
  ASSERT_EQ("Hard drive", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_UpdateTheValueOfAMatchingKeyInLeafNode) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");
  iterator.put("Pmem", "Persistent memory");

  iterator.update("HDD", "Hard drive");

  ASSERT_EQ("Hard drive", static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy("HDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInBetween) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");
  iterator.put("Pmem", "Persistent memory");

  std::string key = "Pmem";
  iterator.deleteBy(key);

  ASSERT_FALSE(iterator.getBy(key).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInEend) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");
  iterator.put("Pmem", "Persistent memory");

  std::string key = "SDD";
  iterator.deleteBy(key);

  ASSERT_FALSE(iterator.getBy(key).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInBeginning) {
  SkipListInternalNode* sentinel = newSentinelInternalNode(6);
  SkipListIterator iterator      = SkipListIterator(sentinel);

  iterator.put("HDD", "Hard disk drive");
  iterator.put("SDD", "Solid state drive");
  iterator.put("Pmem", "Persistent memory");

  std::string key = "HDD";
  iterator.deleteBy(key);

  ASSERT_FALSE(iterator.getBy(key).second);
  ASSERT_FALSE(static_cast<SkipListLeafNode*>(sentinel -> getDown()) -> getBy(key).second);
}