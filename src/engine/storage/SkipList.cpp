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

            Status SkipList::put(const char *key,
                                 const char *value,
                                 const KeyValueSize &keyValueSize) {

                return this->arena->put(Slice(key, keyValueSize.getKeySize()),
                                        Slice(value, keyValueSize.getValueSize()),
                                        this->levelGenerator);
            }

            Status SkipList::update(const char *key,
                                    const char *value,
                                    const KeyValueSize &keyValueSize) {

                return this->arena->update(Slice(key, keyValueSize.getKeySize()), Slice(value, keyValueSize.getValueSize()));
            }

            Status SkipList::deleteBy(const char *key) {
                return this->arena->deleteBy(Slice(key, strlen(key) + 1));
            }

            std::pair<const char *, bool> SkipList::get(const char *key) {
                return this->arena->getBy(Slice(key, strlen(key) + 1));
            }

            std::vector <std::pair<const char *, bool>> SkipList::multiGet(const std::vector<const char *> &keys) {
                std::vector<Slice> keySlices;
                for (auto key: keys) {
                    keySlices.push_back(Slice(key, strlen(key) + 1));
                }
                return this->arena->multiGet(keySlices);
            }

            std::vector <KeyValuePair> SkipList::scan(const char *beginKey, const char *endKey, int64_t maxPairs) {
                return this->arena->scan(Slice(beginKey, strlen(beginKey) + 1), Slice(endKey, strlen(endKey) + 1), maxPairs);
            }

            unsigned long SkipList::totalKeys() {
                return this->arena->totalKeys();
            }

            SkipListIterator* SkipList::newIterator(pmem::storage::KeyComparator *keyComparator,
                                                   std::shared_mutex             &mutex) {
                return new SkipListIterator(this->header, keyComparator, mutex);
            }

            void SkipList::close() {
                delete this;
            }
        }
    }
}