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
  sentinelInternal -> put("HDD", "Hard disk drive");
  sentinelInternal -> put("SDD", "Solid state drive");

  std::string key = "SDD";
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), existenceByNode.first -> keyValuePair());
}


TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForANonExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  sentinelInternal -> put("HDD", "Hard disk drive");
  sentinelInternal -> put("SDD", "Solid state drive");

  std::string key = "Pmem";
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);
  
  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_UpdateValueOfAMatchingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
 
  sentinelInternal -> put("HDD", "Hard disk drive");
  sentinelInternal -> put("SDD", "Solid state drive");
  sentinelInternal -> put("Pmem", "Persistent Memory");

  std::string key = "SDD";
  sentinelInternal -> update(key, "Solid Drive");

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);
  ASSERT_EQ(KeyValuePair("SDD", "Solid Drive"), existenceByNode.first -> keyValuePair());
}

/*
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

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteRange4) {
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
  std::string endKey   = "F";
  sentinelInternal -> deleteRange(beginKey, endKey);

  std::vector<std::string> missingKeys = {"C", "D"};
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(sentinelInternal -> getBy(missingKey).second);
  }

  std::vector<std::string> presentKeys = {"A"};
  for (auto presentKey: presentKeys) {
    ASSERT_EQ(presentKey,  sentinelInternal -> getBy(presentKey).first -> keyValuePair().getValue());
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteRange5) {
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

  std::string beginKey = "A";
  std::string endKey   = "F";
  sentinelInternal -> deleteRange(beginKey, endKey);

  std::vector<std::string> missingKeys = {"A", "C", "D"};
  for (auto missingKey: missingKeys) {
    ASSERT_FALSE(sentinelInternal -> getBy(missingKey).second);
  }
}
*/