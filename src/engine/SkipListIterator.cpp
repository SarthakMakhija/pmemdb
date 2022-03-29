#include "SkipListIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include <algorithm>

namespace pmem {
    namespace storage {
        namespace internal {
            SkipListIterator::SkipListIterator(SkipListNode *startingNode, KeyComparator* keyComparator)
                    : startingNode{startingNode}, keyComparator{keyComparator} {
            }

            Status SkipListIterator::put(const char *key, const char *value, double probability,
                                         std::function<void(void)> postPutHook) {

                PutPosition putPosition = static_cast<SkipListInternalNode *>(this->startingNode)->putPositionOf(key,
                                                                                                                 probability,
                                                                                                                 keyComparator);
                if (putPosition.leaf != nullptr) {
                    std::pair < SkipListLeafNode * ,
                            Status > statusNodePair = static_cast<SkipListLeafNode *>(putPosition.leaf)->put(key,
                                                                                                             value,
                                                                                                             keyComparator,
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

            std::vector <std::pair<const char*, bool>> SkipListIterator::multiGet(std::vector<const char *> keys) {
                std::vector <std::pair<const char*, bool>> result;

                std::sort(keys.begin(), keys.end(), [&](const char *c1, const char *c2) {
                    return keyComparator->compare(c1, c2) < 0;
                });
                for (auto key: keys) {
                    std::pair < SkipListNode * ,
                            bool > existenceByNode = static_cast<SkipListInternalNode *>(startingNode)->getBy(key, keyComparator);

                    if (existenceByNode.second) {
                        result.push_back(static_cast<SkipListLeafNode *>(existenceByNode.first)->getBy(key, keyComparator));
                    } else {
                        result.push_back(std::make_pair("", false));
                    }
                }
                return result;
            }

            std::pair<const char*, bool> SkipListIterator::getBy(const char *key) {
                std::pair < SkipListNode * ,
                        bool > existenceByNode = static_cast<SkipListInternalNode *>(this->startingNode)->getBy(key, keyComparator);

                if (existenceByNode.second) {
                    return static_cast<SkipListLeafNode *>(existenceByNode.first)->getBy(key, keyComparator);
                }
                return std::make_pair("", false);
            }

            std::vector <KeyValuePair>
            SkipListIterator::scan(const char *beginKey, const char *endKey, int64_t maxPairs) {
                std::pair < SkipListNode * ,
                        bool > existenceByNode = static_cast<SkipListInternalNode *>(this->startingNode)->scan(
                                beginKey, keyComparator);

                if (existenceByNode.second) {
                    return static_cast<SkipListLeafNode *>(existenceByNode.first)->scan(beginKey, endKey, maxPairs, keyComparator);
                }
                return std::vector<KeyValuePair>();
            }

            Status
            SkipListIterator::update(const char *key, const char *value, std::function<void(void)> postUpdateHook) {
                UpdatePosition updatePosition = static_cast<SkipListInternalNode *>(this->startingNode)->updatePositionOf(
                        key, keyComparator);

                if (updatePosition.leaf != nullptr) {
                    return static_cast<SkipListLeafNode *>(updatePosition.leaf)->update(key, value, keyComparator, postUpdateHook);
                }
                return Status::KeyNotFound;
            }

            Status SkipListIterator::deleteBy(const char *key, std::function<void(void)> postDeleteHook) {
                DeletePosition deletePosition = static_cast<SkipListInternalNode *>(this->startingNode)->deletePositionOf(
                        key, keyComparator);

                if (deletePosition.internal != nullptr && deletePosition.leaf != nullptr) {
                    Status status = static_cast<SkipListLeafNode *>(deletePosition.leaf)->deleteBy(key, keyComparator, postDeleteHook);
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
        }
    }
}