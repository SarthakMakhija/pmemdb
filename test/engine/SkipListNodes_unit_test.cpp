#include <gtest/gtest.h>
#include "../../src/engine/SkipListNodes.h"
#include "../../src/engine/SkipListInternalNode.h"

TEST(SkipListNodes, AddANodeToSkipListNodes) {
  SkipListNode *node = new SkipListInternalNode("HDD", "Hard disk drive", 4);
  
  SkipListNodes skipListNodes;
  skipListNodes.add(node);
  
  ASSERT_EQ(KeyValuePair("HDD", "Hard disk drive"), skipListNodes.pop() -> keyValuePair());
}

TEST(SkipListNodes, Add2NodesToSkipListNodes) {  
  SkipListNodes skipListNodes;
  skipListNodes.add(new SkipListInternalNode("HDD", "Hard disk drive", 4));
  skipListNodes.add(new SkipListInternalNode("SDD", "Solid state drive",4));
  
  ASSERT_EQ(KeyValuePair("SDD", "Solid state drive"), skipListNodes.pop() -> keyValuePair());
  ASSERT_EQ(KeyValuePair("HDD", "Hard disk drive"), skipListNodes.pop() -> keyValuePair());
}

TEST(SkipListNodes, IsNotEmpty) {  
  SkipListNodes skipListNodes;
  skipListNodes.add(new SkipListInternalNode("SDD", "Solid state drive", 4));
  
  ASSERT_FALSE(skipListNodes.isEmpty());
}

TEST(SkipListNodes, IsEmpty) {  
  SkipListNodes skipListNodes;
  
  ASSERT_TRUE(skipListNodes.isEmpty());
}
