#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/SkipListNodes.h"
#include "../../src/engine/KeyValuePair.h"
#include "./PersistentMemoryPoolFixture.h"
#include "./SkipListNodeTestUtils.h"

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_RightKeyValuePairGivenDownPointerIsNotNull) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive");
  SkipListLeafNode *right = sentinel -> put("SDD", "Solid state drive");
  
  KeyValuePair pair = node -> rightKeyValuePair();
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_RightKeyValuePairGivenRightPointerIsNull) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive");

  KeyValuePair pair = node -> rightKeyValuePair();  
  ASSERT_EQ(KeyValuePair("", ""), node -> rightKeyValuePair());
}


TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_MatchKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_TRUE(matches);
}


TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DoesNotMatchKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  
  SkipListLeafNode *node = sentinel -> put("SDD", "Solid state drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_FALSE(matches);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("SDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  SkipListLeafNode *node = sentinel -> put("SDD", "Solid state drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_PutAKeyValuePair1) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");

  std::string key = "SDD";
  std::pair<std::string, bool> valueByExistence = sentinel -> getBy(key);
  
  ASSERT_EQ("Solid state drive", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_PutAKeyValuePair2) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive");
  sentinel -> put("SDD", "Solid state drive");
  sentinel -> put("Pmem", "Persistent Storage");

  std::string key = "Pmem";
  std::pair<std::string, bool> valueByExistence = sentinel -> getBy(key);
  
  ASSERT_EQ("Persistent Storage", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_GetByKeyForAnExistingKey) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive");
  sentinel -> put("Pmem", "Persistent Storage");

  std::string key = "Pmem";
  std::pair<std::string, bool> valueByExistence = sentinel -> getBy(key);
  
  ASSERT_EQ("Persistent Storage", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_GetByKeyForANonExistingKey) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive");
  sentinel -> put("Pmem", "Persistent Storage");

  std::string key = "SDD";
  std::pair<std::string, bool> valueByExistence = sentinel -> getBy(key);
  
  ASSERT_EQ("", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_UpdateValueOfAMatchingKey) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive");
  sentinel -> put("Pmem", "Persistent Storage");

  sentinel -> update("Pmem", "Persistent Memory");

  std::string key = "Pmem";
  std::pair<std::string, bool> valueByExistence = sentinel -> getBy(key);
  
  ASSERT_EQ("Persistent Memory", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DeleteValueOfAMatchingKeyInTheBeginning) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive");
  sentinel -> put("Pmem", "Persistent Storage");
  sentinel -> put("SDD", "Solid State drive");

  sentinel -> deleteBy("HDD");

  std::string key = "HDD";
  std::pair<std::string, bool> valueByExistence = sentinel -> getBy(key);

  ASSERT_EQ("", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DeleteValueOfAMatchingKeyInBetween) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive");
  sentinel -> put("Pmem", "Persistent Storage");
  sentinel -> put("SDD", "Solid State drive");

  sentinel -> deleteBy("Pmem");

  std::string key = "Pmem";
  std::pair<std::string, bool> valueByExistence = sentinel -> getBy(key);

  ASSERT_EQ("", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DeleteValueOfAMatchingKeyInTheEnd) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("HDD", "Hard disk drive");
  sentinel -> put("Pmem", "Persistent Storage");
  sentinel -> put("SDD", "Solid State drive");

  sentinel -> deleteBy("SDD");

  std::string key = "SDD";
  std::pair<std::string, bool> valueByExistence = sentinel -> getBy(key);

  ASSERT_EQ("", valueByExistence.first);
}
