#include "StringKeyComparator.h"

namespace pmem {
    namespace storage {
        int StringKeyComparator::compare(const Slice& a, const Slice& b) const {
            return strcmp(a.cdata(), b.cdata());
        }
    }
}