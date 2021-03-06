#ifndef _PersistentMemoryPoolFixture_
#define _PersistentMemoryPoolFixture_

#include <gtest/gtest.h>
#include <string>
#include <cstdio>

#include "../../../src/engine/storage/PersistentMemoryPool.h"

using namespace pmem::storage::internal;

class PersistentMemoryPoolFixture : public ::testing::Test {

    private:
    PersistentMemoryPool* pool;
    const char* filePath = "./tests_persistent_memory_pool_fixture.log";

    public:
    PersistentMemoryPoolFixture() {
        pool = nullptr;
    }
    void SetUp() {
        pool = PersistentMemoryPool::initialize(filePath, 8 * 1024 * 1024);
    }

    void TearDown() {
        remove(filePath);
        delete pool;
    }

    PersistentMemoryPool* getPersistentMemoryPool() {
        return this->pool;
    }
};

#endif