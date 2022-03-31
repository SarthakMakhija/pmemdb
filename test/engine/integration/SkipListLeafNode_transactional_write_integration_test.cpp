#include <gtest/gtest.h>
#include <string>
#include "../../../src/engine/storage/SkipListLeafNode.h"
#include "PersistentMemoryPoolFixture.h"
#include "../SkipListNodeTestUtils.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

std::pair<SkipListLeafNode *, Status> put(SkipListLeafNode* node,
                                          const char* key,
                                          const char* value,
                                          std::function<void(void)> postPutHook = [] {}) {

    return node->put(key, value, KeyValueSize(strlen(key) + 1, strlen(value) + 1), stringKeyComparator(), postPutHook);
}

Status update(SkipListLeafNode* node, const char* key, const char* value, std::function<void(void)> postUpdateHook = [] {}) {
    return node->update(key, value, KeyValueSize(strlen(key) + 1, strlen(value) + 1), stringKeyComparator(), postUpdateHook);
}

Status deleteBy(SkipListLeafNode* node, const char* key, std::function<void(void)> postDeleteHook = [] {}) {
    return node->deleteBy(key, stringKeyComparator(), postDeleteHook);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhilePuttingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();

  std::pair<SkipListLeafNode*, Status> statusNodePair = put(sentinel,
                                                            "HDD",
                                                            "Hard disk drive",
                                                            [] {throw std::runtime_error("FailsWhilePuttingAKeyValue");});

  ASSERT_FALSE(sentinel -> getBy("HDD", stringKeyComparator()).second);
  ASSERT_EQ(Status::Failed, statusNodePair.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhilePuttingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  put(sentinel, "SDD", "Solid state drive");

  std::pair<SkipListLeafNode*, Status> statusNodePair = put(sentinel,
                                                            "HDD",
                                                            "Hard disk drive",
                                                            [] {throw std::runtime_error("FailsWhilePuttingAKeyValueAndSucceedsWithOther");});

  ASSERT_FALSE(sentinel -> getBy("HDD", stringKeyComparator()).second);
  ASSERT_TRUE(sentinel -> getBy("SDD", stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileUpdatingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  put(sentinel, "SDD", "Solid state drive");

  Status status = update(sentinel,
                         "SDD",
                         "Solid state",
                         [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});

  ASSERT_EQ("Solid state drive", std::string(sentinel -> getBy("SDD", stringKeyComparator()).first));
  ASSERT_EQ(Status::Failed, status);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileUpdatingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  put(sentinel, "SDD", "Solid state drive");
  put(sentinel, "HDD", "Hard disk drive");

  update(sentinel, "HDD", "HDD");
  update(sentinel,
         "SDD",
         "Solid state",
         [] {throw std::runtime_error("FailsWhileUpdatingAKeyValueAndSucceedsWithOther");});

  ASSERT_EQ("Solid state drive", std::string(sentinel -> getBy("SDD", stringKeyComparator()).first));
  ASSERT_EQ("HDD", std::string(sentinel -> getBy("HDD", stringKeyComparator()).first));
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  put(sentinel, "SDD", "Solid state drive");

  Status status = deleteBy(sentinel, "SDD", [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});

  ASSERT_EQ("Solid state drive", std::string(sentinel -> getBy("SDD", stringKeyComparator()).first));
  ASSERT_EQ(Status::Failed, status);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode();
  put(sentinel, "SDD", "Solid state drive");
  put(sentinel, "HDD", "Hard disk drive");

  deleteBy(sentinel, "SDD", [] {throw std::runtime_error("FailsWhileDeletingAKeyValueAndSucceedsWithOther");});
  deleteBy(sentinel, "HDD");

  ASSERT_EQ("Solid state drive", std::string(sentinel -> getBy("SDD", stringKeyComparator()).first));
  ASSERT_EQ("", std::string(sentinel -> getBy("HDD", stringKeyComparator()).first));
}