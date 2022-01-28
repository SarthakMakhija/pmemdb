#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListLeafNode.h"
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

TEST(SkipListLeafNode, IterateAndReturnNodeWithMatchingKeyValue) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "SDD";
  SkipListNode* targetNode = node -> iterate(key,
                  [key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                        if (node -> matchesKey(key)) {
                          return make_pair(node, true);
                        } else {
                          return make_pair(nullptr, false);
                        }  
                    });
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), targetNode -> getKeyValuePair());
}

TEST(SkipListLeafNode, IterateAndReturnNodeWithoutMatchingKeyValue) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Storage";
  SkipListNode* targetNode = node -> iterate(key,
                  [key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                        if (node -> matchesKey(key)) {
                          return make_pair(node, true);
                        } else {
                          return make_pair(nullptr, false);
                        }  
                    });
  
  ASSERT_EQ(nullptr, targetNode);
}

TEST(SkipListLeafNode, IterateAndReturnNodeAfterWhichKeyValueWouldBePut1) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Rocks";
  SkipListNode* targetNode = node -> iterate(key,
                  [key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                        if (node -> matchesKey(key)) {
                          return make_pair(node, true);
                        } else {
                          return make_pair(nullptr, false);
                        }  
                    });
  
  ASSERT_EQ(KeyValuePair("Pmem", "Persistent Storage"), targetNode -> getKeyValuePair());
}

TEST(SkipListLeafNode, IterateAndReturnNodeAfterWhichKeyValueWouldBePut2) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Tuff";
  SkipListNode* targetNode = node -> iterate(key,
                  [key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                        if (node -> matchesKey(key)) {
                          return make_pair(node, true);
                        } else {
                          return make_pair(nullptr, false);
                        }  
                    });
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), targetNode -> getKeyValuePair());
}