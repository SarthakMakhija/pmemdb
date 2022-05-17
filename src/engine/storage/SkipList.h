#ifndef _SkipList_
#define _SkipList_

#include <vector>
#include <mutex>
#include <shared_mutex>
#include "SkipListNode.h"
#include "SkipListInternalNode.h"
#include "db/Status.h"
#include "db/SkipListArena.h"
#include "comparator/KeyComparator.h"
#include "utils/LevelGenerator.h"
#include "storage/PersistentMemoryPool.h"
#include "storage/Slice.h"
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

                Status put(const Slice& key, const Slice& value);

                Status update(const Slice& key, const Slice& value);

                Status deleteBy(const Slice& key);

                std::pair<Slice, bool> get(const Slice& key);

                std::vector <std::pair<Slice, bool>> multiGet(const std::vector<Slice> &keys);

                std::vector <pmem::storage::KeyValuePair>
                scan(const Slice& key, const Slice& value, int64_t maxPairs);

                unsigned long totalKeys();

                SkipListIterator* newIterator(pmem::storage::KeyComparator *keyComparator,
                                             std::shared_mutex             &mutex);

                void close();
            };
        }
    }
}
#endif