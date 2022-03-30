#include "KeyValueSize.h"

namespace pmem {
    namespace storage {
        KeyValueSize::KeyValueSize(size_t keySize, size_t valueSize) : keySize(keySize), valueSize(valueSize) {}

        size_t KeyValueSize::getKeySize() {
            return keySize;
        }

        size_t KeyValueSize::getValueSize() {
            return valueSize;
        }
    }
}