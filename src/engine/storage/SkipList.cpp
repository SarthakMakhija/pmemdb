#include <stdlib.h>
#include "SkipList.h"
#include "SkipListNode.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include "Slice.h"

namespace pmem {
    namespace storage {
        namespace internal {
            SkipList::SkipList(LevelGenerator *levelGenerator,
                               pmem::storage::KeyComparator *keyComparator,
                               internal::PersistentMemoryPool *persistentMemoryPool) {

                if (levelGenerator->getMaxLevel() < 1) {
                    throw std::invalid_argument("towerSize has to be greater than or equal to one");
                }
                auto *sentinelLeafNode = new pmem::storage::internal::SkipListLeafNode();
                sentinelLeafNode->persist(persistentMemoryPool);

                this->header = new pmem::storage::internal::SkipListInternalNode("", levelGenerator->getMaxLevel());
                this->header->attach(sentinelLeafNode);
                this->levelGenerator = levelGenerator;
                this->arena = new pmem::storage::internal::SkipListArena(this->header, keyComparator, persistentMemoryPool);
            }

            SkipList::~SkipList() {
                this->header->close();
                delete this->levelGenerator;
                delete this->arena;
            }

            Status SkipList::put(const Slice& key, const Slice& value) {
                return this->arena->put(key, value, this->levelGenerator);
            }

            Status SkipList::update(const Slice& key, const Slice& value) {
                return this->arena->update(key, value);
            }

            Status SkipList::deleteBy(const Slice& key) {
                return this->arena->deleteBy(key);
            }

            std::pair<Slice, bool> SkipList::get(const Slice& key) {
                return this->arena->getBy(key);
            }

            std::vector <std::pair<Slice, bool>> SkipList::multiGet(const std::vector<Slice> &keys) {               
                return this->arena->multiGet(keys);
            }

            std::vector <KeyValuePair> SkipList::scan(const Slice& beginKey, const Slice& endKey, int64_t maxPairs) {
                return this->arena->scan(beginKey, endKey, maxPairs);
            }

            unsigned long SkipList::totalKeys() {
                return this->arena->totalKeys();
            }

            SkipListIterator* SkipList::newIterator(pmem::storage::KeyComparator *keyComparator,
                                                     std::shared_mutex            &mutex,
                                                     const Slice                  *upperBound) {
                return new SkipListIterator(this->header, keyComparator, mutex, upperBound);
            }

            void SkipList::close() {
                delete this;
            }
        }
    }
}