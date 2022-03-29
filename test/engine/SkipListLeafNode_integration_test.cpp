#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "./PersistentMemoryPoolFixture.h"
#include "./SkipListNodeTestUtils.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_RightKeyValuePairGivenDownPointerIsNotNull) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive", stringKeyComparator()).first;
  SkipListLeafNode *right = sentinel -> put("SDD", "Solid state drive", stringKeyComparator()).first;
  
  KeyValuePair pair = node -> rightKeyValuePair();
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_RightKeyValuePairGivenRightPointerIsNull) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive", stringKeyComparator()).first;

  KeyValuePair pair = node -> rightKeyValuePair();  
  ASSERT_EQ(KeyValuePair("", ""), node -> rightKeyValuePair());
}


TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_MatchKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive", stringKeyComparator()).first;
  bool matches = node -> matchesKey("HDD", stringKeyComparator());
  
  ASSERT_TRUE(matches);
}


TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DoesNotMatchKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  
  SkipListLeafNode *node = sentinel -> put("SDD", "Solid state drive", stringKeyComparator()).first;
  bool matches = node -> matchesKey("HDD", stringKeyComparator());
  
  ASSERT_FALSE(matches);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive", stringKeyComparator()).first;
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("SDD", stringKeyComparator());
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive", stringKeyComparator()).first;
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD", stringKeyComparator());
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  SkipListLeafNode *node = sentinel -> put("SDD", "Solid state drive", stringKeyComparator()).first;
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD", stringKeyComparator());
  
  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_PutAKeyValuePair1) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive", stringKeyComparator());

  std::string key = "SDD";
  std::pair<const char*, bool> valueByExistence = sentinel -> getBy(key.c_str(), stringKeyComparator());
  
  ASSERT_EQ("Solid state drive", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_PutAKeyValuePair2) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());
  sentinel -> put("SDD", "Solid state drive", stringKeyComparator());
  sentinel -> put("Pmem", "Persistent Storage", stringKeyComparator());

  std::string key = "Pmem";
  std::pair<const char*, bool> valueByExistence = sentinel -> getBy(key.c_str(), stringKeyComparator());
  
  ASSERT_EQ("Persistent Storage", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_GetByKeyForAnExistingKey) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());
  sentinel -> put("Pmem", "Persistent Storage", stringKeyComparator());

  std::string key = "Pmem";
  std::pair<const char*, bool> valueByExistence = sentinel -> getBy(key.c_str(), stringKeyComparator());
  
  ASSERT_EQ("Persistent Storage", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_GetByKeyForANonExistingKey) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());
  sentinel -> put("Pmem", "Persistent Storage", stringKeyComparator());

  std::string key = "SDD";
  std::pair<const char*, bool> valueByExistence = sentinel -> getBy(key.c_str(), stringKeyComparator());
  
  ASSERT_EQ("", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_ScanWithBeginKeyPresent) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());
  sentinel -> put("Pmem", "Persistent Storage", stringKeyComparator());
  sentinel -> put("RAM", "Random Access Memory", stringKeyComparator());
  sentinel -> put("SDD", "Solid state drive", stringKeyComparator());

  std::string beginKey = "Pmem";
  std::string endKey = "SDD";

  std::vector<KeyValuePair> pairs = sentinel -> scan(beginKey.c_str(), endKey.c_str(), 10, stringKeyComparator());
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent Storage"), KeyValuePair("RAM", "Random Access Memory")};
  
  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_ScanWithBeginKeyNotPresent) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());
  sentinel -> put("RAM", "Random Access Memory", stringKeyComparator());
  sentinel -> put("SDD", "Solid state drive", stringKeyComparator());

  std::string beginKey = "Pmem";
  std::string endKey = "SDD";

  std::vector<KeyValuePair> pairs = sentinel -> scan(beginKey.c_str(), endKey.c_str(), 10, stringKeyComparator());
  std::vector<KeyValuePair> expected = {KeyValuePair("RAM", "Random Access Memory")};
  
  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_ScanWithMaxPairsAs1) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());
  sentinel -> put("Pmem", "Persistent Storage", stringKeyComparator());
  sentinel -> put("RAM", "Random Access Memory", stringKeyComparator());
  sentinel -> put("SDD", "Solid state drive", stringKeyComparator());

  std::string beginKey = "Pmem";
  std::string endKey = "SDD";

  std::vector<KeyValuePair> pairs = sentinel -> scan(beginKey.c_str(), endKey.c_str(), 1, stringKeyComparator());
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent Storage")};
  
  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_UpdateValueOfAMatchingKey) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());
  sentinel -> put("Pmem", "Persistent Storage", stringKeyComparator());

  sentinel -> update("Pmem", "Persistent Memory", stringKeyComparator());

  std::string key = "Pmem";
  std::pair<const char*, bool> valueByExistence = sentinel -> getBy(key.c_str(), stringKeyComparator());
  
  ASSERT_EQ("Persistent Memory", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DeleteValueOfAMatchingKeyInTheBeginning) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());
  sentinel -> put("Pmem", "Persistent Storage", stringKeyComparator());
  sentinel -> put("SDD", "Solid State drive", stringKeyComparator());

  sentinel -> deleteBy("HDD", stringKeyComparator());

  std::string key = "HDD";
  std::pair<const char*, bool> valueByExistence = sentinel -> getBy(key.c_str(), stringKeyComparator());

  ASSERT_EQ("", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DeleteValueOfAMatchingKeyInBetween) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());
  sentinel -> put("Pmem", "Persistent Storage", stringKeyComparator());
  sentinel -> put("SDD", "Solid State drive", stringKeyComparator());

  sentinel -> deleteBy("Pmem", stringKeyComparator());

  std::string key = "Pmem";
  std::pair<const char*, bool> valueByExistence = sentinel -> getBy(key.c_str(), stringKeyComparator());

  ASSERT_EQ("", std::string(valueByExistence.first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DeleteValueOfAMatchingKeyInTheEnd) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());
  sentinel -> put("Pmem", "Persistent Storage", stringKeyComparator());
  sentinel -> put("SDD", "Solid State drive", stringKeyComparator());

  sentinel -> deleteBy("SDD", stringKeyComparator());

  std::string key = "SDD";
  std::pair<const char*, bool> valueByExistence = sentinel -> getBy(key.c_str(), stringKeyComparator());

  ASSERT_EQ("", std::string(valueByExistence.first));
}