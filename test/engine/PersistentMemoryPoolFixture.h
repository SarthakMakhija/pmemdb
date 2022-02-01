#ifndef _PersistentMemoryPoolFixture_
#define _PersistentMemoryPoolFixture_

#include <gtest/gtest.h>
#include  <cstdio>
#include  <string>

#include "../../src/engine/PersistentMemoryPool.h"

using namespace std;

class PersistentMemoryPoolFixture : public ::testing::Test {

    private:
    PersistentMemoryPool* pool;
    const char* filePath = "./tests.log";

    public:
    void SetUp() {
        pool = PersistentMemoryPool::initialize(filePath);
    }

    void TearDown() {
        remove(filePath);
        delete pool;
    }
};

#endif