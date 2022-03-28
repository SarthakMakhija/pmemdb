#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListLeafNode.h"
#include "./PersistentMemoryPoolFixture.h"
#include "./SkipListNodeTestUtils.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhilePuttingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  std::pair<SkipListLeafNode*, Status> statusNodePair = sentinel -> put("HDD", "Hard disk drive",
                                                                        stringKeyComparator(),
                                                                        [] {throw std::runtime_error("FailsWhilePuttingAKeyValue");});
  ASSERT_FALSE(sentinel -> getBy("HDD", stringKeyComparator()).second);
  ASSERT_EQ(Status::Failed, statusNodePair.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhilePuttingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive", stringKeyComparator());

  std::pair<SkipListLeafNode*, Status> statusNodePair = sentinel -> put("HDD", "Hard disk drive",
                                                                        stringKeyComparator(),
                                                                        [] {throw std::runtime_error("FailsWhilePuttingAKeyValueAndSucceedsWithOther");});
  ASSERT_FALSE(sentinel -> getBy("HDD", stringKeyComparator()).second);
  ASSERT_TRUE(sentinel -> getBy("SDD", stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileUpdatingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive", stringKeyComparator());

  Status status = sentinel -> update("SDD", "Solid state",
                                     stringKeyComparator(),
                                     [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});
  ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD", stringKeyComparator()).first);
  ASSERT_EQ(Status::Failed, status);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileUpdatingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive", stringKeyComparator());
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());

  sentinel -> update("HDD", "HDD", stringKeyComparator());
  sentinel -> update("SDD", "Solid state", stringKeyComparator(), [] {throw std::runtime_error("FailsWhileUpdatingAKeyValueAndSucceedsWithOther");});
  ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD", stringKeyComparator()).first);
  ASSERT_EQ("HDD", sentinel -> getBy("HDD", stringKeyComparator()).first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive", stringKeyComparator());

  Status status = sentinel -> deleteBy("SDD", stringKeyComparator(), [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});
  ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD", stringKeyComparator()).first);
  ASSERT_EQ(Status::Failed, status);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  sentinel -> put("SDD", "Solid state drive", stringKeyComparator());
  sentinel -> put("HDD", "Hard disk drive", stringKeyComparator());

  sentinel -> deleteBy("SDD", stringKeyComparator(), [] {throw std::runtime_error("FailsWhileDeletingAKeyValueAndSucceedsWithOther");});
  sentinel -> deleteBy("HDD", stringKeyComparator());
  ASSERT_EQ("Solid state drive", sentinel -> getBy("SDD", stringKeyComparator()).first);
  ASSERT_EQ("", sentinel -> getBy("HDD", stringKeyComparator()).first);
}