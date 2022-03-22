#ifndef _SkipList_
#define _SkipList_

#include <string>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include "SkipListNode.h"
#include "SkipListInternalNode.h"

namespace pmem {
    namespace storage {
        class SkipList {
        private:
            pmem::storage::internal::SkipListInternalNode *header;
            std::shared_mutex mutex_;

            double probability;

        public:
            SkipList(int towerSize, double probability);

            void put(std::string key, std::string value);

            void update(std::string key, std::string value);

            void deleteBy(std::string key);

            void deleteRange(std::string beginKey, std::string endKey);

            std::pair<std::string, bool> get(std::string key);

            std::vector <std::pair<std::string, bool>> multiGet(const std::vector <std::string> &keys);

            std::vector <pmem::storage::KeyValuePair> scan(std::string beginKey, std::string endKey, int64_t maxPairs);
        };
    }
}
#endif