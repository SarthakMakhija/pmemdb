#ifndef _KeyValuePair_
#define _KeyValuePair_

#include <cstring>
#include "storage/Slice.h"

namespace pmem {
    namespace storage {
        class KeyValuePair {
        private:
            Slice key;
            Slice value;

        public:
            KeyValuePair(const char* key, const char* value);
            KeyValuePair(const Slice& key, const Slice& value);
            
            bool operator == (const KeyValuePair &other) const;

            Slice getKey() const ;
            Slice getValue() const;
        };
    }
}

#endif