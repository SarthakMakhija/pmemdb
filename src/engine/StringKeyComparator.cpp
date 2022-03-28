#include "StringKeyComparator.h"

namespace pmem {
    namespace storage {
        int StringKeyComparator::compare(const char *a, const char *b) const {
            return strcmp(a, b);
        }
    }
}