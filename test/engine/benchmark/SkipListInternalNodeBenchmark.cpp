#include <benchmark/benchmark.h>
#include <string>
#include <random>
#include "../SkipListNodeTestUtils.h"
#include "./Random.h"
#include "./KeyGenerator.h"

#include "../../../src/engine/storage/PersistentMemoryPool.h"

using namespace pmem::storage;
using namespace pmem::storage::internal;

const char *skipListInternalNodeFilePath = "./SkipListInternalNode_benchmark.log";

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

static std::pair<SkipListInternalNode *, PersistentMemoryPool *> SetUpSkipListInternalNode() {
    auto pool = PersistentMemoryPool::initialize(skipListInternalNodeFilePath, 512 * 1024 * 1024);
    return std::pair(newSentinelInternalNode(1000), pool);
}

static void TearDownSkipListInternalNode(PersistentMemoryPool *pool) {
    delete pool;
    remove(skipListInternalNodeFilePath);
}

static void SkipListInternalNodePutPosition(benchmark::State &state) {
    uint64_t maxKey = state.range(0);
    uint64_t perKeySize = state.range(1);
    KeyComparator *keyComparator = new UInt32KeyComparator();

    auto rnd = Random(301 + state.thread_index());
    KeyGenerator kg(&rnd);

    static SkipListInternalNode *skipListInternalNode = nullptr;
    static PersistentMemoryPool *pool = nullptr;
    if (state.thread_index() == 0) {
        auto nodePoolPair = SetUpSkipListInternalNode();
        skipListInternalNode = nodePoolPair.first;
        pool = nodePoolPair.second;
    }

    for (auto _: state) {
        state.PauseTiming();
        Slice slice = kg.Next();
        char *key = slice.buff;
        state.ResumeTiming();

        PutPosition putPosition = skipListInternalNode->putPositionOf(key, keyComparator);
        if (putPosition.newLevel == -1) {
            state.SkipWithError("Returned -1 as the new level");
        }
    }

    if (state.thread_index() == 0) {
        TearDownSkipListInternalNode(pool);
    }
}

static void SkipListInternalNodePutPositionArguments(benchmark::internal::Benchmark *b) {
    for (int64_t maxKey: {100l << 30}) {
        for (int64_t perKeySize: {256, 1024}) {
            b->Args({maxKey, perKeySize});
        }
    }
    b->ArgNames({"maxKey", "perKeySize"});
}

static void SkipListInternalNodePut(benchmark::State &state) {
    uint64_t maxKey = state.range(0);
    uint64_t perKeySize = state.range(1);
    KeyComparator *keyComparator = new UInt32KeyComparator();

    auto rnd = Random(301 + state.thread_index());
    KeyGenerator kg(&rnd);

    static SkipListInternalNode *skipListInternalNode = nullptr;
    static PersistentMemoryPool *pool = nullptr;
    if (state.thread_index() == 0) {
        auto nodePoolPair = SetUpSkipListInternalNode();
        skipListInternalNode = nodePoolPair.first;
        pool = nodePoolPair.second;
    }

    for (auto _: state) {
        state.PauseTiming();
        Slice slice = kg.Next();
        char *key = slice.buff;
        state.ResumeTiming();

        PutPosition putPosition = skipListInternalNode->putPositionOf(key, keyComparator);
        if (putPosition.newLevel != -1) {
            SkipListNode *newInternal = skipListInternalNode->put(key, putPosition.positions, putPosition.newLevel);
            if (newInternal == nullptr) {
                state.SkipWithError("Returned nullptr as the new node");
            }
        }
    }

    if (state.thread_index() == 0) {
        TearDownSkipListInternalNode(pool);
    }
}

static void SkipListInternalNodePutArguments(benchmark::internal::Benchmark *b) {
    for (int64_t maxKey: {100l << 30}) {
        for (int64_t perKeySize: {256, 1024}) {
            b->Args({maxKey, perKeySize});
        }
    }
    b->ArgNames({"maxKey", "perKeySize"});
}

