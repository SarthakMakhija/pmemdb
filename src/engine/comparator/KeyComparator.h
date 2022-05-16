#ifndef _KeyComparator_
#define _KeyComparator_

#include <stdexcept>
#include "storage/Slice.h"

namespace pmem {
    namespace storage {
        class KeyComparator {
        public:
            virtual ~KeyComparator() = default;

            virtual int compare(const Slice& a, const Slice& b) const = 0;
        };
    }
}
#endif