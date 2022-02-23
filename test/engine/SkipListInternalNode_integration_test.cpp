#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "SkipListNodeTestUtils.h"
#include  "PersistentMemoryPoolFixture.h"

TEST(SkipListInternalNode, UpdateDownPointer) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive",  4);
  SkipListLeafNode *down     = new SkipListLeafNode();
  node -> attach(down);
  
  ASSERT_TRUE(node -> getDown() != nullptr);
}

TEST(SkipListInternalNode, MatchKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive", 4);
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_TRUE(matches);
}

TEST(SkipListInternalNode, DoesNotMatchKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("SDD", "Solid state drive", 4);
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_FALSE(matches);
}

TEST(SkipListInternalNode, NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive",   4);
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("SDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive", 4);
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("SDD", "Solid state drive", 4);
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForAnExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  sentinelInternal -> put("HDD", "Hard disk drive", 0.5);
  sentinelInternal -> put("SDD", "Solid state drive", 0.5);

  std::string key = "SDD";
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), existenceByNode.first -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForANonExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  sentinelInternal -> put("HDD", "Hard disk drive", 0.5);
  sentinelInternal -> put("SDD", "Solid state drive", 0.5);

  std::string key = "Pmem";
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);
  
  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_UpdateValueOfAMatchingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  sentinelInternal -> put("HDD", "Hard disk drive", 0.5);
  sentinelInternal -> put("SDD", "Solid state drive", 0.5);
  sentinelInternal -> put("Pmem", "Persistent Memory", 0.5);

  std::string key = "SDD";
  sentinelInternal -> update(key, "Solid Drive");

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);

  ASSERT_EQ(KeyValuePair("SDD", "Solid Drive"), existenceByNode.first -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInBetweenInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
 
  sentinelInternal -> put("HDD", "Hard disk drive", 0.5);
  sentinelInternal -> put("SDD", "Solid state drive", 0.5);
  sentinelInternal -> put("Pmem", "Persistent Memory", 0.5);

  std::string key = "Pmem";
  SkipListNode* leaf = sentinelInternal -> deleteBy(key);

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInEndInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
 
  sentinelInternal -> put("HDD", "Hard disk drive", 0.5);
  sentinelInternal -> put("SDD", "Solid state drive", 0.5);
  sentinelInternal -> put("Pmem", "Persistent Memory", 0.5);

  std::string key = "SDD";
  SkipListNode* leaf = sentinelInternal -> deleteBy(key);

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInBeginningInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
 
  sentinelInternal -> put("HDD", "Hard disk drive", 0.5);
  sentinelInternal -> put("SDD", "Solid state drive", 0.5);
  sentinelInternal -> put("Pmem", "Persistent Memory", 0.5);

  std::string key = "HDD";
  SkipListNode* leaf = sentinelInternal -> deleteBy(key);

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteRange1) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
 
  sentinelInternal -> put("A", "A", 0.5);
  sentinelInternal -> put("B", "B", 0.5);
  sentinelInternal -> put("C", "C", 0.5);
  sentinelInternal -> put("D", "D", 0.5);

  std::string beginKey = "B";
  std::string endKey   = "D";
  sentinelInternal -> deleteRange(beginKey, endKey);

  std::vector<std::string> missingKeys = {"B", "C"};
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(sentinelInternal -> getBy(missingKey).second);
  }

  std::vector<std::string> presentKeys = {"A", "D"};
  for (auto presentKey: presentKeys) {
    ASSERT_EQ(presentKey,  sentinelInternal -> getBy(presentKey).first -> keyValuePair().getValue());
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteRange2) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
 
  sentinelInternal -> put("A", "A", 0.5);
  sentinelInternal -> put("B", "B", 0.5);
  sentinelInternal -> put("C", "C", 0.5);
  sentinelInternal -> put("D", "D", 0.5);

  std::string beginKey = "A";
  std::string endKey   = "D";
  sentinelInternal -> deleteRange(beginKey, endKey);

  std::vector<std::string> missingKeys = {"A", "B", "C"};
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(sentinelInternal -> getBy(missingKey).second);
  }

  std::vector<std::string> presentKeys = {"D"};
  for (auto presentKey: presentKeys) {
    ASSERT_EQ(presentKey,  sentinelInternal -> getBy(presentKey).first -> keyValuePair().getValue());
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteRange3) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
 
  sentinelInternal -> put("A", "A", 0.5);
  sentinelInternal -> put("B", "B", 0.5);
  sentinelInternal -> put("C", "C", 0.5);
  sentinelInternal -> put("D", "D", 0.5);

  std::string beginKey = "B";
  std::string endKey   = "F";
  sentinelInternal -> deleteRange(beginKey, endKey);

  std::vector<std::string> missingKeys = {"B", "C", "D"};
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(sentinelInternal -> getBy(missingKey).second);
  }

  std::vector<std::string> presentKeys = {"A"};
  for (auto presentKey: presentKeys) {
    ASSERT_EQ(presentKey,  sentinelInternal -> getBy(presentKey).first -> keyValuePair().getValue());
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteRange4) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
 
  sentinelInternal -> put("A", "A", 0.5);
  sentinelInternal -> put("B", "B", 0.5);
  sentinelInternal -> put("C", "C", 0.5);
  sentinelInternal -> put("D", "D", 0.5);

  std::string beginKey = "A";
  std::string endKey   = "F";
  sentinelInternal -> deleteRange(beginKey, endKey);

  std::vector<std::string> missingKeys = {"A", "B", "C", "D"};
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(sentinelInternal -> getBy(missingKey).second);
  }
}