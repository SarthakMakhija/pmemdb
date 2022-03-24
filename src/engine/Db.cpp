#include "Db.h"

namespace pmem {
    namespace storage {
        Db *Db::open(Configuration configuration) {
            Db *db = new Db();

            db->persistentMemoryPool = internal::PersistentMemoryPool::initialize(
                    configuration.getFilePath(),
                    configuration.getFileSize()
            );

            db->skipList = new SkipList(configuration.getSkipListTowerSize(),
                                        configuration.getProbability()
            );
            return db;
        }

        internal::Status Db::put(std::string key, std::string value) {
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->put(key, value);
        }

        internal::Status Db::update(std::string key, std::string value) {
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->update(key, value);
        }

        internal::Status Db::deleteBy(std::string key) {
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->deleteBy(key);
        }

        std::pair<std::string, bool> Db::get(std::string key) {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->get(key);
        }

        std::vector <std::pair<std::string, bool>> Db::multiGet(const std::vector <std::string> &keys) {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->multiGet(keys);
        }

        std::vector <pmem::storage::KeyValuePair> Db::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->scan(beginKey, endKey, maxPairs);
        }

        void Db::close() {
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            delete this->persistentMemoryPool;
        }
    }
}