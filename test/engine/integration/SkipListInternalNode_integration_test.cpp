#include <gtest/gtest.h>
#include <string>
#include "../../../src/engine/storage/SkipListInternalNode.h"
#include "../../../src/engine/storage/SkipListLeafNode.h"
#include "../../../src/engine/db/KeyValuePair.h"
#include "../SkipListNodeTestUtils.h"
#include  "PersistentMemoryPoolFixture.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

void put(SkipListInternalNode* node, const char* key, const char* value, PersistentMemoryPool* pool) {
  PutPosition putPosition = node -> putPositionOf(key, stringKeyComparator(), new LevelGenerator(6));
  if (putPosition.newLevel != -1) {
      auto keySlice = Slice(key, strlen(key) + 1);
      std::pair < SkipListLeafNode *, Status > statusNodePair = static_cast<SkipListLeafNode *>(putPosition.leaf)->put(
              keySlice,
              Slice(value, strlen(value) + 1),
              stringKeyComparator(),
              pool);

      SkipListLeafNode *newLeaf = statusNodePair.first;
      SkipListNode *newInternal =  node -> put(keySlice, putPosition.positions, putPosition.newLevel);
      static_cast<SkipListInternalNode *>(newInternal) -> attach(newLeaf);
  }
}

void deleteBy(SkipListInternalNode* node, const char* key) {
  DeletePosition deletePosition = node -> deletePositionOf(key, stringKeyComparator());
  if (deletePosition.internal != nullptr) {
    auto keySlice = Slice(key, strlen(key) + 1);
    static_cast<SkipListInternalNode*>(deletePosition.internal) -> deleteBy(keySlice, deletePosition.positions, deletePosition.deleteLevel, stringKeyComparator());
  }
}

TEST(SkipListInternalNode, UpdateDownPointer) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", 6);
  SkipListLeafNode *down     = new SkipListLeafNode();
  node -> attach(down);

  ASSERT_TRUE(node -> getDown() != nullptr);
}

TEST(SkipListInternalNode, MatchKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", 6);
  bool matches = node -> matchesKey(Slice("HDD"), stringKeyComparator());

  ASSERT_TRUE(matches);
}

TEST(SkipListInternalNode, DoesNotMatchKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("SDD", 6);
  bool matches = node -> matchesKey(Slice("HDD"), stringKeyComparator());

  ASSERT_FALSE(matches);
}

