#ifndef _KeyComparator_
#define _KeyComparator_

#include <stdexcept>

namespace pmem {
    namespace storage {
        class KeyComparator {
        public:
            virtual ~KeyComparator() = default;

            virtual int compare(char const *a, char const *b) const = 0;
        };
    }
}
#endif