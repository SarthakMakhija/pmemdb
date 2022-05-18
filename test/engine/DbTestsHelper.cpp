#include "./DbTestsHelper.h"
#include "../../src/engine/storage/Slice.h"

void put(Db *db, const char *key, const char *value) {
    db->put(Slice(key), Slice(value));
}

void update(Db *db, const char *key, const char *value) {
    db->update(Slice(key), Slice(value));
}

void deleteBy(Db *db, const char *key) {
    db->deleteBy(Slice(key));
}