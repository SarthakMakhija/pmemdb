#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/SkipListNodes.h"
#include "../../src/engine/KeyValuePair.h"

TEST(SkipListLeafNode, AddNewNodeToRight) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  node -> addToRightWith("SDD", "Solid state drive");

  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListLeafNode, UpdateRightPointer) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("SDD", "Solid state drive");
  node -> updateRight(right);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListLeafNode, RightKeyValuePairGivenDownPointerIsNotNull) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("SDD", "Solid state drive");
  node -> updateRight(right);
  
  KeyValuePair pair = node -> rightKeyValuePair();
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListLeafNode, RightKeyValuePairGivenRightPointerIsNull) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  
  KeyValuePair pair = node -> rightKeyValuePair();
  
  ASSERT_EQ(KeyValuePair("", ""), node -> rightKeyValuePair());
}

TEST(SkipListLeafNode, MatchKeyInSkipListNode) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_TRUE(matches);
}

TEST(SkipListLeafNode, DoesNotMatchKeyInSkipListNode) {
  SkipListLeafNode *node = new SkipListLeafNode("SDD", "Solid state drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_FALSE(matches);
}

TEST(SkipListLeafNode, NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("SDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListLeafNode, NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListLeafNode, NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListLeafNode *node = new SkipListLeafNode("SDD", "Solid state drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST(SkipListLeafNode, UpdateNodeValue) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  node -> updateValue("Hard disk");
  
  ASSERT_EQ(KeyValuePair("HDD", "Hard disk"), node -> getKeyValuePair());
}

TEST(SkipListLeafNode, GetByKeyForAnExistingKey) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "SDD";
  pair<SkipListNode*, bool> existenceByNode = node -> getBy(key);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), existenceByNode.first -> getKeyValuePair());
}

TEST(SkipListLeafNode, GetByKeyForANonExistingKey) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Storage";
  pair<SkipListNode*, bool> existenceByNode = node -> getBy(key);
  
  ASSERT_EQ(nullptr, existenceByNode.first);
  ASSERT_FALSE(existenceByNode.second);
}

TEST(SkipListLeafNode, InsertPositionAfterWhichKeyValueWouldBePut1) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Rocks";
  SkipListNode* targetNode = node -> insertPosition(key);
  
  ASSERT_EQ(KeyValuePair("Pmem", "Persistent Storage"), targetNode -> getKeyValuePair());
}

TEST(SkipListLeafNode, InsertPositionAfterWhichKeyValueWouldBePut2) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Tuff";
  SkipListNode* targetNode = node -> insertPosition(key);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), targetNode -> getKeyValuePair());
}

TEST(SkipListLeafNode, UpdateValueOfAMatchingKey) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Pmem";
  node -> update(key, "Persistent Memory");

  KeyValuePair pair = node -> getBy(key).first -> getKeyValuePair();
  ASSERT_EQ(KeyValuePair("Pmem", "Persistent Memory"), pair);
}