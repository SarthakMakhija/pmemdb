#ifndef _DbTestsHelper_
#define _DbTestsHelper_

#include "../../src/engine/db/Db.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

void put(Db *db, const char* key, const char* value);

void update(Db *db, const char* key, const char* value);

void deleteBy(Db *db, const char* key);
#endif