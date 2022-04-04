#ifndef _KeyGenerator_
#define _KeyGenerator_

#include "Random.h"

struct Slice {
    char *buff;
    size_t keySize;
};

class KeyGenerator {
private:
    uint32_t nextSequentialKey = 0;
    Random *rnd_;
    size_t keySize;
    uint64_t maxKey;
    bool isSequential = false;
    const int MULTIPLIER = 3;

public:
    explicit KeyGenerator(Random *rnd, uint64_t maxKey_ = 100 * 1024 * 1024, size_t keySize_ = 10) {
        keySize = keySize_;
        maxKey = maxKey_;
        rnd_ = rnd;
    }

    Slice Next(int8_t offset = 1) {
        assert(maxKey < std::numeric_limits<uint32_t>::max() / MULTIPLIER);
        char *buffer = new char[256];

        uint32_t k;
        if (isSequential) {
            assert(nextSequentialKey < maxKey);
            k = (nextSequentialKey % maxKey) * MULTIPLIER + offset;
            nextSequentialKey++;
        } else {
            k = (rnd_->Next() % maxKey) * MULTIPLIER + offset;
        }

        memset(buffer, 0, keySize);
        Encode(buffer, k);
        return Slice{buffer, keySize};
    }

    void static Encode(char *buf, uint32_t value) {
        memcpy(buf, &value, sizeof(value));
    }
};

#endif