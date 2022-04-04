#ifndef _Db_
#define _Db_

#include <vector>
#include <mutex>
#include <shared_mutex>

#include "Status.h"
#include "../storage/SkipList.h"
#include "../storage/PersistentMemoryPool.h"
#include "../configuration/Configuration.h"
#include "KeyValueSize.h"

namespace pmem {
    namespace storage {
        class Db {
        private:
            internal::SkipList *skipList;
            internal::PersistentMemoryPool* persistentMemoryPool;
            std::shared_mutex mutex_;
            KeyComparator* keyComparator;

        public:
            static Db *open(const Configuration& configuration);

            Status put(const char* key, const char* value, const KeyValueSize& keyValueSize);

            Status update(const char* key, const char* value, const KeyValueSize& keyValueSize);

            Status deleteBy(const char* key);

            std::pair<const char*, bool> get(const char* key);

            std::vector <std::pair<const char*, bool>> multiGet(const std::vector <const char*> &keys);

            std::vector <pmem::storage::KeyValuePair> scan(const char* beginKey, const char* endKey, int64_t maxPairs);

            unsigned long totalKeys();

            void close();
        };
    }
}

#endif