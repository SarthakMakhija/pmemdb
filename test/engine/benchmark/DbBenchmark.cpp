#include <benchmark/benchmark.h>
#include <string>
#include <random>
#include "../../../src/engine/db/Db.h"
#include "./Random.h"
#include "./KeyGenerator.h"

using namespace pmem::storage;

const char *dbFilePath = "./DB_Benchmark.log";

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
    return Db::open(Configuration(dbFilePath, 512 * 1024 * 1024, 1000, new UInt32KeyComparator()));
}

static void TeardownDB(Db *db) {
    db->close();
    remove(dbFilePath);
}

static void DbPut(benchmark::State &state) {
    uint64_t maximumNumberOfKeys = state.range(0);
    uint64_t perKeySize = state.range(1);

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
        char *value = (new std::string(rnd.HumanReadableString(static_cast<int>(perKeySize))))->data();
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
    for (int64_t maximumNumberOfKeys: {100l << 30}) {
        for (int64_t perKeySize: {256, 1024}) {
            b->Args({maximumNumberOfKeys, perKeySize});
        }
    }
    b->ArgNames({"maximumNumberOfKeys", "perKeySize"});
}

static void DBGet(benchmark::State &state) {
    uint64_t maximumNumberOfKeys = state.range(0);
    uint64_t perKeySize = state.range(1);
    bool negativeQuery = state.range(2);

    uint64_t numberOfKeys = maximumNumberOfKeys / perKeySize;

    auto rnd = Random(301 + state.thread_index());
    KeyGenerator kg(&rnd, numberOfKeys);

    static Db *db;
    if (state.thread_index() == 0) {
        db = SetupDB();
        for (uint64_t count = 0; count < numberOfKeys; count++) {
            Slice slice = kg.Next();
            char *key = slice.buff;
            char *value = (new std::string(rnd.HumanReadableString(static_cast<int>(perKeySize))))->data();
            KeyValueSize keyValueSize = KeyValueSize(slice.keySize, strlen(value) + 1);
            Status status = db->put(key, value, keyValueSize);

            if (status == Status::Failed) {
                state.SkipWithError("failed while loading db");
            }
        }
    }
    size_t notFound = 0;

    for (auto _: state) {
        std::pair<const char *, bool> pair = db->get(kg.Next().buff);
        if (!pair.second) {
            notFound++;
        }
    }

    if (state.thread_index() == 0) {
        TeardownDB(db);
    }
}

static void DBGetArguments(benchmark::internal::Benchmark *b) {
    for (int64_t maximumNumberOfKeys: {512l << 20}) {
        for (int64_t perKeySize: {1024}) {
            for (bool negativeQuery: {false}) {
                b->Args({maximumNumberOfKeys, perKeySize, negativeQuery});
            }
        }
    }
    b->ArgNames({"maximumNumberOfKeys", "perKeySize", "negativeQuery"});
}

BENCHMARK(DbPut)->Apply(DBPutArguments);
//BENCHMARK(DBGet)->Apply(DBGetArguments);

BENCHMARK_MAIN();