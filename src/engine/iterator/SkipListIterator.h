#ifndef _SkipListIterator_
#define _SkipListIterator_

#include <mutex>
#include <shared_mutex>
#include "comparator/KeyComparator.h"
#include "storage/SkipListInternalNode.h"
#include "storage/Slice.h"

namespace pmem {
    namespace storage {
        class SkipListIterator {
        private:
            pmem::storage::internal::SkipListInternalNode *startingNode;
            pmem::storage::internal::SkipListInternalNode *currentNode;
            KeyComparator *keyComparator;
            std::shared_mutex &mutex;
            const Slice             *upperBound;  

        public:
            // No copying allowed
            SkipListIterator(const SkipListIterator &copy) = delete;
            void operator=(const SkipListIterator &) = delete;

            SkipListIterator(pmem::storage::internal::SkipListInternalNode *startingNode, 
                             KeyComparator                                 *keyComparator,
                             std::shared_mutex                             &mutex,
                             const Slice                                   *upperBound);


            bool isValid() const;
            void seekToFirst();
            void seekToLast();
            void seek(const Slice& key);
            void next();
            Slice key() const;
            Slice value() const;
        };
    }
}
#endif