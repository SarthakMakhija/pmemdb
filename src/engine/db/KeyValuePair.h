#ifndef _KeyValuePair_
#define _KeyValuePair_

#include <cstring>

namespace pmem {
    namespace storage {
        class KeyValuePair {
        private:
            const char* key;
            const char* value;

        public:
            KeyValuePair(const char* key, const char* value);
            bool operator == (const KeyValuePair &other) const;

            const char* getKey() const ;
            const char* getValue() const;
        };
    }
}

#endif