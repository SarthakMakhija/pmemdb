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
  SkipListNodes parents;  
  SkipListNode* targetNode = node -> iterate(key,
                   [&, key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                            parents.add(node);
                            return make_pair(node, false);
                    });
  
  KeyValuePair pair = parents.pop() -> getKeyValuePair();
  ASSERT_EQ(KeyValuePair("Pmem", "Persistent Storage"), pair);
}

TEST(SkipListLeafNode, IterateAndReturnNodeAfterWhichKeyValueWouldBePut2) {
  SkipListLeafNode *node = new SkipListLeafNode("HDD", "Hard disk drive");
  SkipListLeafNode *right = new SkipListLeafNode("Pmem", "Persistent Storage");
  
  node  -> updateRight(right);
  right -> updateRight(new SkipListLeafNode("SDD", "Solid state drive"));
  
  string key = "Tuff";
  SkipListNodes parents;  
  SkipListNode* targetNode = node -> iterate(key,
                   [&, key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                            parents.add(node);
                            return make_pair(node, false);
                    });
  
  KeyValuePair pair = parents.pop() -> getKeyValuePair();
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), pair);
}