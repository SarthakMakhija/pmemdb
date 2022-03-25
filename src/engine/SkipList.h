#ifndef _SkipList_
#define _SkipList_

#include <string>
#include <vector>
#include "SkipListNode.h"
#include "SkipListInternalNode.h"
#include "Status.h"

namespace pmem {
    namespace storage {
        namespace internal {

            class SkipList {
            private:
                pmem::storage::internal::SkipListInternalNode *header;
                double probability;

            public:
                SkipList(int towerSize, double probability);

                Status put(const char* key, const char* value);

                Status update(const char* key, const char* value);

                Status deleteBy(const char* key);

                std::pair<std::string, bool> get(const char* key);

                std::vector <std::pair<std::string, bool>> multiGet(const std::vector <std::string> &keys);

                std::vector <pmem::storage::KeyValuePair>
                scan(const char* beginKey, const char* endKey, int64_t maxPairs);
            };
        }
    }
}
#endif