#ifndef _Db_
#define _Db_

#include <string>
#include <vector>
#include <mutex>
#include <shared_mutex>

#include "Status.h"
#include "SkipList.h"
#include "PersistentMemoryPool.h"
#include "Configuration.h"

namespace pmem {
    namespace storage {
        class Db {
        private:
            SkipList *skipList;
            internal::PersistentMemoryPool* persistentMemoryPool;
            std::shared_mutex mutex_;

        public:
            static Db *open(Configuration configuration);

            internal::Status put(std::string key, std::string value);

            internal::Status update(std::string key, std::string value);

            internal::Status deleteBy(std::string key);

            std::pair<std::string, bool> get(std::string key);

            std::vector <std::pair<std::string, bool>> multiGet(const std::vector <std::string> &keys);

            std::vector <pmem::storage::KeyValuePair> scan(std::string beginKey, std::string endKey, int64_t maxPairs);

            void close();
        };
    }
}

#endif