#ifndef _SkipListNode_
#define _SkipListNode_

#include <functional>
#include "db/KeyValuePair.h"
#include "comparator/KeyComparator.h"
#include "Slice.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListNode {
            public:
                virtual ~SkipListNode() = default;

                virtual bool matchesKey(const Slice& key, pmem::storage::KeyComparator* keyComparator) const = 0;

                virtual bool isKeyLessEqualTo(const Slice& key, pmem::storage::KeyComparator* keyComparator) = 0;

                virtual pmem::storage::KeyValuePair keyValuePair() = 0;
            };
        }
    }
}
#endif