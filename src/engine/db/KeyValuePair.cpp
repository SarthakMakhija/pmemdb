#include "KeyValuePair.h"

namespace pmem {
    namespace storage {
        KeyValuePair::KeyValuePair(const char* key, const char* value) : key(Slice(key)), value(Slice(value)) {
        }
        
        KeyValuePair::KeyValuePair(const Slice& key, const Slice& value): key(key), value(value) {            
        }

        Slice KeyValuePair::getKey() const {
            return key.cdata();
        }

        Slice KeyValuePair::getValue() const {
            return value.cdata();
        }

        bool KeyValuePair::operator==(const KeyValuePair &other) const {
            return key == other.key && value == other.value;
        }
    }
}