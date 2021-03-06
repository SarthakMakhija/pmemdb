#include <gtest/gtest.h>
#include <string>
#include "../../../src/engine/storage/SkipListLeafNode.h"
#include "../SkipListNodeTestUtils.h"
#include "PersistentMemoryPoolFixture.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

std::pair<SkipListLeafNode *, Status> put(SkipListLeafNode* node,
                                          const char* key,
                                          const char* value,
                                          PersistentMemoryPool* pool,
                                          std::function<void(void)> postPutHook = [] {}) {

    return node->put(Slice(key, strlen(key) + 1), 
                     Slice(value, strlen(value) + 1), 
                     stringKeyComparator(), 
                     pool,
                     postPutHook);
}

Status update(SkipListLeafNode* node,
              const char* key,
              const char* value,
              PersistentMemoryPool* pool,
              std::function<void(void)> postUpdateHook = [] {}) {
  
  return node->update(Slice(key, strlen(key) + 1), 
                      Slice(value, strlen(value) + 1), 
                      stringKeyComparator(), 
                      pool,
                      postUpdateHook); 
}

Status deleteBy(SkipListLeafNode* node,
                const char* key,
                PersistentMemoryPool* pool,
                std::function<void(void)> postDeleteHook = [] {}) {

  return node->deleteBy(Slice(key, strlen(key) + 1), stringKeyComparator(), pool, postDeleteHook);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhilePuttingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());

  std::pair<SkipListLeafNode*, Status> statusNodePair = put(sentinel,
                                                            "HDD",
                                                            "Hard disk drive",
                                                            PersistentMemoryPoolFixture::getPersistentMemoryPool(),
                                                            [] {throw std::runtime_error("FailsWhilePuttingAKeyValue");});

  ASSERT_FALSE(sentinel -> getBy(Slice("HDD"), stringKeyComparator()).second);
  ASSERT_EQ(Status::Failed, statusNodePair.second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhilePuttingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  put(sentinel,
            "HDD",
            "Hard disk drive",
            PersistentMemoryPoolFixture::getPersistentMemoryPool(),
            [] {throw std::runtime_error("FailsWhilePuttingAKeyValueAndSucceedsWithOther");});

  ASSERT_FALSE(sentinel -> getBy(Slice("HDD"), stringKeyComparator()).second);
  ASSERT_TRUE(sentinel -> getBy(Slice("SDD"), stringKeyComparator()).second);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileUpdatingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Status status = update(sentinel,
                         "SDD",
                         "Solid state",
                         PersistentMemoryPoolFixture::getPersistentMemoryPool(),
                         [] {throw std::runtime_error("FailsWhileUpdatingAKeyValue");});

  ASSERT_EQ(Slice("Solid state drive"), sentinel -> getBy(Slice("SDD"), stringKeyComparator()).first);
  ASSERT_EQ(Status::Failed, status);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileUpdatingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  update(sentinel, "HDD", "HDD", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  update(sentinel,
         "SDD",
         "Solid state",
         PersistentMemoryPoolFixture::getPersistentMemoryPool(),
         [] {throw std::runtime_error("FailsWhileUpdatingAKeyValueAndSucceedsWithOther");});

  ASSERT_EQ(Slice("Solid state drive"), sentinel -> getBy(Slice("SDD"), stringKeyComparator()).first);
  ASSERT_EQ(Slice("HDD"), sentinel -> getBy(Slice("HDD"), stringKeyComparator()).first);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyValue) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  Status status = deleteBy(sentinel, "SDD", PersistentMemoryPoolFixture::getPersistentMemoryPool(), [] {throw std::runtime_error("FailsWhileDeleting");});

  ASSERT_EQ(Slice("Solid state drive"), sentinel -> getBy(Slice("SDD"), stringKeyComparator()).first);
  ASSERT_EQ(Status::Failed, status);
}

TEST_F(PersistentMemoryPoolFixture, SkipListLeafNode_FailsWhileDeletingAKeyValueAndSucceedsWithOther) {
  SkipListLeafNode* sentinel = newSentinelLeafNode(PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "SDD", "Solid state drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());
  put(sentinel, "HDD", "Hard disk drive", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  deleteBy(sentinel, "SDD", PersistentMemoryPoolFixture::getPersistentMemoryPool(), [] {throw std::runtime_error("FailsWhileDeletingAKeyValueAndSucceedsWithOther");});
  deleteBy(sentinel, "HDD", PersistentMemoryPoolFixture::getPersistentMemoryPool());

  ASSERT_EQ(Slice("Solid state drive"), sentinel -> getBy(Slice("SDD"), stringKeyComparator()).first);
  ASSERT_EQ(Slice(""), sentinel -> getBy(Slice("HDD"), stringKeyComparator()).first);
}