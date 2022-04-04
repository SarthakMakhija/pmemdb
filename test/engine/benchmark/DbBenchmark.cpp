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
    return Db::open(Configuration(filePath, 32 * 1024 * 1024, 1000, new StringKeyComparator()));
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
        for (int64_t per_key_size: {32, 32}) {
            b->Args({max_data, per_key_size});
        }
    }
    b->ArgNames({"max_data", "per_key_size"});
}


BENCHMARK(DbPut)->Apply(DBPutArguments);

BENCHMARK_MAIN();

