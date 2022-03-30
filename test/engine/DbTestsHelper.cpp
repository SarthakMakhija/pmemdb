#include "./DbTestsHelper.h"

void put(Db *db, const char* key, const char* value) {
    db->put(key, value);
}

void update(Db *db, const char* key, const char* value) {
    db->update(key, value);
}

void deleteBy(Db *db, const char* key) {
    db->deleteBy(key);
}