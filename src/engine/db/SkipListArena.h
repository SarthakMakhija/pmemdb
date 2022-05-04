#ifndef _SkipListArena_
#define _SkipListArena_

#include <functional>
#include "storage/SkipListNode.h"
#include "Status.h"
#include "comparator/KeyComparator.h"
#include "KeyValueSize.h"
#include "storage/utils/LevelGenerator.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListArena {
            private:
                SkipListNode *startingNode;
                pmem::storage::KeyComparator *keyComparator;

            public:
                SkipListArena(SkipListNode *startingNode, pmem::storage::KeyComparator *keyComparator);

                Status put(const char *key,
                           const char *value,
                           const KeyValueSize& keyValueSize,
                           LevelGenerator* levelGenerator,
                           std::function<void(void)> postPutHook = [] {});

                std::pair<const char *, bool> getBy(const char *key);

                std::vector <std::pair<const char *, bool>> multiGet(std::vector<const char *> keys);

                std::vector <KeyValuePair> scan(const char *beginKey, const char *endKey, int64_t maxPairs);

                Status update(const char *key,
                              const char *value,
                              const KeyValueSize& keyValueSize,
                              std::function<void(void)> postUpdateHook = [] {});

                Status deleteBy(const char *key, std::function<void(void)> postDeleteHook = [] {});

                unsigned long totalKeys();
            };
        }
    }
}
#endif