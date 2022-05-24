#ifndef _PersistentMemoryPool_
#define _PersistentMemoryPool_

#include <libpmemobj++/pool.hpp>
#include <string>
#include <filesystem>
#include "PersistentLeaf.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class PersistentMemoryPool {

            public:
                PersistentMemoryPool(PersistentMemoryPool &other) = delete;

                void operator=(const PersistentMemoryPool &) = delete;

                static PersistentMemoryPool *initialize(const char *filePath, uint64_t size);

                pmem::obj::pool_base getPmpool() const;

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

                explicit PersistentMemoryPool(const char *filePath, uint64_t size = 8 * 1024 * 1024) {
                    std::string pathAsString  = std::string(filePath);
                    size_t lastIndex          = pathAsString.find_last_of("/");
                    std::string layout        = pathAsString.substr(lastIndex + 1);
                    std::string directoryPath = pathAsString.substr(0, lastIndex);

                    std::filesystem::file_status status;
                    if (!(std::filesystem::status_known(status) ? std::filesystem::exists(status) : std::filesystem::exists(directoryPath))) {
                        std::filesystem::create_directories(directoryPath);
                    }
                    
                    bool openFailed = false;
                    try {
                        pmpool = pmem::obj::pool<Root>::open(filePath, layout);
                    } catch (pmem::pool_invalid_argument &e) {
                        openFailed = true;
                    }

                    if (openFailed) {
                        pmpool = createOrFail(filePath, size, layout);
                    }
                    root_oid = static_cast<pmem::obj::pool<Root>>(pmpool).root()->ptr.raw_ptr();
                }

                pmem::obj::pool<Root>
                static createOrFail(const char *path, const std::size_t size, const std::string &layout) {
                    try {
                        return pmem::obj::pool<Root>::create(path, layout, size);
                    } catch (pmem::pool_invalid_argument &e) {
                        throw std::invalid_argument(e.what());
                    }
                }
            };
        }
    }
}

#endif