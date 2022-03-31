#ifndef _DbFixture_
#define _DbFixture_

#include <gtest/gtest.h>
#include  <cstdio>
#include  <string>

#include "../../src/engine/Db.h"
#include "../../src/engine/StringKeyComparator.h"

using namespace pmem::storage::internal;

class DbFixture : public ::testing::Test {

private:
    pmem::storage::Db *db;
    const char *filePath = "./tests.log";

public:
    void SetUp() {
        db = pmem::storage::Db::open(
                pmem::storage::Configuration(filePath,
                                             8 * 1024 * 1024,
                                             8,
                                             new pmem::storage::StringKeyComparator()));
    }

    void TearDown() {
        db->close();
        remove(filePath);
    }

    pmem::storage::Db* getDb() {
        return db;
    }
};

#endif