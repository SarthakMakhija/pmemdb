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

TEST(SkipListLeafNode, PutAKeyValuePair1) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");  
  node  -> updateRight(right);
  
  node -> put("SDD", "Solid state drive");

  string key = "SDD";
  pair<string, bool> valueByExistence = node -> getBy(key);
  
  ASSERT_EQ("Solid state drive", valueByExistence.first);
}

TEST(SkipListLeafNode, PutAKeyValuePair2) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("SDD", "Solid state drive");  
  node  -> updateRight(right);
  
  node -> put("Pmem", "Persistent Storage");

  string key = "Pmem";
  pair<string, bool> valueByExistence = node -> getBy(key);
  
  ASSERT_EQ("Persistent Storage", valueByExistence.first);
}

TEST(SkipListLeafNode, GetByKeyForAnExistingKey) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "SDD";
  pair<string, bool> valueByExistence = node -> getBy(key);
  
  ASSERT_EQ("Solid state drive", valueByExistence.first);
}

TEST(SkipListLeafNode, GetByKeyForANonExistingKey) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Storage";
  pair<string, bool> valueByExistence = node -> getBy(key);
  
  ASSERT_EQ("", valueByExistence.first);
  ASSERT_FALSE(valueByExistence.second);
}

TEST(SkipListLeafNode, UpdateValueOfAMatchingKey) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Pmem";
  node -> update(key, "Persistent Memory");

  string value = node -> getBy(key).first;
  ASSERT_EQ("Persistent Memory", value);
}

TEST(SkipListLeafNode, DeleteValueOfAMatchingKeyInBetween) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Pmem";
  node -> deleteBy(key);

  ASSERT_FALSE(node -> getBy(key).second);
}

TEST(SkipListLeafNode, DeleteValueOfAMatchingKeyInEnd) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "SDD";
  node -> deleteBy(key);

  ASSERT_FALSE(node -> getBy(key).second);
}
