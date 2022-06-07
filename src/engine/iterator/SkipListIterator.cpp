#include "SkipListIterator.h"
#include "storage/SkipListInternalNode.h"

namespace pmem {
    namespace storage {
        
        SkipListIterator::SkipListIterator(pmem::storage::internal::SkipListInternalNode *startingNode, 
                                           KeyComparator                                 *keyComparator,
                                           std::shared_mutex                             &mutex,
                                           const Slice                                   *upperBound)
            : startingNode{startingNode},
              currentNode{startingNode},
              keyComparator{keyComparator},
              mutex{mutex},
              upperBound{upperBound} {
        }

        bool SkipListIterator::isValid() const {
            std::shared_lock <std::shared_mutex> lock(this->mutex);
            if (this->currentNode != nullptr) {
                return true;
            }
            return false;
        }

        void SkipListIterator::seekToFirst() {
            std::shared_lock <std::shared_mutex> lock(this->mutex);
            this->currentNode = startingNode->next();
        }

        void SkipListIterator::seekToLast() {
            std::shared_lock <std::shared_mutex> lock(this->mutex);
            pmem::storage::internal::SkipListInternalNode* node = this->currentNode;
            while (node->next() != nullptr) {
                node = node->next();
            }
            this->currentNode = node;
        }

        void SkipListIterator::seek(const Slice& key) {
            if (this->upperBound != nullptr) {
                if (this->keyComparator->compare(key, *this->upperBound) >= 0 ) {
                    this->currentNode = nullptr;
                    return;
                }
            }
            std::shared_lock <std::shared_mutex> lock(this->mutex);
            auto existenceByNode = this->startingNode->seek(key, keyComparator);

            this->currentNode = static_cast<pmem::storage::internal::SkipListInternalNode *>(existenceByNode.first);            
        }

        void SkipListIterator::next() {
            std::shared_lock <std::shared_mutex> lock(this->mutex);
            this->currentNode = currentNode->next();
        }

        Slice SkipListIterator::key() const {
            std::shared_lock <std::shared_mutex> lock(this->mutex);
            return this->currentNode->keyValuePair().getKey();
        }

        Slice SkipListIterator::value() const {
            std::shared_lock <std::shared_mutex> lock(this->mutex);
            auto leafNode = this->currentNode->getDown();
            return static_cast<pmem::storage::internal::SkipListLeafNode *>(leafNode)->keyValuePair().getValue();
        }
    }
}