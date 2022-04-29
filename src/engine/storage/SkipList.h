#ifndef _SkipList_
#define _SkipList_

#include <vector>
#include "SkipListNode.h"
#include "SkipListInternalNode.h"
#include "db/Status.h"
#include "comparator/KeyComparator.h"
#include "db/KeyValueSize.h"
#include "utils/LevelGenerator.h"

namespace pmem {
    namespace storage {
        namespace internal {

            class SkipList {
            private:
                pmem::storage::internal::SkipListInternalNode *header;
                pmem::storage::internal::LevelGenerator *levelGenerator;
            public:
                // No copying allowed
                SkipList(const SkipList &copy) = delete;
                void operator=(const SkipList&) = delete;

                explicit SkipList(LevelGenerator *levelGenerator);
                ~SkipList();

                Status put(const char *key,
                           const char *value,
                           const KeyValueSize &keyValueSize,
                           pmem::storage::KeyComparator *keyComparator);

                Status update(const char *key,
                              const char *value,
                              const KeyValueSize &keyValueSize,
                              pmem::storage::KeyComparator *keyComparator);

                Status deleteBy(const char *key, pmem::storage::KeyComparator *keyComparator);

                std::pair<const char *, bool> get(const char *key, pmem::storage::KeyComparator *keyComparator);

                std::vector <std::pair<const char *, bool>> multiGet(const std::vector<const char *> &keys,
                                                                     pmem::storage::KeyComparator *keyComparator);

                std::vector <pmem::storage::KeyValuePair>
                scan(const char *beginKey, const char *endKey, int64_t maxPairs,
                     pmem::storage::KeyComparator *keyComparator);

                unsigned long totalKeys();

                void close();
            };
        }
    }
}
#endif