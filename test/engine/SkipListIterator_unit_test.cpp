#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "../../src/engine/SkipListIterator.h"
#include "../../src/engine/SkipListNodes.h"

TEST(SkipListNodeIterator, PutInASkipListLeafNode) {
    SkipListLeafNode *leafFirst = new SkipListLeafNode("Pmem", "Persistent Memory");
    SkipListIterator iterator = SkipListIterator(leafFirst);

    iterator.put("HDD", "Hard disk drive");

    pair<string, bool> valueByExistence = iterator.getBy("HDD");
    ASSERT_EQ("Hard disk drive", valueByExistence.first);
}

TEST(SkipListNodeIterator, PutInASkipListInHierarchy) {
  SkipListInternalNode *internalFirst = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *internalSecond = new SkipListInternalNode("SDD", "Solid state drive");
  
  internalFirst -> updateRight(internalSecond);

  SkipListLeafNode *leafFirst = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *leafSecond = new SkipListLeafNode("SDD", "Solid state drive");
  
  leafFirst  -> updateRight(leafSecond);

  internalFirst  -> updateDown(leafFirst);
  internalSecond -> updateDown(leafSecond);

  SkipListIterator iterator = SkipListIterator(internalFirst);
  iterator.put("Pmem", "Persistent Memory");

  pair<string, bool> valueByExistence = iterator.getBy("Pmem");
  ASSERT_EQ("Persistent Memory", valueByExistence.first);
}

TEST(SkipListNodeIterator, GetByKeyForAnExistingKeyInInternalNode) {
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
  pair<string, bool> valueByExistence = SkipListIterator(internalFirst).getBy(key);
  
  ASSERT_EQ("Solid state drive", valueByExistence.first);
}

TEST(SkipListNodeIterator, GetByKeyForAnExistingKeyInLeafNode) {
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
  pair<string, bool> valueByExistence = SkipListIterator(internalFirst).getBy(key);
  
  ASSERT_EQ("Persistent Storage", valueByExistence.first);
}

TEST(SkipListNodeIterator, GetByKeyForANonExistingKey) {
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
  
  string key = "Tuff";
  pair<string, bool> valueByExistence = SkipListIterator(internalFirst).getBy(key);
  
  ASSERT_EQ("", valueByExistence.first);
}

TEST(SkipListNodeIterator, UpdateValueOfAMatchingKeyInInternalNode) {
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
  SkipListIterator(internalFirst).update(key, "Solid Drive");
  
  pair<SkipListNode*, bool> existenceByNode = internalFirst -> getBy(key);
  ASSERT_EQ(KeyValuePair("SDD", "Solid Drive"), existenceByNode.first -> keyValuePair());
}

TEST(SkipListNodeIterator, UpdateValueOfAMatchingKeyInLeafNode) {
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
  SkipListIterator(internalFirst).update(key, "Solid Drive");
  
  pair<string, bool> valueByExistence = leafFirst -> getBy(key);
  ASSERT_EQ("Solid Drive", valueByExistence.first);
}

TEST(SkipListNodeIterator, DeleteValueOfAMatchingKeyInBetween) {
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

TEST(SkipListNodeIterator, DeleteValueOfAMatchingKeyInEnd) {
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
