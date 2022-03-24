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
            return this->skipList->put(key, value);
        }

        internal::Status Db::update(std::string key, std::string value) {
            return this->skipList->update(key, value);
        }

        internal::Status Db::deleteBy(std::string key) {
            return this->skipList->deleteBy(key);
        }

        std::pair<std::string, bool> Db::get(std::string key) {
            return this->skipList->get(key);
        }

        std::vector <std::pair<std::string, bool>> Db::multiGet(const std::vector <std::string> &keys) {
            return this->skipList->multiGet(keys);
        }

        std::vector <pmem::storage::KeyValuePair> Db::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
            return this->skipList->scan(beginKey, endKey, maxPairs);
        }

        void Db::close() {
            delete this->persistentMemoryPool;
        }
    }
}