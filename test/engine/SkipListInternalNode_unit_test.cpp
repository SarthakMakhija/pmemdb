#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/KeyValuePair.h"

TEST(SkipListInternalNode, AddNewNodeToRight) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  node -> addToRightWith("SDD", "Solid state drive");

  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListInternalNode, UpdateDownPointer) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *down = new SkipListInternalNode("SDD", "Solid state drive");
  node -> updateDown(down);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> downKeyValuePair());
}

TEST(SkipListInternalNode, UpdateRightPointer) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *right = new SkipListInternalNode("SDD", "Solid state drive");
  node -> updateRight(right);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListInternalNode, DownKeyValuePairGivenDownPointerIsNotNull) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *down = new SkipListInternalNode("SDD", "Solid state drive");
  node -> updateDown(down);
  
  KeyValuePair pair = node -> downKeyValuePair();
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> downKeyValuePair());
}

TEST(SkipListInternalNode, DownKeyValuePairGivenDownPointerIsNull) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  
  KeyValuePair pair = node -> downKeyValuePair();
  
  ASSERT_EQ(KeyValuePair("", ""), node -> downKeyValuePair());
}