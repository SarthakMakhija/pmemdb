#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListNode.h"
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"

TEST(SkipListNode, MatchKeyInSkipListNode) {
  SkipListNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_TRUE(matches);
}

TEST(SkipListNode, DoesNotMatchKeyInSkipListNode) {
  SkipListNode *node = new SkipListInternalNode("SDD", "Solid state drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_FALSE(matches);
}

TEST(SkipListNode, NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("SDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListNode, NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListNode, NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListNode *node = new SkipListInternalNode("SDD", "Solid state drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST(SkipListNode, UpdateNodeValue) {
  SkipListNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  node -> updateValue("Hard disk");
  
  ASSERT_EQ(KeyValuePair("HDD", "Hard disk"), node -> getKeyValuePair());
}
