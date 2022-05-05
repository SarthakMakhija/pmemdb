#include "SkipListIterator.h"
#include "storage/SkipListInternalNode.h"

namespace pmem {
    namespace storage {
        SkipListIterator::SkipListIterator(pmem::storage::internal::SkipListInternalNode *startingNode, KeyComparator *keyComparator) 
            : startingNode{startingNode},
              currentNode{startingNode},
              keyComparator{keyComparator} {
        }

        bool SkipListIterator::isValid() const {
            if (currentNode != nullptr) {
                return true;
            }
            return false;
        }

        void SkipListIterator::seekToFirst() {
            currentNode = startingNode;
        }

        void SkipListIterator::seekToLast() {
            pmem::storage::internal::SkipListInternalNode* node = currentNode;
            while (node->next() != nullptr) {
                node = node->next();
            }
            currentNode = node;
        }

        void SkipListIterator::seek(const char* key) {
            auto existenceByNode = startingNode->getBy(key, keyComparator);
            currentNode = static_cast<pmem::storage::internal::SkipListInternalNode *>(existenceByNode.first);
        }

        void SkipListIterator::next() {
            currentNode = currentNode->next();
        }

        const char* SkipListIterator::key() const {
            return currentNode->keyValuePair().getKey();
        }

        const char* SkipListIterator::value() const {
            auto leaf = currentNode->getDown();
            return static_cast<pmem::storage::internal::SkipListLeafNode *>(leaf)->keyValuePair().getValue();
        }
    }
}