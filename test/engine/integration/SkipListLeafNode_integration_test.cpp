#include <gtest/gtest.h>
#include <string>
#include "../../../src/engine/storage/SkipListLeafNode.h"
#include "../../../src/engine/db/KeyValuePair.h"
#include "PersistentMemoryPoolFixture.h"
#include "../SkipListNodeTestUtils.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

std::pair<SkipListLeafNode *, Status> put(SkipListLeafNode* node, const char* key, const char* value, PersistentMemoryPool* pool) {    
    return node->put(
                      Slice(key, strlen(key) + 1), 
                      Slice(value, strlen(value) + 1), 
                      stringKeyComparator(), 
                      pool);
}

void update(SkipListLeafNode* node, const char* key, const char* value, PersistentMemoryPool* pool) {
    node->update(
                  Slice(key, strlen(key) + 1), 
                  Slice(value, strlen(value) + 1), 
                  stringKeyComparator(), 
                  pool);
}

void deleteBy(SkipListLeafNode* node, const char* key, PersistentMemoryPool* pool) {
    node->deleteBy(Slice(key, strlen(key) + 1), stringKeyComparator(), pool);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_RightKeyValuePairGivenDownPointerIsNotNull) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());

  SkipListLeafNode *node = put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool()).first;
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  ASSERT_EQ(KeyValuePair(Slice("SDD"), Slice("Solid state drive")), node -> rightKeyValuePair());
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_RightKeyValuePairGivenRightPointerIsNull) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  SkipListLeafNode *node = put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool()).first;

  ASSERT_EQ(KeyValuePair(Slice(""), Slice("")), node -> rightKeyValuePair());
}


TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_MatchKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());

  SkipListLeafNode *node = put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool()).first;
  bool matches = node -> matchesKey(Slice("HDD"), stringKeyComparator());

  ASSERT_TRUE(matches);
}


TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DoesNotMatchKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());

  SkipListLeafNode *node = put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool()).first;
  bool matches = node -> matchesKey(Slice("HDD"), stringKeyComparator());

  ASSERT_FALSE(matches);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());

  SkipListLeafNode *node = put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool()).first;
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo(Slice("SDD"), stringKeyComparator());

  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());

  SkipListLeafNode *node = put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool()).first;
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo(Slice("HDD"), stringKeyComparator());

  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());

  SkipListLeafNode *node = put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool()).first;
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo(Slice("HDD"), stringKeyComparator());

  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_PutAKeyValuePair1) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice key = Slice("SDD");
  std::pair<Slice, bool> valueByExistence = sentinel -> getBy(key, stringKeyComparator());

  ASSERT_EQ(Slice("Solid state drive"), valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_PutAKeyValuePair2) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "Pmem", "Persistent Storage", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice key = Slice("Pmem");
  std::pair<Slice, bool> valueByExistence = sentinel -> getBy(key, stringKeyComparator());

  ASSERT_EQ(Slice("Persistent Storage"), valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_GetByKeyForAnExistingKey) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "Pmem", "Persistent Storage", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice key = Slice("Pmem");
  std::pair<Slice, bool> valueByExistence = sentinel -> getBy(key, stringKeyComparator());

  ASSERT_EQ(Slice("Persistent Storage"), valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_GetByKeyForANonExistingKey) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "Pmem", "Persistent Storage", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice key = Slice("SDD");
  std::pair<Slice, bool> valueByExistence = sentinel -> getBy(key, stringKeyComparator());

  ASSERT_EQ(Slice(""), valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_ScanWithBeginKeyPresent) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "Pmem", "Persistent Storage", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "RAM", "Random Access Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice beginKey = Slice("Pmem");
  Slice endKey = Slice("SDD");

  std::vector<KeyValuePair> pairs = sentinel -> scan(beginKey, endKey, 10, stringKeyComparator());
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent Storage"), KeyValuePair("RAM", "Random Access Memory")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_ScanWithBeginKeyNotPresent) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "RAM", "Random Access Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice beginKey = Slice("Pmem");
  Slice endKey = Slice("SDD");
  
  std::vector<KeyValuePair> pairs = sentinel -> scan(beginKey, endKey, 10, stringKeyComparator());
  std::vector<KeyValuePair> expected = {KeyValuePair("RAM", "Random Access Memory")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_ScanWithMaxPairsAs1) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "Pmem", "Persistent Storage", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "RAM", "Random Access Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice beginKey = Slice("Pmem");
  Slice endKey = Slice("SDD");

  std::vector<KeyValuePair> pairs = sentinel -> scan(beginKey, endKey, 1, stringKeyComparator());
  std::vector<KeyValuePair> expected = {KeyValuePair("Pmem", "Persistent Storage")};

  ASSERT_EQ(expected, pairs);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_UpdateValueOfAMatchingKey) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "Pmem", "Persistent Storage", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  update(sentinel, "Pmem", "Persistent Memory", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice key = Slice("Pmem");
  std::pair<Slice, bool> valueByExistence = sentinel -> getBy(key, stringKeyComparator());

  ASSERT_EQ(Slice("Persistent Memory"), valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DeleteValueOfAMatchingKeyInTheBeginning) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "Pmem", "Persistent Storage", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid State drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  deleteBy(sentinel, "HDD", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice key = Slice("HDD");
  std::pair<Slice, bool> valueByExistence = sentinel -> getBy(key, stringKeyComparator());

  ASSERT_EQ(Slice(""), valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DeleteValueOfAMatchingKeyInBetween) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "Pmem", "Persistent Storage", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid State drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  deleteBy(sentinel, "Pmem", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice key = Slice("Pmem");
  std::pair<Slice, bool> valueByExistence = sentinel -> getBy(key, stringKeyComparator());

  ASSERT_EQ(Slice(""), valueByExistence.first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_DeleteValueOfAMatchingKeyInTheEnd) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "Pmem", "Persistent Storage", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid State drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  deleteBy(sentinel, "SDD", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Slice key = Slice("SDD");
  std::pair<Slice, bool> valueByExistence = sentinel -> getBy(key, stringKeyComparator());

  ASSERT_EQ(Slice(""), valueByExistence.first);
}