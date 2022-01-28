#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "SkipListNodeUtils.h"

TEST(SkipListInternalNode, AddNewNodeToRight) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  node -> addToRightWith("SDD", "Solid state drive");

  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListInternalNode, UpdateDownPointer) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *down = new SkipListInternalNode("SDD", "Solid state drive");
  node -> updateDown(down);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), downKeyValuePairOf(node));
}

TEST(SkipListInternalNode, UpdateRightPointer) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *right = new SkipListInternalNode("SDD", "Solid state drive");
  node -> updateRight(right);
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListInternalNode, RightKeyValuePairGivenDownPointerIsNotNull) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  SkipListInternalNode *right = new SkipListInternalNode("SDD", "Solid state drive");
  node -> updateRight(right);
  
  KeyValuePair pair = node -> rightKeyValuePair();
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), node -> rightKeyValuePair());
}

TEST(SkipListInternalNode, RightKeyValuePairGivenRightPointerIsNull) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  
  KeyValuePair pair = node -> rightKeyValuePair();
  
  ASSERT_EQ(KeyValuePair("", ""), node -> rightKeyValuePair());
}

TEST(SkipListInternalNode, MatchKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_TRUE(matches);
}

TEST(SkipListInternalNode, DoesNotMatchKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("SDD", "Solid state drive");
  bool matches = node -> matchesKey("HDD");
  
  ASSERT_FALSE(matches);
}

TEST(SkipListInternalNode, NodesKeyIsLessThanGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("SDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, NodesKeyIsEqualToGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_TRUE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, NodesKeyIsGreaterThanGivenKeyInSkipListNode) {
  SkipListInternalNode *node = new SkipListInternalNode("SDD", "Solid state drive");
  bool isKeyLessOrEqual = node -> isKeyLessEqualTo("HDD");
  
  ASSERT_FALSE(isKeyLessOrEqual);
}

TEST(SkipListInternalNode, UpdateNodeValue) {
  SkipListInternalNode *node = new SkipListInternalNode("HDD", "Hard disk drive");
  node -> updateValue("Hard disk");
  
  ASSERT_EQ(KeyValuePair("HDD", "Hard disk"), node -> getKeyValuePair());
}

TEST(SkipListInternalNode, IterateAndReturnNodeWithMatchingKeyValue) {
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
  SkipListNode* targetNode = internalFirst -> iterate(key,
                  [key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                        if (node -> matchesKey(key)) {
                          return make_pair(node, true);
                        } else {
                          return make_pair(nullptr, false);
                        }  
                    });
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), targetNode -> getKeyValuePair());
}

TEST(SkipListInternalNode, IterateAndReturnTheLastLeafNodeGivenKeyDesNotExist) {
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
  SkipListNode* targetNode = internalFirst -> iterate(key,
                  [key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                        if (node -> matchesKey(key)) {
                          return make_pair(node, true);
                        } else {
                          return make_pair(nullptr, false);
                        }  
                    });
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), targetNode -> getKeyValuePair());
  ASSERT_TRUE(targetNode -> isLeaf());
}

