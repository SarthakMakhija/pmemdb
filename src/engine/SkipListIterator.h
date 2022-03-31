#ifndef _SkipListIterator_
#define _SkipListIterator_

#include <functional>
#include "SkipListNode.h"
#include "Status.h"
#include "KeyComparator.h"
#include "KeyValueSize.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListIterator {
            private:
                SkipListNode *startingNode;
                pmem::storage::KeyComparator *keyComparator;

            public:
                SkipListIterator(SkipListNode *startingNode, pmem::storage::KeyComparator *keyComparator);

                Status put(const char *key,
                           const char *value,
                           KeyValueSize keyValueSize,
                           std::function<void(void)> postPutHook = [] {});

                std::pair<const char *, bool> getBy(const char *key);

                std::vector <std::pair<const char *, bool>> multiGet(std::vector<const char *> keys);

                std::vector <KeyValuePair> scan(const char *beginKey, const char *endKey, int64_t maxPairs);

                Status update(const char *key,
                              const char *value,
                              KeyValueSize keyValueSize,
                              std::function<void(void)> postUpdateHook = [] {});

                Status deleteBy(const char *key, std::function<void(void)> postDeleteHook = [] {});

                unsigned long totalKeys();
            };
        }
    }
}
#endif