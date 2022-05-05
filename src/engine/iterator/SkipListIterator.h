#ifndef _SkipListIterator_
#define _SkipListIterator_

#include "comparator/KeyComparator.h"
#include "storage/SkipListInternalNode.h"

namespace pmem {
    namespace storage {
        class SkipListIterator {
        private:
            pmem::storage::internal::SkipListInternalNode *startingNode;
            pmem::storage::internal::SkipListInternalNode *currentNode;
            KeyComparator *keyComparator;

        public:
            // No copying allowed
            SkipListIterator(const SkipListIterator &copy) = delete;
            void operator=(const SkipListIterator &) = delete;

            SkipListIterator(pmem::storage::internal::SkipListInternalNode *startingNode, KeyComparator *keyComparator);
            bool isValid();
            void seekToFirst();
            void seekToLast();
            void seek(const char* key);
            void next();
            const char* key() const;
            const char* value() const;
        };
    }
}
#endif