#ifndef _SkipList_
#define _SkipList_

#include <vector>
#include "SkipListNode.h"
#include "SkipListInternalNode.h"
#include "db/Status.h"
#include "db/SkipListArena.h"
#include "db/KeyValueSize.h"
#include "comparator/KeyComparator.h"
#include "utils/LevelGenerator.h"
#include "storage/PersistentMemoryPool.h"
#include "iterator/SkipListIterator.h"

namespace pmem {
    namespace storage {
        namespace internal {

            class SkipList {
            private:
                pmem::storage::internal::SkipListInternalNode *header;
                pmem::storage::internal::LevelGenerator *levelGenerator;
                pmem::storage::internal::SkipListArena *arena;
            public:
                // No copying allowed
                SkipList(const SkipList &copy) = delete;

                void operator=(const SkipList &) = delete;

                SkipList(LevelGenerator *levelGenerator,
                         pmem::storage::KeyComparator *keyComparator,
                         pmem::storage::internal::PersistentMemoryPool *persistentMemoryPool);

                ~SkipList();

                Status put(const char *key,
                           const char *value,
                           const KeyValueSize &keyValueSize);

                Status update(const char *key,
                              const char *value,
                              const KeyValueSize &keyValueSize);

                Status deleteBy(const char *key);

                std::pair<const char *, bool> get(const char *key);

                std::vector <std::pair<const char *, bool>> multiGet(const std::vector<const char *> &keys);

                std::vector <pmem::storage::KeyValuePair>
                scan(const char *beginKey, const char *endKey, int64_t maxPairs);

                unsigned long totalKeys();

                SkipListIterator newIterator(pmem::storage::KeyComparator *keyComparator);

                void close();
            };
        }
    }
}
#endif