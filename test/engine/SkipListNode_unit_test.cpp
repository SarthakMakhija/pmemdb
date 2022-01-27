#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListNode.h"
#include "../../src/engine/KeyValuePair.h"


TEST(SkipListNode, MatchKeyInSkipListNode) {
  SkipListNode *node = new SkipListNode("HDD", "Hard disk drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_TRUE(matches);
}

TEST(SkipListNode, DoesNotMatchKeyInSkipListNode) {
  SkipListNode *node = new SkipListNode("SDD", "Solid state drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_FALSE(matches);
}

TEST(SkipListNode, NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListNode *node = new SkipListNode("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("SDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListNode, NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListNode *node = new SkipListNode("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListNode, NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListNode *node = new SkipListNode("SDD", "Solid state drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST(SkipListNode, AddNewNodeToRight) {
  SkipListNode *node = new SkipListNode("HDD", "Hard disk drive");
  node -> addToRightWith("SDD", "Solid state drive");

  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListNode, UpdateDownPointer) {
  SkipListNode *node = new SkipListNode("HDD", "Hard disk drive");
  SkipListNode *down = new SkipListNode("SDD", "Solid state drive");
  node -> updateDown(down);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> downKeyValuePair());
}

TEST(SkipListNode, UpdateRightPointer) {
  SkipListNode *node = new SkipListNode("HDD", "Hard disk drive");
  SkipListNode *right = new SkipListNode("SDD", "Solid state drive");
  node -> updateRight(right);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListNode, UpdateNodeValue) {
  SkipListNode *node = new SkipListNode("HDD", "Hard disk drive");
  node -> updateValue("Hard disk");
  
  ASSERT_EQ(KeyValuePair("HDD", "Hard disk"), node -> getKeyValuePair());
}