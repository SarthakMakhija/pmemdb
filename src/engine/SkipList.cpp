#include "SkipList.h"
#include "SkipListNode.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include "SkipListIterator.h"
#include <stdlib.h>

namespace pmem {
    namespace storage {
        SkipList::SkipList(int towerSize, double probability) {
            if (towerSize < 1) {
                throw std::invalid_argument("towerSize has to be greater than or equal to one");
            }
            if (probability < 0 || probability > 1) {
                throw std::invalid_argument("probability must be less than 1 and greater than zero");
            }
            auto *sentinelLeafNode = new pmem::storage::internal::SkipListLeafNode();
            sentinelLeafNode->persist();

            this->header = new pmem::storage::internal::SkipListInternalNode("", towerSize);
            this->header->attach(sentinelLeafNode);
            this->probability = probability;
        }

        pmem::storage::internal::Status SkipList::put(std::string key, std::string value) {
            return pmem::storage::internal::SkipListIterator(this->header).put(key, value, this->probability);
        }

        pmem::storage::internal::Status SkipList::update(std::string key, std::string value) {
            return pmem::storage::internal::SkipListIterator(this->header).update(key, value);
        }

        pmem::storage::internal::Status SkipList::deleteBy(std::string key) {
            return pmem::storage::internal::SkipListIterator(this->header).deleteBy(key);
        }

        std::pair<std::string, bool> SkipList::get(std::string key) {
            return pmem::storage::internal::SkipListIterator(this->header).getBy(key);
        }

        std::vector <std::pair<std::string, bool>> SkipList::multiGet(const std::vector <std::string> &keys) {
            return pmem::storage::internal::SkipListIterator(this->header).multiGet(keys);
        }

        std::vector <KeyValuePair> SkipList::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
            return pmem::storage::internal::SkipListIterator(this->header).scan(beginKey, endKey, maxPairs);
        }
    }
}