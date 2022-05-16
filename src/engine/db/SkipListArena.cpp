#include "SkipListArena.h"
#include "storage/SkipListInternalNode.h"
#include "storage/SkipListLeafNode.h"
#include "storage/Slice.h"
#include <algorithm>

namespace pmem {
    namespace storage {
        namespace internal {
            SkipListArena::SkipListArena(SkipListNode *startingNode,
                                         KeyComparator *keyComparator,
                                         PersistentMemoryPool *persistentMemoryPool)
                    : startingNode{startingNode},
                      keyComparator{keyComparator},
                      persistentMemoryPool{persistentMemoryPool} {
            }

            Status SkipListArena::put(const char *key,
                                      const char *value,
                                      const KeyValueSize &keyValueSize,
                                      LevelGenerator *levelGenerator,
                                      std::function<void(void)> postPutHook) {

                PutPosition putPosition = static_cast<SkipListInternalNode *>(this->startingNode)->putPositionOf(key,
                                                                                                                 keyComparator,
                                                                                                                 levelGenerator);
                if (putPosition.leaf != nullptr) {
                    std::pair < SkipListLeafNode * ,
                            Status > statusNodePair = static_cast<SkipListLeafNode *>(putPosition.leaf)->put(pmem::storage::Slice(key, keyValueSize.getKeySize()),
                                                                                                             pmem::storage::Slice(value, keyValueSize.getValueSize()),
                                                                                                             keyComparator,
                                                                                                             this->persistentMemoryPool,
                                                                                                             postPutHook);
                    if (statusNodePair.second != Status::Failed) {
                        SkipListLeafNode *newLeaf = statusNodePair.first;
                        SkipListNode *newInternal = static_cast<SkipListInternalNode *>(putPosition.internal)->put(key,
                                                                                                                   putPosition.positions,
                                                                                                                   putPosition.newLevel);
                        static_cast<SkipListInternalNode *>(newInternal)->attach(newLeaf);
                    }
                    return statusNodePair.second;
                }
                return Status::KeyAlreadyExists;
            }

            std::vector <std::pair<const char *, bool>> SkipListArena::multiGet(std::vector<const char *> keys) {
                std::vector <std::pair<const char *, bool>> result;

                std::sort(keys.begin(), keys.end(), [&](const char *c1, const char *c2) {
                    return keyComparator->compare(c1, c2) < 0;
                });
                for (auto key: keys) {
                    std::pair < SkipListNode * ,
                            bool > existenceByNode = static_cast<SkipListInternalNode *>(startingNode)->getBy(key,
                                                                                                              keyComparator);

                    if (existenceByNode.second) {
                        auto leaf = static_cast<SkipListInternalNode *>(existenceByNode.first)->getDown();
                        result.push_back(static_cast<SkipListLeafNode *>(leaf)->getBy(pmem::storage::Slice(key, strlen(key)+1), keyComparator));
                    } else {
                        result.push_back(std::make_pair("", false));
                    }
                }
                return result;
            }

            std::pair<const char *, bool> SkipListArena::getBy(const char *key) {
                std::pair < SkipListNode * ,
                        bool > existenceByNode = static_cast<SkipListInternalNode *>(this->startingNode)->getBy(key,
                                                                                                                keyComparator);

                if (existenceByNode.second) {
                    auto leaf = static_cast<SkipListInternalNode *>(existenceByNode.first)->getDown();
                    return static_cast<SkipListLeafNode *>(leaf)->getBy(pmem::storage::Slice(key, strlen(key)+1), keyComparator);
                }
                return std::make_pair("", false);
            }

            std::vector <KeyValuePair>
            SkipListArena::scan(const char *beginKey, const char *endKey, int64_t maxPairs) {
                std::pair < SkipListNode * ,
                        bool > existenceByNode = static_cast<SkipListInternalNode *>(this->startingNode)->scan(
                                beginKey, keyComparator);

                if (existenceByNode.second) {
                    return static_cast<SkipListLeafNode *>(existenceByNode.first)->scan(pmem::storage::Slice(beginKey, strlen(beginKey)+1), 
                                                                                        pmem::storage::Slice(endKey, strlen(endKey)+1), 
                                                                                        maxPairs,
                                                                                        keyComparator);
                }
                return std::vector<KeyValuePair>();
            }

            Status
            SkipListArena::update(const char *key,
                                  const char *value,
                                  const KeyValueSize &keyValueSize,
                                  std::function<void(void)> postUpdateHook) {

                UpdatePosition updatePosition = static_cast<SkipListInternalNode *>(this->startingNode)->updatePositionOf(
                        key, keyComparator);

                if (updatePosition.leaf != nullptr) {
                    return static_cast<SkipListLeafNode *>(updatePosition.leaf)->update(pmem::storage::Slice(key, keyValueSize.getKeySize()),
                                                                                        pmem::storage::Slice(value, keyValueSize.getValueSize()),
                                                                                        keyComparator,
                                                                                        this->persistentMemoryPool,
                                                                                        postUpdateHook);
                }
                return Status::KeyNotFound;
            }

            Status SkipListArena::deleteBy(const char *key, std::function<void(void)> postDeleteHook) {
                DeletePosition deletePosition = static_cast<SkipListInternalNode *>(this->startingNode)->deletePositionOf(
                        key, keyComparator);

                if (deletePosition.internal != nullptr && deletePosition.leaf != nullptr) {
                    Status status = static_cast<SkipListLeafNode *>(deletePosition.leaf)->deleteBy(pmem::storage::Slice(key, strlen(key)+1),
                                                                                                   keyComparator,
                                                                                                   this->persistentMemoryPool,
                                                                                                   postDeleteHook);
                    if (status != Status::Failed) {
                        static_cast<SkipListInternalNode *>(deletePosition.internal)->deleteBy(key,
                                                                                               deletePosition.positions,
                                                                                               deletePosition.deleteLevel,
                                                                                               keyComparator);
                    }
                    return status;
                }
                return Status::KeyNotFound;
            }

            unsigned long SkipListArena::totalKeys() {
                return static_cast<SkipListInternalNode *>(this->startingNode)->totalKeys();
            }
        }
    }
}