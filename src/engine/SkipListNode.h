#ifndef _SkipListNode_
#define _SkipListNode_

#include <string>
#include <functional>
#include "KeyValuePair.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListNode {
            public:
                virtual bool isLeaf() = 0;

                virtual bool matchesKey(std::string key) const = 0;

                virtual bool isKeyLessEqualTo(std::string key) = 0;

                virtual pmem::storage::KeyValuePair keyValuePair() = 0;
            };
        }
    }
}
#endif