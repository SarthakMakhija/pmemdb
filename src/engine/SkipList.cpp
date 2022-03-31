#include "SkipList.h"
#include "SkipListNode.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include "SkipListIterator.h"
#include <stdlib.h>

namespace pmem {
    namespace storage {
        namespace internal {

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

            Status SkipList::put(const char *key,
                                 const char *value,
                                 KeyValueSize keyValueSize,
                                 pmem::storage::KeyComparator* keyComparator) {

                return pmem::storage::internal::SkipListIterator(this->header, keyComparator).put(key,
                                                                                                  value,
                                                                                                  keyValueSize,
                                                                                                  this->probability);
            }

            Status SkipList::update(const char *key,
                                    const char *value,
                                    KeyValueSize keyValueSize,
                                    pmem::storage::KeyComparator* keyComparator) {

                return pmem::storage::internal::SkipListIterator(this->header, keyComparator).update(key, value, keyValueSize);
            }

            Status SkipList::deleteBy(const char *key, pmem::storage::KeyComparator* keyComparator) {
                return pmem::storage::internal::SkipListIterator(this->header, keyComparator).deleteBy(key);
            }

            std::pair<const char*, bool> SkipList::get(const char *key, pmem::storage::KeyComparator* keyComparator) {
                return pmem::storage::internal::SkipListIterator(this->header, keyComparator).getBy(key);
            }

            std::vector <std::pair<const char*, bool>> SkipList::multiGet(const std::vector<const char *> &keys,
                                                                          pmem::storage::KeyComparator* keyComparator) {
                return pmem::storage::internal::SkipListIterator(this->header, keyComparator).multiGet(keys);
            }

            std::vector <KeyValuePair> SkipList::scan(const char *beginKey, const char *endKey, int64_t maxPairs,
                                                      pmem::storage::KeyComparator* keyComparator) {
                return pmem::storage::internal::SkipListIterator(this->header, keyComparator).scan(beginKey, endKey,
                                                                                                   maxPairs);
            }

            unsigned long SkipList::totalKeys() {
                return pmem::storage::internal::SkipListIterator(this->header, nullptr).totalKeys();
            }
        }
    }
}