#include "Db.h"
#include "../storage/utils/LevelGenerator.h"

namespace pmem {
    namespace storage {
        Db *Db::open(const Configuration& configuration) {
            Db *db = new Db();

            db->persistentMemoryPool = internal::PersistentMemoryPool::initialize(
                    configuration.getFilePath(),
                    configuration.getFileSize()
            );

            db->skipList = new internal::SkipList(new internal::LevelGenerator(configuration.getSkipListTowerSize()));
            db->keyComparator = configuration.getKeyComparator();
            return db;
        }

        Status Db::put(const char* key, const char* value, const KeyValueSize& keyValueSize) {
            //TODO: Handle blank key
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->put(key, value, keyValueSize, this->keyComparator);
        }

        Status Db::update(const char* key, const char* value, const KeyValueSize& keyValueSize) {
            //TODO: Handle blank key
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->update(key, value, keyValueSize, this->keyComparator);
        }

        Status Db::deleteBy(const char* key) {
            //TODO: Handle blank key
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->deleteBy(key, this->keyComparator);
        }

        std::pair<const char*, bool> Db::get(const char* key) {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->get(key, this->keyComparator);
        }

        std::vector <std::pair<const char*, bool>> Db::multiGet(const std::vector <const char*> &keys) {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->multiGet(keys, this->keyComparator);
        }

        std::vector <pmem::storage::KeyValuePair> Db::scan(const char* beginKey, const char* endKey, int64_t maxPairs) {
            if (maxPairs <= 0) {
                throw std::invalid_argument("maxPairs must be greater than 0");
            }
            auto comparisonResult = this->keyComparator->compare(endKey, beginKey);
            if (comparisonResult == 0 || comparisonResult < 0 ) {
                throw std::invalid_argument(
                        "beginKey and endKey must be different and endKey must be greater than beginKey");
            }

            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->scan(beginKey, endKey, maxPairs, this->keyComparator);
        }

        unsigned long Db::totalKeys() {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->totalKeys();
        }

        void Db::close() {
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            this->keyComparator = nullptr;
            delete this->keyComparator;
            delete this->persistentMemoryPool;
        }
    }
}