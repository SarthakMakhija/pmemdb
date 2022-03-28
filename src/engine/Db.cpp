#include "Db.h"

namespace pmem {
    namespace storage {
        Db *Db::open(Configuration configuration) {
            Db *db = new Db();

            db->persistentMemoryPool = internal::PersistentMemoryPool::initialize(
                    configuration.getFilePath(),
                    configuration.getFileSize()
            );

            db->skipList = new internal::SkipList(configuration.getSkipListTowerSize(),
                                        configuration.getProbability()
            );
            return db;
        }

        Status Db::put(const char* key, const char* value) {
            if (strlen(key) == 0 || strlen(value) == 0) {
                    throw std::invalid_argument("key and value can not be blank while putting");
            }

            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->put(key, value);
        }

        Status Db::update(const char* key, const char* value) {
            if (strlen(key) == 0 || strlen(value) == 0) {
                throw std::invalid_argument("key and value can not be blank while updating");
            }

            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->update(key, value);
        }

        Status Db::deleteBy(const char* key) {
            if (strlen(key) == 0) {
                throw std::invalid_argument("key can not be blank while deleting the corresponding value");
            }
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->deleteBy(key);
        }

        std::pair<std::string, bool> Db::get(const char* key) {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->get(key);
        }

        std::vector <std::pair<std::string, bool>> Db::multiGet(const std::vector <const char*> &keys) {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->multiGet(keys);
        }

        std::vector <pmem::storage::KeyValuePair> Db::scan(const char* beginKey, const char* endKey, int64_t maxPairs) {
            if (maxPairs <= 0) {
                throw std::invalid_argument("maxPairs must be greater than 0");
            }
            if (strcmp(beginKey, endKey) == 0 || strcmp(endKey, beginKey) < 0 ) {
                throw std::invalid_argument(
                        "beginKey and endKey must be different and endKey must be greater than beginKey");
            }

            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->scan(beginKey, endKey, maxPairs);
        }

        void Db::close() {
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            delete this->persistentMemoryPool;
        }
    }
}