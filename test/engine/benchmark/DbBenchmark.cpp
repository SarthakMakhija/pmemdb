#include <benchmark/benchmark.h>
#include <string>
#include <random>
#include "../../../src/engine/db/Db.h"
#include "./Random.h"
#include "./KeyGenerator.h"

using namespace pmem::storage;

const char *dbFilePath = "./tests_db_benchmark.log";

class UInt32KeyComparator : public KeyComparator {
    int compare(const Slice& a, const Slice& b) const override {
        const uint32_t* first = reinterpret_cast<const uint32_t *>(a.cdata());
        const uint32_t* second = reinterpret_cast<const uint32_t *>(b.cdata());

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
    return Db::open(Configuration(dbFilePath, 512 * 1024 * 1024, 18, new UInt32KeyComparator()));
}

static void TeardownDB(Db *db) {
    db->close();
    remove(dbFilePath);
}

static void DbPut(benchmark::State &state) {
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
        char *value = (new std::string(rnd.HumanReadableString(static_cast<int>(perKeySize))))->data();
        state.ResumeTiming();

        Status status = db->put(slice, Slice(value, strlen(value) + 1));
        if (status == Status::Failed) {
            state.SkipWithError("Returned failed as the status");
        }
    }

    if (state.thread_index() == 0) {
        TeardownDB(db);
    }
}

static void DBPutArguments(benchmark::internal::Benchmark *b) {
    for (int64_t maxKey: {100l << 30}) {
        for (int64_t perKeySize: {256, 1024}) {
            b->Args({maxKey, perKeySize});
        }
    }
    b->ArgNames({"maxKey", "perKeySize"});
}

static void DBGet(benchmark::State &state) {
    uint64_t maxKey = state.range(0);
    uint64_t perKeySize = state.range(1);

    uint64_t numberOfKeys = maxKey / perKeySize;

    auto rnd = Random(301 + state.thread_index());
    KeyGenerator kg(&rnd, numberOfKeys);

    static Db *db;
    if (state.thread_index() == 0) {
        db = SetupDB();
        for (uint64_t count = 0; count < numberOfKeys; count++) {
            pmem::storage::Slice slice = kg.Next();
            char *value = (new std::string(rnd.HumanReadableString(static_cast<int>(perKeySize))))->data();
            Status status = db->put(slice, Slice(value, strlen(value) + 1));

            if (status == Status::Failed) {
                state.SkipWithError("failed while loading db in get");
            }
        }
    }
    size_t notFound = 0;

    for (auto _: state) {
        state.PauseTiming();
        const char *key = kg.Next().cdata();
        state.ResumeTiming();
        std::pair<Slice, bool> pair = db->get(key);
        if (!pair.second) {
            notFound++;
        }
    }

    if (state.thread_index() == 0) {
        TeardownDB(db);
    }
}

static void DBGetArguments(benchmark::internal::Benchmark *b) {
    for (int64_t maxKey: {512l << 20}) {
        for (int64_t perKeySize: {1024}) {
            for (bool negativeQuery: {false}) {
                b->Args({maxKey, perKeySize, negativeQuery});
            }
        }
    }
    b->ArgNames({"maxKey", "perKeySize", "negativeQuery"});
}


static void DBScan(benchmark::State &state) {
    uint64_t maxKey = state.range(0);
    uint64_t perKeySize = state.range(1);
    int64_t maxPairs = state.range(2);

    uint64_t numberOfKeys = maxKey / perKeySize;

    auto rnd = Random(301 + state.thread_index());
    KeyGenerator kg(&rnd, numberOfKeys);

    static Db *db;
    if (state.thread_index() == 0) {
        db = SetupDB();
        for (uint64_t count = 0; count < numberOfKeys; count++) {
            pmem::storage::Slice slice = kg.Next();
            char *value = (new std::string(rnd.HumanReadableString(static_cast<int>(perKeySize))))->data();
            Status status = db->put(slice, Slice(value, strlen(value) + 1));

            if (status == Status::Failed) {
                state.SkipWithError("failed while loading db in scan");
            }
        }
    }
    size_t notFound = 0;

    for (auto _: state) {
        state.PauseTiming();
        Slice beginKey = kg.Next();
        Slice endKey = kg.Next();

        std::vector <KeyValuePair> result;
        if (*(uint32_t *) beginKey.cdata() > *(uint32_t *) endKey.cdata()) {
            state.ResumeTiming();
            result = db->scan(endKey, beginKey, maxPairs);
        } else {
            state.ResumeTiming();
            result = db->scan(beginKey, endKey, maxPairs);
        }
        if (result.empty()) {
            notFound++;
        }
    }

    if (state.thread_index() == 0) {
        TeardownDB(db);
    }
}

static void DBScanArguments(benchmark::internal::Benchmark *b) {
    for (int64_t maxKey: {512l << 20}) {
        for (int64_t perKeySize: {1024}) {
            for (int64_t maxPairs : {1000}) {
                b->Args({maxKey, perKeySize, maxPairs});
            }
        }
    }
    b->ArgNames({"maxKey", "perKeySize", "maxPairs"});
}

BENCHMARK(DbPut)->Apply(DBPutArguments);
BENCHMARK(DBGet)->Apply(DBGetArguments);
BENCHMARK(DBScan)->Apply(DBScanArguments);

BENCHMARK_MAIN();