#include "Db.h"
#include "../storage/utils/LevelGenerator.h"

namespace pmem {
    namespace storage {
        Db::Db() {
            this->keyComparator = nullptr;
            this->persistentMemoryPool = nullptr;
            this->skipList = nullptr;
        }

        Db::~Db() {
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            delete this->skipList;
            delete this->keyComparator;
            delete this->persistentMemoryPool;
        }

        Db *Db::open(const Configuration &configuration) {
            Db *db = new Db();

            db->persistentMemoryPool = internal::PersistentMemoryPool::initialize(
                    configuration.getFilePath(),
                    configuration.getFileSize()
            );

            db->keyComparator = configuration.getKeyComparator();
            db->skipList = new internal::SkipList(new internal::LevelGenerator(configuration.getSkipListMaxLevel()),
                                                  db->keyComparator,
                                                  db->persistentMemoryPool);
            return db;
        }

        Status Db::put(const char *key, const char *value, const KeyValueSize &keyValueSize) {
            //TODO: Handle blank key
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->put(key, value, keyValueSize);
        }

        Status Db::update(const char *key, const char *value, const KeyValueSize &keyValueSize) {
            //TODO: Handle blank key
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->update(key, value, keyValueSize);
        }

        Status Db::deleteBy(const char *key) {
            //TODO: Handle blank key
            std::lock_guard <std::shared_mutex> lock(this->mutex_);
            return this->skipList->deleteBy(key);
        }

        std::pair<const char *, bool> Db::get(const char *key) {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->get(key);
        }

        std::vector <std::pair<const char *, bool>> Db::multiGet(const std::vector<const char *> &keys) {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->multiGet(keys);
        }

        std::vector <pmem::storage::KeyValuePair> Db::scan(const char *beginKey, const char *endKey, int64_t maxPairs) {
            if (maxPairs <= 0) {
                throw std::invalid_argument("maxPairs must be greater than 0");
            }
            auto comparisonResult = this->keyComparator->compare(endKey, beginKey);
            if (comparisonResult == 0 || comparisonResult < 0) {
                throw std::invalid_argument(
                        "beginKey and endKey must be different and endKey must be greater than beginKey");
            }

            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->scan(beginKey, endKey, maxPairs);
        }

        unsigned long Db::totalKeys() {
            std::shared_lock <std::shared_mutex> lock(this->mutex_);
            return this->skipList->totalKeys();
        }

        SkipListIterator Db::newIterator() {
            return this->skipList->newIterator(this->keyComparator);
        }

        void Db::close() {
            delete this;
        }
    }
}