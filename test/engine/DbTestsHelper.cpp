#include "./DbTestsHelper.h"
#include "../../src/engine/storage/Slice.h"

void put(Db *db, const char *key, const char *value) {
    db->put(Slice(key, strlen(key) + 1), Slice(value, strlen(value) + 1));
}

void update(Db *db, const char *key, const char *value) {
    db->update(Slice(key, strlen(key) + 1), Slice(value, strlen(value) + 1));
}

void deleteBy(Db *db, const char *key) {
    db->deleteBy(Slice(key, strlen(key) + 1));
}