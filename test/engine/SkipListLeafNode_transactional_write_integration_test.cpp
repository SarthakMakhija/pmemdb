#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "./PersistentMemoryPoolFixture.h"
#include "./SkipListNodeTestUtils.h"

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhilePuttingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  try {  
    SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive", [] {throw std::runtime_error("FailsWhilePuttingAKeyValue");});
  } catch(...) {
      ASSERT_FALSE(sentinel -> getBy("HDD").second);
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhilePuttingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");

  try {  
    SkipListLeafNode *node = sentinel -> put("HDD", "Hard disk drive", [] {throw std::runtime_error("FailsWhilePuttingAKeyValueAndSucceedsWithOther");});
  } catch(...) {
      ASSERT_FALSE(sentinel -> getBy("HDD").second);
      ASSERT_TRUE(sentinel -> getBy("SDD").second);
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileUpdatingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");

  try {  
    sentinel -> update("SDD", "Solid state", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});
  } catch(...) {
      ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD").first);
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileUpdatingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");
  sentinel -> put("HDD", "Hard disk drive");

  sentinel -> update("HDD", "HDD");  
  try {  
    sentinel -> update("SDD", "Solid state", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValueAndSucceedsWithOther");});
  } catch(...) {
      ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD").first);
      ASSERT_EQ("HDD", sentinel -> getBy("HDD").first);
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");

  try {  
    sentinel -> deleteBy("SDD", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});
  } catch(...) {
      ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD").first);
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");
  sentinel -> put("HDD", "Hard disk drive");

  try {  
    sentinel -> deleteBy("SDD", [] {throw std::runtime_error("FailsWhileDeletingAKeyValueAndSucceedsWithOther");});
    sentinel -> deleteBy("HDD");
  } catch(...) {
      ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD").first);
      ASSERT_EQ("", sentinel -> getBy("HDD").first);
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyRange) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("A", "A");
  sentinel -> put("B", "B");
  sentinel -> put("C", "C");
  sentinel -> put("D", "D");

  try {  
    sentinel -> deleteRange("A", "C", [] {throw std::runtime_error("FailsWhileDeletingAKeyRange");});
  } catch(...) {
      ASSERT_EQ("A", sentinel -> getBy("A").first);
      ASSERT_EQ("B", sentinel -> getBy("B").first);
      ASSERT_EQ("C", sentinel -> getBy("C").first);
      ASSERT_EQ("D", sentinel -> getBy("D").first);
  }
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyRangeAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("A", "A");
  sentinel -> put("B", "B");
  sentinel -> put("C", "C");
  sentinel -> put("D", "D");

  sentinel -> deleteRange("A", "C");

  try {  
    sentinel -> deleteRange("D", "E", [] {throw std::runtime_error("FailsWhileDeletingAKeyRangeAndSucceedsWithOther");});
  } catch(...) {
      ASSERT_EQ("", sentinel -> getBy("A").first);
      ASSERT_EQ("", sentinel -> getBy("B").first);
      ASSERT_EQ("C", sentinel -> getBy("C").first);
      ASSERT_EQ("D", sentinel -> getBy("D").first);
  }
}