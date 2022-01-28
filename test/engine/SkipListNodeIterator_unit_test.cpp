#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "../../src/engine/SkipListNodeIterator.h"
#include "../../src/engine/SkipListNodes.h"

TEST(SkipListNodeIterator, IterateAndReturnNodeWithMatchingKeyValue1) {
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
  SkipListNode* targetNode = SkipListNodeIterator(internalFirst).iterate(key,
                  [key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                        if (node -> matchesKey(key)) {
                          return make_pair(node, true);
                        } else {
                          return make_pair(nullptr, false);
                        }  
                    });
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), targetNode -> getKeyValuePair());
}

TEST(SkipListNodeIterator, IterateAndReturnNodeWithMatchingKeyValue2) {
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
  SkipListNode* targetNode = SkipListNodeIterator(internalFirst).iterate(key,
                  [key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                        if (node -> matchesKey(key)) {
                          return make_pair(node, true);
                        } else {
                          return make_pair(nullptr, false);
                        }  
                    });
  
  ASSERT_EQ(KeyValuePair("Pmem", "Persistent Storage"), targetNode -> getKeyValuePair());
}

TEST(SkipListNodeIterator, IterateAndReturnNodeAfterWhichKeyValueWouldBePut) {
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
  
  string key = "Rocks";
  SkipListNodes parents;  
  SkipListNode* targetNode = SkipListNodeIterator(internalFirst).iterate(key,
                   [&, key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
                            parents.add(node);
                            return make_pair(node, false);
                    });
  
  ASSERT_EQ(KeyValuePair("Pmem", "Persistent Storage"), parents.pop() -> getKeyValuePair());
  ASSERT_EQ(KeyValuePair("HDD" ,"Hard disk drive"),     parents.pop() -> getKeyValuePair());
}