static void SkipListInternalNodeGet(benchmark::State &state) {
    uint64_t maxKey = state.range(0);
    uint64_t perKeySize = state.range(1);
    bool negativeQuery = state.range(2);

    uint64_t numberOfKeys = maxKey / perKeySize;

    auto rnd = Random(301 + state.thread_index());
    KeyGenerator kg(&rnd, numberOfKeys);
    KeyComparator *keyComparator = new UInt32KeyComparator();

    static SkipListInternalNode *skipListInternalNode = nullptr;
    static PersistentMemoryPool *pool = nullptr;

    if (state.thread_index() == 0) {
        auto nodePoolPair = SetUpSkipListInternalNode();
        skipListInternalNode = nodePoolPair.first;
        pool = nodePoolPair.second;

        for (uint64_t count = 0; count < numberOfKeys; count++) {
            Slice slice = kg.Next();
            char *key = slice.buff;

            PutPosition putPosition = skipListInternalNode->putPositionOf(key, keyComparator);
            if (putPosition.newLevel != -1) {
                SkipListNode *newInternal = skipListInternalNode->put(key, putPosition.positions, putPosition.newLevel);
                if (newInternal == nullptr) {
                    state.SkipWithError("Returned nullptr as the new node");
                }
            }
        }
    }

    size_t notFound = 0;
    for (auto _: state) {
        state.PauseTiming();
        char *key = kg.Next().buff;
        state.ResumeTiming();
        std::pair<SkipListNode*, bool> pair = skipListInternalNode->getBy(key, keyComparator);
        if (!pair.second) {
            notFound++;
        }
    }
    if (state.thread_index() == 0) {
        TearDownSkipListInternalNode(pool);
    }
}

static void SkipListInternalNodeGetArguments(benchmark::internal::Benchmark *b) {
    for (int64_t maxKey: {512l << 20}) {
        for (int64_t perKeySize: {1024}) {
            for (bool negativeQuery: {false}) {
                b->Args({maxKey, perKeySize, negativeQuery});
            }
        }
    }
    b->ArgNames({"maxKey", "perKeySize", "negativeQuery"});
}


static void SkipListInternalNodeScan(benchmark::State &state) {
    uint64_t maxKey = state.range(0);
    uint64_t perKeySize = state.range(1);
    bool negativeQuery = state.range(2);

    uint64_t numberOfKeys = maxKey / perKeySize;

    auto rnd = Random(301 + state.thread_index());
    KeyGenerator kg(&rnd, numberOfKeys);
    KeyComparator *keyComparator = new UInt32KeyComparator();

    static SkipListInternalNode *skipListInternalNode = nullptr;
    static PersistentMemoryPool *pool = nullptr;

    if (state.thread_index() == 0) {
        auto nodePoolPair = SetUpSkipListInternalNode();
        skipListInternalNode = nodePoolPair.first;
        pool = nodePoolPair.second;

        for (uint64_t count = 0; count < numberOfKeys; count++) {
            Slice slice = kg.Next();
            char *key = slice.buff;

            PutPosition putPosition = skipListInternalNode->putPositionOf(key, keyComparator);
            if (putPosition.newLevel != -1) {
                SkipListNode *newInternal = skipListInternalNode->put(key, putPosition.positions, putPosition.newLevel);
                if (newInternal == nullptr) {
                    state.SkipWithError("Returned nullptr as the new node");
                }
            }
        }
    }

    size_t notFound = 0;
    for (auto _: state) {
        state.PauseTiming();
        char *beginKey = kg.Next().buff;
        state.ResumeTiming();
        std::pair<SkipListNode*, bool> pair = skipListInternalNode->scan(beginKey, keyComparator);
        if (!pair.second) {
            notFound++;
        }
    }
    if (state.thread_index() == 0) {
        TearDownSkipListInternalNode(pool);
    }
}

static void SkipListInternalNodeScanArguments(benchmark::internal::Benchmark *b) {
    for (int64_t maxKey: {512l << 20}) {
        for (int64_t perKeySize: {1024}) {
            for (bool negativeQuery: {false}) {
                b->Args({maxKey, perKeySize, negativeQuery});
            }
        }
    }
    b->ArgNames({"maxKey", "perKeySize", "negativeQuery"});
}


BENCHMARK(SkipListInternalNodePutPosition)->Apply(SkipListInternalNodePutPositionArguments);
BENCHMARK(SkipListInternalNodePut)->Apply(SkipListInternalNodePutArguments);
BENCHMARK(SkipListInternalNodeGet)->Apply(SkipListInternalNodeGetArguments);
BENCHMARK(SkipListInternalNodeScan)->Apply(SkipListInternalNodeScanArguments);