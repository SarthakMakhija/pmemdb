#include "./PersistentMemoryPool.h"

namespace pmem {
    namespace storage {
        namespace internal {
            PersistentMemoryPool *PersistentMemoryPool::instance = nullptr;

            PersistentMemoryPool *PersistentMemoryPool::initialize(const char *filePath, uint64_t size) {
                if (instance == nullptr) {
                    instance = new PersistentMemoryPool(filePath, size);
                }
                return instance;
            }

            PersistentMemoryPool *PersistentMemoryPool::getInstance() {
                if (instance == nullptr) {
                    throw std::logic_error("Can not invoke getInstance without initializing PersistentMemoryPool");
                }
                return instance;
            }

            pmem::obj::pool_base PersistentMemoryPool::getPmpool() {
                return this->pmpool;
            }
        }
    }
}