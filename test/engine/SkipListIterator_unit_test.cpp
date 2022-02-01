#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "../../src/engine/SkipListIterator.h"
#include "../../src/engine/SkipListNodes.h"
#include "./PersistentMemoryPoolFixture.h"
#include "./SkipListNodeTestUtils.h"

TEST_F(PersistentMemoryPoolFixture, PutInASkipListLeafNode) {
    SkipListNode* sentinel    = newSentinelLeafNode();
    SkipListIterator iterator = SkipListIterator(sentinel);

    iterator.put("HDD", "Hard disk drive");

    pair<string, bool> valueByExistence = iterator.getBy("HDD");
    ASSERT_EQ("Hard disk drive", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, PutInASkipListInHierarchy) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafSecond);

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  iterator.put("Pmem", "Persistent Memory");

  pair<string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("Persistent Memory", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, GetByKeyForAnExistingKeyInInternalNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafSecond);

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  iterator.put("Pmem", "Persistent Memory");

  pair<string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("Persistent Memory", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, GetByKeyForAnExistingKeyInLeafNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("Pmem", "Persistent Memory");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);

  SkipListIterator iterator = SkipListIterator(sentinelInternal);

  pair<string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("Persistent Memory", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, GetByKeyForANonExistingKeyViaIterator) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafSecond);

  SkipListIterator iterator = SkipListIterator(sentinelInternal);

  pair<string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, UpdateTheValueOfAMatchingKeyViaIterator) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafSecond);

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  iterator.update("HDD", "Hard drive");

  pair<string, bool> valueByExistence = iterator.getBy("HDD");
  ASSERT_EQ("Hard drive", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, UpdateTheValueOfAMatchingKeyInLeafNode) {
  SkipListInternalNode* sentinelInternal = newSentinelInternalNode();
  SkipListLeafNode* sentinelLeaf         = newSentinelLeafNode();

  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = sentinelLeaf -> put("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = sentinelLeaf -> put("Pmem", "Persistent Memory");
  SkipListLeafNode *leafThird = sentinelLeaf -> put("SDD", "Solid state drive");
  
  sentinelInternal -> updateDown(sentinelLeaf);
  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  iterator.update("Pmem", "Persistent Storage");

  pair<string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("Persistent Storage", valueByExistence.first);
}

/*
TEST_F(PersistentMemoryPoolFixture, DeleteValueOfAMatchingKeyInBetween) {
  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = new SkipListLeafNode("Pmem", "Persistent Storage");
  SkipListLeafNode *leafThird = new SkipListLeafNode("SDD", "Solid state drive");
  
  leafFirst  -> updateRight(leafSecond);
  leafSecond -> updateRight(leafThird);

  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);
  
  string key = "Pmem";
  SkipListIterator iterator = SkipListIterator(internalFirst);
  iterator.deleteBy(key);
  
  pair<string, bool> valueByExistence = iterator.getBy(key);
  ASSERT_FALSE(valueByExistence.second);
}

TEST_F(PersistentMemoryPoolFixture, DeleteValueOfAMatchingKeyInEnd) {
  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = new SkipListLeafNode("Pmem", "Persistent Storage");
  SkipListLeafNode *leafThird = new SkipListLeafNode("SDD", "Solid state drive");
  
  leafFirst  -> updateRight(leafSecond);
  leafSecond -> updateRight(leafThird);

  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafThird);
  
  string key = "SDD";
  SkipListIterator iterator = SkipListIterator(internalFirst);
  iterator.deleteBy(key);
  
  pair<string, bool> valueByExistence = iterator.getBy(key);
  ASSERT_FALSE(valueByExistence.second);
}
*/