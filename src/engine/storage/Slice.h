#ifndef _Slice_
#define _Slice_

#include <string>
#include <cstring>
#include "utils/Fastmem.h"

namespace pmem {
    namespace storage {

        class Slice {

        private:          
            friend bool operator==(const Slice& x, const Slice& y);

            inline const char* to_char_ptr(const uint8_t* uptr) const {
                return reinterpret_cast<const char *>(uptr);
            }

            inline const uint8_t* to_uchar_ptr(const char *ptr) {
                return reinterpret_cast<const uint8_t *>(ptr);
            }

            static int MemCompare(const void* a, const void* b, size_t n) {
                return fastmemcmp_inlined(a, b, n);
            }

            static bool MemEqual(const void* a, const void* b, size_t n) {
                return memeq(a, b, n);
            }

            const uint8_t *begin_;
            const uint8_t *end_;

        public:
            // Create an empty slice.
            Slice() : begin_(to_uchar_ptr("")), end_(begin_) {}

            // Create a slice that refers to d[0,n-1].
            Slice(const uint8_t *d, size_t n) : begin_(d), end_(d + n) {}

            // Create a slice that refers to d[0,n-1].
            Slice(const char *d, size_t n) : Slice(to_uchar_ptr(d), n) {}

            // Create a slice that refers to [begin, end).
            Slice(const uint8_t *begin, const uint8_t *end) : begin_(begin), end_(end) {}

            Slice(const char *begin, const char *end)
                    : Slice(to_uchar_ptr(begin), to_uchar_ptr(end)) {}

            // Create a slice that refers to s[0,strlen(s)]
            Slice(const char *s) // NOLINT(runtime/explicit)
                    : Slice(to_uchar_ptr(s), strlen(s)+1) {}

            const char *cdata() const { return to_char_ptr(begin_); }

            // Return a pointer to the beginning of the referenced data
            const uint8_t *data() const { return begin_; }

            const uint8_t *end() const { return end_; }

            // Return the length (in bytes) of the referenced data
            size_t size() const { return end_ - begin_; }

            // Return true iff the length of the referenced data is zero
            bool empty() const { return begin_ == end_; }

            // Return a string that contains the copy of the referenced data.
            std::string ToBuffer() const;

            std::string ToString() const __attribute__ ((deprecated)) { return ToBuffer(); }

            std::string ToDebugHexString() const;

            // Three-way comparison.  Returns value:
            //   <  0 iff "*this" <  "b",
            //   == 0 iff "*this" == "b",
            //   >  0 iff "*this" >  "b"
            int compare(const Slice &b) const;           
        };
    }
};

#endif