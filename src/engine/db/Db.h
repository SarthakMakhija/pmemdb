#ifndef _Db_
#define _Db_

#include <vector>
#include <mutex>
#include <shared_mutex>

#include "Status.h"
#include "storage/SkipList.h"
#include "storage/PersistentMemoryPool.h"
#include "storage/Slice.h"
#include "configuration/Configuration.h"
#include "iterator/SkipListIterator.h"

namespace pmem {
    namespace storage {
        class Db {
        private:
            Db();
            internal::SkipList *skipList;
            internal::PersistentMemoryPool* persistentMemoryPool;
            std::shared_mutex mutex_;
            KeyComparator* keyComparator;

        public:
            ~Db();
            static Db *open(const Configuration& configuration);

            Status put(const Slice& key, const Slice& value);

            Status update(const Slice& key, const Slice& value);

            Status deleteBy(const Slice& key);

            std::pair<Slice, bool> get(const Slice& key);

            std::vector <std::pair<Slice, bool>> multiGet(const std::vector <Slice> &keys);

            std::vector <pmem::storage::KeyValuePair> scan(const Slice& beginKey, const Slice& endKey, int64_t maxPairs);

            unsigned long totalKeys();

            SkipListIterator* newIterator();

            void close();
        };
    }
}

#endif