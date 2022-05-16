#include <string>
#include "Slice.h"

namespace pmem {
    namespace storage {

        std::string Slice::ToBuffer() const {
            return std::string(cdata(), size());
        }

        std::string Slice::ToDebugHexString() const {
            std::string result;
            char buf[10];
            for (auto i = begin_; i != end_; ++i) {
                snprintf(buf, sizeof(buf), "%02X", *i);
                result += buf;
            }
            return result;
        }

        int Slice::compare(const Slice& b) const {
            auto my_size = size();
            auto b_size = b.size();
            const size_t min_len = std::min(my_size, b_size);
            int r = Slice::MemCompare(begin_, b.begin_, min_len);
            if (r == 0) {
                if (my_size < b_size) { return -1; }
                if (my_size > b_size) { return 1; }
            }
            return r;
        }

        bool operator==(const Slice& x, const Slice& y) {
            return ((x.size() == y.size()) &&
                    (Slice::MemEqual(x.data(), y.data(), x.size())));
        }
    }
};