#ifndef _SkipListLeafNode_
#define _SkipListLeafNode_

#include <string>
#include <functional>
#include "SkipListNode.h"
#include "PersistentLeaf.h"
#include "Status.h"
#include "KeyComparator.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListLeafNode : public SkipListNode {
            private:
                SkipListLeafNode *right;
                persistent_ptr<PersistentLeaf> leaf;

            public:
                SkipListLeafNode();

                void persist();

                bool isLeaf();

                bool matchesKey(const char *key, pmem::storage::KeyComparator* keyComparator) const;

                bool isKeyLessEqualTo(const char *key, pmem::storage::KeyComparator* keyComparator);

                pmem::storage::KeyValuePair keyValuePair();

                pmem::storage::KeyValuePair rightKeyValuePair();

                std::pair<SkipListLeafNode *, Status>
                put(const char *key, const char *value, pmem::storage::KeyComparator* keyComparator,
                    std::function<void(void)> postPutHook = [] {});

                std::pair<std::string, bool> getBy(const char *key, pmem::storage::KeyComparator* keyComparator);

                std::vector <pmem::storage::KeyValuePair>
                scan(const char *beginKey, const char *endKey, int64_t maxPairs,
                     pmem::storage::KeyComparator* keyComparator);

                Status update(const char *key, const char *value,
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