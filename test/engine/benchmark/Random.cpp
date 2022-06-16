#include "Random.h"

std::string Random::HumanReadableString(uint64_t len) {
    std::string ret;
    ret.resize(len);
    for (uint64_t i = 0; i < len; ++i) {
        ret[i] = static_cast<char>('a' + Uniform(26));
    }
    return ret;
}