TEST(SkipListInternalNode, NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", 6);
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo(Slice("SDD"), stringKeyComparator());

  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", 6);
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo(Slice("HDD"), stringKeyComparator());

  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("SDD", 6);
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo(Slice("HDD"), stringKeyComparator());

  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForAnExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  Slice hdd = Slice("HDD");
  Slice sdd = Slice("SDD");

  put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, sdd.cdata(), "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(sdd, stringKeyComparator());

  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), static_cast<SkipListInternalNode*>(existenceByNode.first)->getDown() -> keyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_NextInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  Slice hdd = Slice("HDD");
  Slice sdd = Slice("SDD");

  put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, sdd.cdata(), "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  SkipListNode* node = sentinelInternal->next();

  ASSERT_EQ(Slice("HDD"), node->keyValuePair().getKey());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_GetByKeyForANonExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  Slice hdd = Slice("HDD");
  Slice sdd = Slice("SDD");

  put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, sdd.cdata(), "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice key = Slice("Pmem");
  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(key, stringKeyComparator());

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_AttemptsToPutSameKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  Slice hdd = Slice("HDD");
  Slice sdd = Slice("SDD");

  put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, sdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  auto leaf = static_cast<SkipListInternalNode*>(sentinelInternal -> getBy(hdd, stringKeyComparator()).first)->getDown();
  Slice actualValue = leaf -> keyValuePair().getValue();
  ASSERT_EQ(Slice("Hard disk drive"), actualValue);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_ReturnsTheStartingLeafNodeToPerformScan) {
    SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    Slice hdd = Slice("HDD");
    Slice sdd = Slice("SDD");
    Slice pmem = Slice("Pmem");

    put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
    put(sentinelInternal, sdd.cdata(), "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
    put(sentinelInternal, pmem.cdata(), "Persistent Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());

    Slice beginKey = Slice("Pmem");
    std::pair<SkipListNode*, bool> nodeByExistence = sentinelInternal -> scan(beginKey, stringKeyComparator());

    ASSERT_EQ(Slice("Persistent Memory"), nodeByExistence.first -> keyValuePair().getValue());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_ReturnsFalseToPerformScanGivenBeginKeyIsOutsideTheBounds) {
    SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
    Slice hdd = Slice("HDD");
    Slice sdd = Slice("SDD");
    Slice pmem = Slice("Pmem");

    put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
    put(sentinelInternal, sdd.cdata(), "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
    put(sentinelInternal, pmem.cdata(), "Persistent Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());

    Slice beginKey = Slice("Tuff");
    std::pair<SkipListNode*, bool> nodeByExistence = sentinelInternal -> scan(beginKey, stringKeyComparator());

    ASSERT_FALSE(nodeByExistence.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_ReturnsTheUpdatePositionOfAMatchingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  Slice hdd = Slice("HDD");
  Slice sdd = Slice("SDD");
  Slice pmem = Slice("Pmem");

  put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, sdd.cdata(), "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, pmem.cdata(), "Persistent Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  UpdatePosition updatePosition = sentinelInternal -> updatePositionOf(sdd, stringKeyComparator());

  ASSERT_EQ(Slice("SDD"),               updatePosition.internal -> keyValuePair().getKey());
  ASSERT_EQ(Slice("Solid state drive"), updatePosition.leaf -> keyValuePair().getValue());
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfANonMatchingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  Slice hdd = Slice("HDD");
  Slice sdd = Slice("SDD");
  Slice pmem = Slice("Pmem");

  put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, sdd.cdata(), "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, pmem.cdata(), "Persistent Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice key = Slice("NonMatchingKey");
  deleteBy(sentinelInternal, key.cdata());

  ASSERT_TRUE(sentinelInternal -> getBy(hdd, stringKeyComparator()).second);
  ASSERT_TRUE(sentinelInternal -> getBy(sdd, stringKeyComparator()).second);
  ASSERT_TRUE(sentinelInternal -> getBy(pmem, stringKeyComparator()).second);
  ASSERT_FALSE(sentinelInternal -> getBy(key, stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInBetweenInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  Slice hdd = Slice("HDD");
  Slice sdd = Slice("SDD");
  Slice pmem = Slice("Pmem");

  put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, sdd.cdata(), "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, pmem.cdata(), "Persistent Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  deleteBy(sentinelInternal, pmem.cdata());

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(pmem, stringKeyComparator());

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInEndInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  Slice hdd = Slice("HDD");
  Slice sdd = Slice("SDD");
  Slice pmem = Slice("Pmem");

  put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, sdd.cdata(), "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, pmem.cdata(), "Persistent Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  deleteBy(sentinelInternal, sdd.cdata());

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(sdd, stringKeyComparator());

  ASSERT_FALSE(existenceByNode.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListInternalNode_DeleteValueOfAMatchingKeyInBeginningInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode(6, PersistentMemoryPoolFixture::getPersistentMemoryPool());
  Slice hdd = Slice("HDD");
  Slice sdd = Slice("SDD");
  Slice pmem = Slice("Pmem");

  put(sentinelInternal, hdd.cdata(), "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, sdd.cdata(), "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinelInternal, pmem.cdata(), "Persistent Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  deleteBy(sentinelInternal, hdd.cdata());

  std::pair<SkipListNode*, bool> existenceByNode = sentinelInternal -> getBy(hdd, stringKeyComparator());

  ASSERT_FALSE(existenceByNode.second);
}
