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

                Status put(std::string key, std::string value);

                Status update(std::string key, std::string value);

                Status deleteBy(std::string key);

                std::pair<std::string, bool> get(std::string key);

                std::vector <std::pair<std::string, bool>> multiGet(const std::vector <std::string> &keys);

                std::vector <pmem::storage::KeyValuePair>
                scan(std::string beginKey, std::string endKey, int64_t maxPairs);
            };
        }
    }
}
#endif