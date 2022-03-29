#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "SkipListNodeTestUtils.h"
#include  "PersistentMemoryPoolFixture.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

void put(SkipListInternalNode* node, const char* key, const char* value, double probability = 0.5) {
  PutPosition putPosition = node -> putPositionOf(key, probability, stringKeyComparator());
  if (putPosition.newLevel != -1) {
      std::pair < SkipListLeafNode *, Status > statusNodePair = static_cast<SkipListLeafNode *>(putPosition.leaf)->put(key, value, stringKeyComparator());
      SkipListLeafNode *newLeaf = statusNodePair.first;
      SkipListNode *newInternal =  node -> put(key, putPosition.positions, putPosition.newLevel);
      static_cast<SkipListInternalNode *>(newInternal) -> attach(newLeaf);
  }
}

void deleteBy(SkipListInternalNode* node, const char* key) {
  DeletePosition deletePosition = node -> deletePositionOf(key, stringKeyComparator());
  if (deletePosition.internal != nullptr) {
    static_cast<SkipListInternalNode*>(deletePosition.internal) -> deleteBy(key, deletePosition.positions, deletePosition.deleteLevel, stringKeyComparator());
  }
}

TEST(SkipListInternalNode, UpdateDownPointer) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", 4);
  SkipListLeafNode *down     = new SkipListLeafNode();
  node -> attach(down);

  ASSERT_TRUE(node -> getDown() != nullptr);
}

TEST(SkipListInternalNode, MatchKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", 4);
  bool matches = node -> matchesKey("HDD", stringKeyComparator());

  ASSERT_TRUE(matches);
}

TEST(SkipListInternalNode, DoesNotMatchKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("SDD", 4);
  bool matches = node -> matchesKey("HDD", stringKeyComparator());

  ASSERT_FALSE(matches);
}

TEST(SkipListInternalNode, NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", 4);
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("SDD", stringKeyComparator());

  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", 4);
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD", stringKeyComparator());

  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("SDD", 4);
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD", stringKeyComparator());

  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForAnExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  std::string hdd = "HDD";
  std::string sdd = "SDD";

  put(sentinelInternal, hdd.c_str(), "Hard disk drive");
  put(sentinelInternal, sdd.c_str(), "Solid state drive");

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(sdd.c_str(), stringKeyComparator());

  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), existenceByNode.first -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForANonExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  std::string hdd = "HDD";
  std::string sdd = "SDD";

  put(sentinelInternal, hdd.c_str(), "Hard disk drive");
  put(sentinelInternal, sdd.c_str(), "Solid state drive");

  std::string key = "Pmem";
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key.c_str(), stringKeyComparator());

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_AttemptsToPutSameKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  std::string hdd = "HDD";
  std::string sdd = "SDD";

  put(sentinelInternal, hdd.c_str(), "Hard disk drive");
  put(sentinelInternal, sdd.c_str(), "Hard disk drive");

  const char* actualValue = sentinelInternal -> getBy(hdd.c_str(), stringKeyComparator()).first -> keyValuePair().getValue();
  ASSERT_EQ("Hard disk drive", std::string(actualValue));
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_ReturnsTheStartingLeafNodeToPerformScan) {
    SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
    std::string hdd = "HDD";
    std::string sdd = "SDD";
    std::string pmem = "Pmem";

    put(sentinelInternal, hdd.c_str(), "Hard disk drive");
    put(sentinelInternal, sdd.c_str(), "Solid state drive");
    put(sentinelInternal, pmem.c_str(), "Persistent Memory");

    std::string beginKey = "Pmem";
    std::pair<SkipListNode*, bool> nodeByExistence = sentinelInternal -> scan(beginKey.c_str(), stringKeyComparator());

    ASSERT_EQ("Persistent Memory", std::string(nodeByExistence.first -> keyValuePair().getValue()));
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_ReturnsFalseToPerformScanGivenBeginKeyIsOutsideTheBounds) {
    SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
    std::string hdd = "HDD";
    std::string sdd = "SDD";
    std::string pmem = "Pmem";

    put(sentinelInternal, hdd.c_str(), "Hard disk drive");
    put(sentinelInternal, sdd.c_str(), "Solid state drive");
    put(sentinelInternal, pmem.c_str(), "Persistent Memory");

    std::string beginKey = "Tuff";
    std::pair<SkipListNode*, bool> nodeByExistence = sentinelInternal -> scan(beginKey.c_str(), stringKeyComparator());

    ASSERT_FALSE(nodeByExistence.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_ReturnsTheUpdatePositionOfAMatchingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  std::string hdd = "HDD";
  std::string sdd = "SDD";
  std::string pmem = "Pmem";

  put(sentinelInternal, hdd.c_str(), "Hard disk drive");
  put(sentinelInternal, sdd.c_str(), "Solid state drive");
  put(sentinelInternal, pmem.c_str(), "Persistent Memory");

  UpdatePosition updatePosition = sentinelInternal -> updatePositionOf(sdd.c_str(), stringKeyComparator());

  ASSERT_EQ("SDD", std::string(updatePosition.internal -> keyValuePair().getKey()));
  ASSERT_EQ("Solid state drive", std::string(updatePosition.leaf -> keyValuePair().getValue()));
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfANonMatchingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  std::string hdd = "HDD";
  std::string sdd = "SDD";
  std::string pmem = "Pmem";

  put(sentinelInternal, hdd.c_str(), "Hard disk drive");
  put(sentinelInternal, sdd.c_str(), "Solid state drive");
  put(sentinelInternal, pmem.c_str(), "Persistent Memory");

  std::string key = "NonMatchingKey";
  deleteBy(sentinelInternal, key.c_str());

  ASSERT_TRUE(sentinelInternal -> getBy(hdd.c_str(), stringKeyComparator()).second);
  ASSERT_TRUE(sentinelInternal -> getBy(sdd.c_str(), stringKeyComparator()).second);
  ASSERT_TRUE(sentinelInternal -> getBy(pmem.c_str(), stringKeyComparator()).second);
  ASSERT_FALSE(sentinelInternal -> getBy(key.c_str(), stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInBetweenInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  std::string hdd = "HDD";
  std::string sdd = "SDD";
  std::string pmem = "Pmem";

  put(sentinelInternal, hdd.c_str(), "Hard disk drive");
  put(sentinelInternal, sdd.c_str(), "Solid state drive");
  put(sentinelInternal, pmem.c_str(), "Persistent Memory");

  deleteBy(sentinelInternal, pmem.c_str());

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(pmem.c_str(), stringKeyComparator());

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInEndInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  std::string hdd = "HDD";
  std::string sdd = "SDD";
  std::string pmem = "Pmem";

  put(sentinelInternal, hdd.c_str(), "Hard disk drive");
  put(sentinelInternal, sdd.c_str(), "Solid state drive");
  put(sentinelInternal, pmem.c_str(), "Persistent Memory");

  deleteBy(sentinelInternal, sdd.c_str());

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(sdd.c_str(), stringKeyComparator());

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInBeginningInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6);
  std::string hdd = "HDD";
  std::string sdd = "SDD";
  std::string pmem = "Pmem";

  put(sentinelInternal, hdd.c_str(), "Hard disk drive");
  put(sentinelInternal, sdd.c_str(), "Solid state drive");
  put(sentinelInternal, pmem.c_str(), "Persistent Memory");

  deleteBy(sentinelInternal, hdd.c_str());

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(hdd.c_str(), stringKeyComparator());

  ASSERT_FALSE(existenceByNode.second);
}
