#include <benchmark/benchmark.h>
#include <string>
#include <random>
#include "../../../src/engine/Db.h"

using namespace pmem::storage;

std::string random_string() {
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(str.begin(), str.end(), generator);
    return str.substr(0, 32);
}

class DbInitFixture : public benchmark::Fixture {
private:
    pmem::storage::Db *db;
    const char *filePath = "./benchmark.log";
public:
    void SetUp(const ::benchmark::State &state) {
        db = pmem::storage::Db::open(
                pmem::storage::Configuration(filePath,
                                             32 * 1024 * 1024,
                                             1000,
                                             new pmem::storage::StringKeyComparator()));
    }

    void TearDown(const ::benchmark::State &state) {
        db->close();
        remove(filePath);
    }

    Db *getDb() {
        return db;
    }
};


BENCHMARK_F(DbInitFixture, DbPut)(benchmark::State& state) {
    Db* db = DbInitFixture::getDb();
    int count = 0;
    for (auto _ : state) {

        state.PauseTiming();
        auto randomKey   = new std::string(random_string());
        auto randomValue = new std::string(random_string());
        const char* key   = randomKey->c_str();
        const char* value = randomValue->c_str();
        KeyValueSize keyValueSize = KeyValueSize(strlen(key) + 1, strlen(value) + 1);
        state.ResumeTiming();

        Status status = db->put(key, value, keyValueSize);
        benchmark::DoNotOptimize(status);
    }
}

BENCHMARK_REGISTER_F(DbInitFixture, DbPut)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();