#ifndef _KeyValueSize_
#define _KeyValueSize_

#include <cstddef>

namespace pmem {
    namespace storage {
        class KeyValueSize {

        private:
            size_t keySize;
            size_t valueSize;

        public:
            KeyValueSize(size_t keySize, size_t valueSize);

            size_t getKeySize();

            size_t getValueSize();
        };
    }
}

#endif
