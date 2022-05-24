#ifndef _DbFixture_
#define _DbFixture_

#include <gtest/gtest.h>
#include  <string>
#include <filesystem>

#include "../../src/engine/db/Db.h"
#include "../../src/engine/comparator/StringKeyComparator.h"

using namespace pmem::storage::internal;

class DbFixture : public ::testing::Test {

private:
    pmem::storage::Db *db;
    const char *filePath = "./db/tests_db_fixture.log";

public:
    DbFixture() {
        db = nullptr;
    }
    void SetUp() {
        db = pmem::storage::Db::open(
                pmem::storage::Configuration(filePath,
                                             8 * 1024 * 1024,
                                             8,
                                             new pmem::storage::StringKeyComparator()));
    }

    void TearDown() {
        db->close();
        std::filesystem::remove(filePath);
    }

    pmem::storage::Db* getDb() {
        return db;
    }
};

#endif