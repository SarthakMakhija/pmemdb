#include <benchmark/benchmark.h>
#include <string>
#include <random>
#include "../../../src/engine/db/Db.h"
#include "./Random.h"
#include "./KeyGenerator.h"

using namespace pmem::storage;

const char *filePath = "./benchmark.log";

class UInt32KeyComparator : public KeyComparator {
    int compare(char const *a, char const *b) const {
        uint32_t *first = (uint32_t *) a;
        uint32_t *second = (uint32_t *) b;
        if (*first == *second) {
            return 0;
        }
        if (*first < *second) {
            return -1;
        }
        return 1;
    }
};

static Db *SetupDB() {
    return Db::open(Configuration(filePath, 512 * 1024 * 1024, 1000, new UInt32KeyComparator()));
}

static void TeardownDB(Db *db) {
    db->close();
    remove(filePath);
}

static void DbPut(benchmark::State &state) {

    uint64_t max_data = state.range(0);
    uint64_t per_key_size = state.range(1);

    auto rnd = Random(301 + state.thread_index());
    KeyGenerator kg(&rnd);

    static Db *db = nullptr;
    if (state.thread_index() == 0) {
        db = SetupDB();
    }

    for (auto _: state) {
        state.PauseTiming();
        Slice slice = kg.Next();
        char *key = slice.buff;
        char *value = (new std::string(rnd.HumanReadableString(static_cast<int>(per_key_size))))->data();
        KeyValueSize keyValueSize = KeyValueSize(slice.keySize, strlen(value) + 1);
        state.ResumeTiming();

        Status status = db->put(key, value, keyValueSize);
        if (status == Status::Failed) {
            state.SkipWithError("Returned failed as the status");
        }
    }

    if (state.thread_index() == 0) {
        TeardownDB(db);
    }
}

static void DBPutArguments(benchmark::internal::Benchmark *b) {
    for (int64_t max_data: {100l << 30}) {
        for (int64_t per_key_size: {256, 1024}) {
            b->Args({max_data, per_key_size});
        }
    }
    b->ArgNames({"max_data", "per_key_size"});
}

static void DBGet(benchmark::State &state) {
    uint64_t max_data = state.range(0);
    uint64_t per_key_size = state.range(1);
    bool negative_query = state.range(2);
    uint64_t key_num = max_data / per_key_size;

    auto rnd = Random(301 + state.thread_index());
    KeyGenerator kg(&rnd, key_num);

    static Db *db;
    if (state.thread_index() == 0) {
        db = SetupDB();
        for (uint64_t i = 0; i < key_num; i++) {
            Slice slice = kg.Next();
            char *key = slice.buff;
            char *value = (new std::string(rnd.HumanReadableString(static_cast<int>(per_key_size))))->data();
            KeyValueSize keyValueSize = KeyValueSize(slice.keySize, strlen(value) + 1);
            Status s = db->put(key, value, keyValueSize);

            if (s == Status::Failed) {
                state.SkipWithError("failed while loading db");
            }
        }
    }
    size_t not_found = 0;

    for (auto _: state) {
        std::string val;
        std::pair<const char *, bool> pair = db->get(kg.Next().buff);
        if (!pair.second) {
            not_found++;
        }
    }

    if (state.thread_index() == 0) {
        TeardownDB(db);
    }
}

static void DBGetArguments(benchmark::internal::Benchmark *b) {
    for (int64_t max_data: {512l << 20}) {
        for (int64_t per_key_size: {1024}) {
            for (bool negative_query: {false}) {
                b->Args({max_data, per_key_size, negative_query});
            }
        }
    }
    b->ArgNames({"max_data", "per_key_size", "negative_query"});
}

BENCHMARK(DbPut)->Apply(DBPutArguments);
BENCHMARK(DBGet)->Apply(DBGetArguments);

BENCHMARK_MAIN();