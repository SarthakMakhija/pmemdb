#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include <iostream>

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

