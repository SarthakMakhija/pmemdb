#include "PersistentMemoryPool.h"

namespace pmem {
    namespace storage {
        namespace internal {
            PersistentMemoryPool *PersistentMemoryPool::initialize(const char *filePath, uint64_t size) {
                return new PersistentMemoryPool(filePath, size);
            }

            pmem::obj::pool_base PersistentMemoryPool::getPmpool() const {
                return this->pmpool;
            }
        }
    }
}