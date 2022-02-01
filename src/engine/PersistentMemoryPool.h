#ifndef LIBPMEMKV_PMEMOBJ_ENGINE_H
#define LIBPMEMKV_PMEMOBJ_ENGINE_H

#include <libpmemobj++/pool.hpp>
#include <string>
#include "PersistentLeaf.h"

class PersistentMemoryPool {

    public:
    PersistentMemoryPool(PersistentMemoryPool &other) = delete;

    void operator=(const PersistentMemoryPool &) = delete;

    static PersistentMemoryPool* initialize(const char* filePath, uint64_t size = 8  * 1024 * 1024);

    static PersistentMemoryPool* getInstance();

	~PersistentMemoryPool() {
        try {
            pmpool.close();
        } catch (const std::logic_error &e) {
            std::terminate();
        }
	}

    protected:
	struct Root {
		pmem::obj::persistent_ptr<PersistentLeaf> ptr;
	};

	pmem::obj::pool_base pmpool;
	PMEMoid *root_oid;

    private:
	static PersistentMemoryPool* instance;

	PersistentMemoryPool(const char* filePath, uint64_t size = 8  * 1024 * 1024) {
        bool openFailed = false;
		std::string layout = "skiplist";

        try {
            pmpool = pmem::obj::pool<Root>::open(filePath, layout);
        } catch (pmem::pool_invalid_argument &e) {
            openFailed = true;
        }

        if (openFailed) {
            pmpool = createOrFail(filePath, size, layout);
        }
        root_oid = static_cast<pmem::obj::pool<Root>>(pmpool).root() -> ptr.raw_ptr();
	}

	pmem::obj::pool<Root> createOrFail(const char *path, const std::size_t size, const std::string &layout) {
		try {
			return pmem::obj::pool<Root>::create(path, layout, size, S_IRWXU);
		} catch (pmem::pool_invalid_argument &e) {
			throw std::invalid_argument(e.what());
		}
	}
};

PersistentMemoryPool* PersistentMemoryPool::instance = nullptr;

PersistentMemoryPool* PersistentMemoryPool::initialize(const char* filePath, uint64_t size = 8  * 1024 * 1024) {
    if(instance == nullptr) {
        instance = new PersistentMemoryPool(filePath, size);
    }
    return instance;
}

PersistentMemoryPool* PersistentMemoryPool::getInstance() { 
    if(instance == nullptr) {
        throw std::logic_error("Can not invoke getInstance without initializing PersistentMemoryPool");
    }
    return instance;
}

#endif