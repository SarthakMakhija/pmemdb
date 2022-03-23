#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "SkipListNodeTestUtils.h"
#include  "PersistentMemoryPoolFixture.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

void put(SkipListInternalNode* node, std::string key, std::string value, double probability = 0.5) {
  PutPosition putPosition = node -> putPositionOf(key, probability);
  if (putPosition.newLevel != -1) {
      std::pair < SkipListLeafNode *, Status > statusNodePair = static_cast<SkipListLeafNode *>(putPosition.leaf)->put(key, value);
      SkipListLeafNode *newLeaf = statusNodePair.first;
      SkipListNode *newInternal =  node -> put(key, value, putPosition.positions, putPosition.newLevel);
      static_cast<SkipListInternalNode *>(newInternal) -> attach(newLeaf);
  }
}

void deleteBy(SkipListInternalNode* node, std::string key) {
  DeletePosition deletePosition = node -> deletePositionOf(key);
  if (deletePosition.internal != nullptr) {
    static_cast<SkipListInternalNode*>(deletePosition.internal) -> deleteBy(key, deletePosition.positions, deletePosition.deleteLevel);
  }
}

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
  put(sentinelInternal, "HDD", "Hard disk drive");
  put(sentinelInternal, "SDD", "Solid state drive");

  std::string key = "SDD";
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);

  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), existenceByNode.first -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForANonExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  put(sentinelInternal, "HDD", "Hard disk drive");
  put(sentinelInternal, "SDD", "Solid state drive");

  std::string key = "Pmem";
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_AttemptsToPutSameKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);

  put(sentinelInternal, "HDD", "Hard disk drive");
  put(sentinelInternal, "HDD", "Hard disk drive");

  std::string key = "HDD";
  ASSERT_EQ("Hard disk drive", sentinelInternal -> getBy(key).first -> keyValuePair().getValue());
}

/*
TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_UpdateValueOfAMatchingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  put(sentinelInternal, "HDD", "Hard disk drive");
  put(sentinelInternal, "SDD", "Solid state drive");
  put(sentinelInternal, "Pmem", "Persistent Memory");

  std::string key = "SDD";
  UpdatePosition updatePosition = sentinelInternal -> updatePositionOf(key);
  static_cast<SkipListInternalNode*>(updatePosition.internal) -> update(key, "Solid Drive");

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);

  ASSERT_EQ(KeyValuePair("SDD", "Solid Drive"), existenceByNode.first -> keyValuePair());
}
*/

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_ReturnsTheUpdatePositionOfAMatchingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  put(sentinelInternal, "HDD", "Hard disk drive");
  put(sentinelInternal, "SDD", "Solid state drive");
  put(sentinelInternal, "Pmem", "Persistent Memory");

  std::string key = "SDD";
  UpdatePosition updatePosition = sentinelInternal -> updatePositionOf(key);

  ASSERT_EQ("SDD", updatePosition.internal -> keyValuePair().getKey());
  ASSERT_EQ("Solid state drive", updatePosition.internal -> keyValuePair().getValue());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfANonMatchingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  put(sentinelInternal, "HDD", "Hard disk drive");
  put(sentinelInternal, "SDD", "Solid state drive");
  put(sentinelInternal, "Pmem", "Persistent Memory");

  std::string key = "NonMatchingKey";
  deleteBy(sentinelInternal, key);

  ASSERT_TRUE(sentinelInternal -> getBy("HDD").second);
  ASSERT_TRUE(sentinelInternal -> getBy("SDD").second);
  ASSERT_TRUE(sentinelInternal -> getBy("Pmem").second);
  ASSERT_FALSE(sentinelInternal -> getBy(key).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInBetweenInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  put(sentinelInternal, "HDD", "Hard disk drive");
  put(sentinelInternal, "SDD", "Solid state drive");
  put(sentinelInternal, "Pmem", "Persistent Memory");

  std::string key = "Pmem";
  deleteBy(sentinelInternal, key);

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInEndInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  put(sentinelInternal, "HDD", "Hard disk drive");
  put(sentinelInternal, "SDD", "Solid state drive");
  put(sentinelInternal, "Pmem", "Persistent Memory");

  std::string key = "SDD";
  deleteBy(sentinelInternal, key);

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInBeginningInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  put(sentinelInternal, "HDD", "Hard disk drive");
  put(sentinelInternal, "SDD", "Solid state drive");
  put(sentinelInternal, "Pmem", "Persistent Memory");

  std::string key = "HDD";
  deleteBy(sentinelInternal, key);

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key);

  ASSERT_FALSE(existenceByNode.second);
}
