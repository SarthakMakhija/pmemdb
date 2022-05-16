#ifndef _SkipListLeafNode_
#define _SkipListLeafNode_

#include <functional>
#include "SkipListNode.h"
#include "db/Status.h"
#include "comparator/KeyComparator.h"
#include "PersistentMemoryPool.h"
#include "Slice.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListLeafNode : public SkipListNode {
            private:
                SkipListLeafNode *right;
                persistent_ptr<PersistentLeaf> leaf;

            public:
                SkipListLeafNode();

                ~SkipListLeafNode();

                void persist(pmem::storage::internal::PersistentMemoryPool *pool);

                bool matchesKey(const char *key, pmem::storage::KeyComparator *keyComparator) const override;

                bool isKeyLessEqualTo(const char *key, pmem::storage::KeyComparator *keyComparator) override;

                pmem::storage::KeyValuePair keyValuePair() override;

                pmem::storage::KeyValuePair rightKeyValuePair();

                std::pair<SkipListLeafNode *, Status>
                put(const pmem::storage::Slice& key,
                    const pmem::storage::Slice& value,
                    pmem::storage::KeyComparator *keyComparator,
                    PersistentMemoryPool *pool,
                    std::function<void(void)> postPutHook = [] {});

                std::pair<const char *, bool> getBy(const pmem::storage::Slice& key, pmem::storage::KeyComparator *keyComparator);

                std::vector <pmem::storage::KeyValuePair>
                scan(const pmem::storage::Slice& beginKey,
                     const pmem::storage::Slice& endKey, 
                     int64_t maxPairs,
                     pmem::storage::KeyComparator *keyComparator);

                Status update(const pmem::storage::Slice& key,
                              const pmem::storage::Slice& value,
                              pmem::storage::KeyComparator *keyComparator,
                              PersistentMemoryPool *pool,
                              std::function<void(void)> postUpdateHook = [] {});

                Status deleteBy(const pmem::storage::Slice& key,
                                pmem::storage::KeyComparator *keyComparator,
                                PersistentMemoryPool *pool,
                                std::function<void(void)> postDeleteHook = [] {});
            };
        }
    }
}

#endif