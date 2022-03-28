#ifndef _SkipListNode_
#define _SkipListNode_

#include <string>
#include <functional>
#include "KeyValuePair.h"
#include "KeyComparator.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListNode {
            public:
                virtual bool isLeaf() = 0;

                virtual bool matchesKey(const char* key, pmem::storage::KeyComparator* keyComparator) const = 0;

                virtual bool isKeyLessEqualTo(const char* key, pmem::storage::KeyComparator* keyComparator) = 0;

                virtual pmem::storage::KeyValuePair keyValuePair() = 0;
            };
        }
    }
}
#endif