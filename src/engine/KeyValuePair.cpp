#include "KeyValuePair.h"

namespace pmem {
    namespace storage {
        KeyValuePair::KeyValuePair(const char* key, const char* value) : key(key), value(value) {
        }

        const char* KeyValuePair::getKey() const {
            return key;
        }

        const char* KeyValuePair::getValue() const {
            return value;
        }

        bool KeyValuePair::operator==(const KeyValuePair &other) const {
            return strcmp(key, other.key) == 0 && strcmp(value, other.value) == 0;
        }
    }
}