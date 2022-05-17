#ifndef _SkipListArena_
#define _SkipListArena_

#include <functional>
#include "storage/SkipListNode.h"
#include "Status.h"
#include "comparator/KeyComparator.h"
#include "storage/utils/LevelGenerator.h"
#include "storage/PersistentMemoryPool.h"
#include "storage/Slice.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListArena {
            private:
                PersistentMemoryPool *persistentMemoryPool;
                SkipListNode *startingNode;
                pmem::storage::KeyComparator *keyComparator;

            public:
                SkipListArena(SkipListNode *startingNode,
                              pmem::storage::KeyComparator *keyComparator,
                              PersistentMemoryPool *persistentMemoryPool);

                Status put(const Slice& key,
                           const Slice& value, 
                           LevelGenerator *levelGenerator,
                           std::function<void(void)> postPutHook = [] {});

                std::pair<Slice, bool> getBy(const Slice& key);

                std::vector <std::pair<Slice, bool>> multiGet(std::vector<Slice> keys);

                std::vector <KeyValuePair> scan(const Slice& beginKey, const Slice& endKey, int64_t maxPairs);

                Status update(const Slice& key,
                              const Slice& value,
                              std::function<void(void)> postUpdateHook = [] {});

                Status deleteBy(const Slice& key, std::function<void(void)> postDeleteHook = [] {});

                unsigned long totalKeys();
            };
        }
    }
}
#endif