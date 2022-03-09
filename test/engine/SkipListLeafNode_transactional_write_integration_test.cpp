#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListLeafNode.h"
#include "./PersistentMemoryPoolFixture.h"
#include "./SkipListNodeTestUtils.h"

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhilePuttingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  std::pair<SkipListLeafNode*, Status> statusNodePair = sentinel -> put("HDD", "Hard disk drive", [] {throw std::runtime_error("FailsWhilePuttingAKeyValue");});
  ASSERT_FALSE(sentinel -> getBy("HDD").second);
  ASSERT_EQ(Status::Failed, statusNodePair.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhilePuttingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");

  std::pair<SkipListLeafNode*, Status> statusNodePair = sentinel -> put("HDD", "Hard disk drive", [] {throw std::runtime_error("FailsWhilePuttingAKeyValueAndSucceedsWithOther");});
  ASSERT_FALSE(sentinel -> getBy("HDD").second);
  ASSERT_TRUE(sentinel -> getBy("SDD").second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileUpdatingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");

  Status status = sentinel -> update("SDD", "Solid state", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});
  ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD").first);
  ASSERT_EQ(Status::Failed, status);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileUpdatingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");
  sentinel -> put("HDD", "Hard disk drive");

  sentinel -> update("HDD", "HDD");  
  sentinel -> update("SDD", "Solid state", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValueAndSucceedsWithOther");});
  ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD").first);
  ASSERT_EQ("HDD", sentinel -> getBy("HDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");

  Status status = sentinel -> deleteBy("SDD", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});
  ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD").first);
  ASSERT_EQ(Status::Failed, status);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive");
  sentinel -> put("HDD", "Hard disk drive");

  sentinel -> deleteBy("SDD", [] {throw std::runtime_error("FailsWhileDeletingAKeyValueAndSucceedsWithOther");});
  sentinel -> deleteBy("HDD");
  ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD").first);
  ASSERT_EQ("", sentinel -> getBy("HDD").first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyRange) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("A", "A");
  sentinel -> put("B", "B");
  sentinel -> put("C", "C");
  sentinel -> put("D", "D");

  Status status = sentinel -> deleteRange("A", "C", [] {throw std::runtime_error("FailsWhileDeletingAKeyRange");});
  ASSERT_EQ("A", sentinel -> getBy("A").first);
  ASSERT_EQ("B", sentinel -> getBy("B").first);
  ASSERT_EQ("C", sentinel -> getBy("C").first);
  ASSERT_EQ("D", sentinel -> getBy("D").first);
  ASSERT_EQ(Status::Failed, status);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingOneOfTheKeysInAKeyRange) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("A", "A");
  sentinel -> put("B", "B");
  sentinel -> put("C", "C");
  sentinel -> put("D", "D");

  int count = 0;
  Status status = sentinel -> deleteRange("A", "D", [&] {
    count = count + 1;
    if (count >= 2) {
      throw std::runtime_error("FailsWhileDeletingAKeyRange");
    }
  });
  ASSERT_EQ("A", sentinel -> getBy("A").first);
  ASSERT_EQ("B", sentinel -> getBy("B").first);
  ASSERT_EQ("C", sentinel -> getBy("C").first);
  ASSERT_EQ("D", sentinel -> getBy("D").first);
  ASSERT_EQ(Status::Failed, status);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyRangeAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("A", "A");
  sentinel -> put("B", "B");
  sentinel -> put("C", "C");
  sentinel -> put("D", "D");

  sentinel -> deleteRange("A", "C");
  sentinel -> deleteRange("D", "E", [] {throw std::runtime_error("FailsWhileDeletingAKeyRangeAndSucceedsWithOther");});
  ASSERT_EQ("", sentinel -> getBy("A").first);
  ASSERT_EQ("", sentinel -> getBy("B").first);
  ASSERT_EQ("C", sentinel -> getBy("C").first);
  ASSERT_EQ("D", sentinel -> getBy("D").first);
}