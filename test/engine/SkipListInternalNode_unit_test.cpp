#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "SkipListNodeTestUtils.h"
#include  "PersistentMemoryPoolFixture.h"

TEST(SkipListInternalNode, AddNewNodeToRight) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  node -> addToRightWith("SDD", "Solid state drive");

  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListInternalNode, UpdateDownPointer) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *down = new SkipListInternalNode("SDD", "Solid state drive");
  node -> updateDown(down);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), downKeyValuePairOf(node));
}

TEST(SkipListInternalNode, UpdateRightPointer) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *right = new SkipListInternalNode("SDD", "Solid state drive");
  node -> updateRight(right);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListInternalNode, RightKeyValuePairGivenDownPointerIsNotNull) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *right = new SkipListInternalNode("SDD", "Solid state drive");
  node -> updateRight(right);
  
  KeyValuePair pair = node -> rightKeyValuePair();
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListInternalNode, RightKeyValuePairGivenRightPointerIsNull) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  
  KeyValuePair pair = node -> rightKeyValuePair();
  
  ASSERT_EQ(KeyValuePair("", ""), node -> rightKeyValuePair());
}

TEST(SkipListInternalNode, MatchKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_TRUE(matches);
}

TEST(SkipListInternalNode, DoesNotMatchKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("SDD", "Solid state drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_FALSE(matches);
}

TEST(SkipListInternalNode, NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("SDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("SDD", "Solid state drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForAnExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  sentinelInternal -> updateRight(internalFirst);
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafSecond);

  std::string key = "SDD";
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), existenceByNode.first -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForANonExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  sentinelInternal -> updateRight(internalFirst);
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("Pmem", "Persistent Memory");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);

  std::string key = "Pmem";
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);
  
  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForANonExistingKeyInInternalNodeAndReturnTheLeafNodeToBeginTheGetOperation) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  sentinelInternal -> updateRight(internalFirst);
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("Pmem", "Persistent Memory");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);

  std::string key = "Pmem";
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);
  
  ASSERT_EQ(KeyValuePair("HDD", "Hard disk drive"), existenceByNode.first -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_InsertPositionInInternalNodeAfterWhichKeyValueWouldBePut1) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  sentinelInternal -> updateRight(internalFirst);
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("Pmem", "Persistent Memory");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);

  std::string key = "Ignite";
  std::pair<std::vector<SkipListNode*>, SkipListNode*> leafNodeByInternalNodePositions = sentinelInternal -> insertPositionsFor(key);

  SkipListNode* internalNode = leafNodeByInternalNodePositions.first.back();
  ASSERT_EQ(KeyValuePair("HDD", "Hard disk drive"), internalNode -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_InsertPositionInInternalNodeAfterWhichKeyValueWouldBePut2) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  sentinelInternal -> updateRight(internalFirst);
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("Pmem", "Persistent Memory");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);

  std::string key = "Tuff";
  std::pair<std::vector<SkipListNode*>, SkipListNode*> leafNodeByInternalNodePositions = sentinelInternal -> insertPositionsFor(key);
  SkipListNode* internalNode = leafNodeByInternalNodePositions.first.back();

  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), internalNode -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_UpdateValueOfAMatchingKeyInInternalNodeAndReturnThePositionToStartLeafNodeUpdate) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  sentinelInternal -> updateRight(internalFirst);
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("Pmem", "Persistent Memory");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);

  std::string key = "SDD";
  SkipListNode* leafNode = sentinelInternal -> update(key, "Solid Drive");
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), leafNode -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInInternalNodeAndReturnThePositionToStartLeafNodeDelete) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  sentinelInternal -> updateRight(internalFirst);
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("Pmem", "Persistent Memory");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);

  std::string key = "SDD";
  SkipListNode* leafNode = sentinelInternal -> deleteBy(key);

  ASSERT_EQ(KeyValuePair("HDD", "Hard disk drive"), leafNode -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteRange1) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("A", "A");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("B", "B");
  SkipListInternalNode *internalThird = new SkipListInternalNode("D", "D");
  
  sentinelInternal -> updateRight(internalFirst);
  internalFirst -> updateRight(internalSecond);
  internalSecond -> updateRight(internalThird);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("A", "A");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("B", "B");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("C", "C");
  SkipListLeafNode *leafFourth = sentinelLeaf -> put("D", "D");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafSecond);
  internalThird -> updateDown(leafFourth);

  std::string beginKey = "B";
  std::string endKey   = "D";
  sentinelInternal -> deleteRange(beginKey, endKey);

  std::vector<std::string> missingKeys = {"B"};
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(sentinelInternal -> getBy(missingKey).second);
  }

  std::vector<std::string> presentKeys = {"A", "D"};
  for (auto presentKey: presentKeys) {
    ASSERT_EQ(presentKey,  sentinelInternal -> getBy(presentKey).first -> keyValuePair().getValue());
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteRange2) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("A", "A");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("B", "B");
  SkipListInternalNode *internalThird = new SkipListInternalNode("D", "D");
  
  sentinelInternal -> updateRight(internalFirst);
  internalFirst -> updateRight(internalSecond);
  internalSecond -> updateRight(internalThird);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("A", "A");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("B", "B");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("C", "C");
  SkipListLeafNode *leafFourth = sentinelLeaf -> put("D", "D");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafSecond);
  internalThird -> updateDown(leafFourth);

  std::string beginKey = "A";
  std::string endKey   = "D";
  sentinelInternal -> deleteRange(beginKey, endKey);

  std::vector<std::string> missingKeys = {"A", "B"};
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(sentinelInternal -> getBy(missingKey).second);
  }

  std::vector<std::string> presentKeys = {"D"};
  for (auto presentKey: presentKeys) {
    ASSERT_EQ(presentKey,  sentinelInternal -> getBy(presentKey).first -> keyValuePair().getValue());
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteRange3) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("A", "A");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("C", "C");
  SkipListInternalNode *internalThird = new SkipListInternalNode("D", "D");
  
  sentinelInternal -> updateRight(internalFirst);
  internalFirst -> updateRight(internalSecond);
  internalSecond -> updateRight(internalThird);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("A", "A");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("B", "B");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("C", "C");
  SkipListLeafNode *leafFourth = sentinelLeaf -> put("D", "D");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);
  internalThird -> updateDown(leafFourth);

  std::string beginKey = "B";
  std::string endKey   = "D";
  sentinelInternal -> deleteRange(beginKey, endKey);

  std::vector<std::string> missingKeys = {"C"};
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(sentinelInternal -> getBy(missingKey).second);
  }

  std::vector<std::string> presentKeys = {"A", "D"};
  for (auto presentKey: presentKeys) {
    ASSERT_EQ(presentKey,  sentinelInternal -> getBy(presentKey).first -> keyValuePair().getValue());
  }
}