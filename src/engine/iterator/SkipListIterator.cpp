#include "SkipListIterator.h"
#include "storage/SkipListInternalNode.h"

namespace pmem {
    namespace storage {
        SkipListIterator::SkipListIterator(pmem::storage::internal::SkipListInternalNode *startingNode, 
                                           KeyComparator                                 *keyComparator,
                                           std::shared_mutex                              &mutex)
            : startingNode{startingNode},
              currentNode{startingNode},
              keyComparator{keyComparator},
              mutex{mutex} {
        }

        bool SkipListIterator::isValid() const {
            std::shared_lock <std::shared_mutex> lock(mutex);
            if (currentNode->next() != nullptr) {
                return true;
            }
            return false;
        }

        void SkipListIterator::seekToFirst() {
            std::shared_lock <std::shared_mutex> lock(mutex);
            currentNode = startingNode;
        }

        void SkipListIterator::seekToLast() {
            std::shared_lock <std::shared_mutex> lock(mutex);
            pmem::storage::internal::SkipListInternalNode* node = currentNode;
            while (node->next() != nullptr) {
                node = node->next();
            }
            currentNode = node;
        }

        void SkipListIterator::seek(const char* key) {
            std::shared_lock <std::shared_mutex> lock(mutex);
            auto existenceByNode = startingNode->getBy(key, keyComparator);
            currentNode = static_cast<pmem::storage::internal::SkipListInternalNode *>(existenceByNode.first);
        }

        void SkipListIterator::next() {
            std::shared_lock <std::shared_mutex> lock(mutex);
            currentNode = currentNode->next();
        }

        const char* SkipListIterator::key() const {
            std::shared_lock <std::shared_mutex> lock(mutex);
            return currentNode->keyValuePair().getKey();
        }

        const char* SkipListIterator::value() const {
            std::shared_lock <std::shared_mutex> lock(mutex);
            auto leaf = currentNode->getDown();
            return static_cast<pmem::storage::internal::SkipListLeafNode *>(leaf)->keyValuePair().getValue();
        }
    }
}