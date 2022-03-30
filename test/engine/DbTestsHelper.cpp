#include "./DbTestsHelper.h"

void put(Db *db, const char *key, const char *value) {
    db->put(key, value, KeyValueSize(strlen(key) + 1, strlen(value) + 1));
}

void update(Db *db, const char *key, const char *value) {
    db->update(key, value, KeyValueSize(strlen(key) + 1, strlen(value) + 1));
}

void deleteBy(Db *db, const char *key) {
    db->deleteBy(key);
}