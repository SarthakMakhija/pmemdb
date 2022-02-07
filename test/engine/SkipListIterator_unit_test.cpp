#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "../../src/engine/SkipListIterator.h"
#include "../../src/engine/SkipListNodes.h"
#include "./PersistentMemoryPoolFixture.h"
#include "./SkipListNodeTestUtils.h"

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_PutInASkipListLeafNode) {
    SkipListNode* sentinel    = newSentinelLeafNode();
    SkipListIterator iterator = SkipListIterator(sentinel);

    iterator.put("HDD", "Hard disk drive");

    std::pair<std::string, bool> valueByExistence = iterator.getBy("HDD");
    ASSERT_EQ("Hard disk drive", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_PutInASkipListInHierarchy) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  iterator.put("Pmem", "Persistent Memory");

  std::pair<std::string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("Persistent Memory", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForAnExistingKeyInInternalNode) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  iterator.put("Pmem", "Persistent Memory");

  std::pair<std::string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("Persistent Memory", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForAnExistingKeyInLeafNode) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);

  std::pair<std::string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("Persistent Memory", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_GetByKeyForANonExistingKey) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);

  std::pair<std::string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_MultiGet) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  std::vector<std::string> keys = {"HDD", "SDD", "Pmem", "DoesNotExist"};
  std::vector<std::pair<std::string, bool>> result = iterator.multiGet(keys);
  
  std::vector<std::pair<std::string, bool>> expected = {
                            std::make_pair("Hard disk drive", true), 
                            std::make_pair("Solid state drive", true),
                            std::make_pair("Persistent Memory", true),
                            std::make_pair("", false)
  };

  ASSERT_EQ(expected, result);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyPresent) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  std::string beginKey = "Pmem";
  std::string endKey = "SDD";

  std::vector<KeyValuePair> pairs = iterator.scan(beginKey, endKey);
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent Memory")};
  
  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_ScanWithBeginKeyNotPresent) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  std::string beginKey = "RAM";
  std::string endKey = "Tuff";

  std::vector<KeyValuePair> pairs = iterator.scan(beginKey, endKey);
  std::vector<KeyValuePair> expected = {KeyValuePair("SDD", "Solid state drive")};
  
  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_UpdateTheValueOfAMatchingKey) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  iterator.update("HDD", "Hard drive");

  std::pair<std::string, bool> valueByExistence = iterator.getBy("HDD");
  ASSERT_EQ("Hard drive", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_UpdateTheValueOfAMatchingKeyInLeafNode) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  iterator.update("Pmem", "Persistent Storage");

  std::pair<std::string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("Persistent Storage", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInBetween) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  iterator.deleteBy("Pmem");

  std::pair<std::string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("", valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListIterator_DeleteValueOfAMatchingKeyInEnd) {
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

  SkipListIterator iterator = SkipListIterator(sentinelInternal);
  iterator.deleteBy("SDD");

  std::pair<std::string, bool> valueByExistence = iterator.getBy("SDD");
  ASSERT_EQ("", valueByExistence.first);
}