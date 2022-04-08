#ifndef _SkipListNode_
#define _SkipListNode_

#include <functional>
#include "../db/KeyValuePair.h"
#include "../comparator/KeyComparator.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListNode {
            public:
                virtual bool matchesKey(const char* key, pmem::storage::KeyComparator* keyComparator) const = 0;

                virtual bool isKeyLessEqualTo(const char* key, pmem::storage::KeyComparator* keyComparator) = 0;

                virtual pmem::storage::KeyValuePair keyValuePair() = 0;
            };
        }
    }
}
#endif