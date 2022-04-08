#ifndef _SkipListLeafNode_
#define _SkipListLeafNode_

#include <functional>
#include "SkipListNode.h"
#include "PersistentLeaf.h"
#include "../db/Status.h"
#include "../comparator/KeyComparator.h"
#include "../db/KeyValueSize.h"

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

                void persist();

                bool matchesKey(const char *key, pmem::storage::KeyComparator* keyComparator) const override;

                bool isKeyLessEqualTo(const char *key, pmem::storage::KeyComparator* keyComparator) override;

                pmem::storage::KeyValuePair keyValuePair() override;

                pmem::storage::KeyValuePair rightKeyValuePair();

                std::pair<SkipListLeafNode *, Status>
                put(const char *key,
                    const char *value,
                    const KeyValueSize& keyValueSize,
                    pmem::storage::KeyComparator* keyComparator,
                    std::function<void(void)> postPutHook = [] {});

                std::pair<const char*, bool> getBy(const char *key, pmem::storage::KeyComparator* keyComparator);

                std::vector <pmem::storage::KeyValuePair>
                scan(const char *beginKey, const char *endKey, int64_t maxPairs,
                     pmem::storage::KeyComparator* keyComparator);

                Status update(const char *key,
                              const char *value,
                              const KeyValueSize& keyValueSize,
                              pmem::storage::KeyComparator* keyComparator,
                              std::function<void(void)> postUpdateHook = [] {});

                Status deleteBy(const char *key,
                                pmem::storage::KeyComparator* keyComparator,
                                std::function<void(void)> postDeleteHook = [] {});
            };
        }
    }
}

#endif