#ifndef _StringKeyComparator_
#define _StringKeyComparator_

#include "KeyComparator.h"
#include <cstring>

namespace pmem {
    namespace storage {
        class StringKeyComparator : public KeyComparator {
        public:
            int compare(char const* a, char const* b) const override;
        };
    }
}
#endif