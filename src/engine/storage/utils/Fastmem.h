#ifndef _Fast_Mem_
#define _Fast_Mem_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "IntegralTypes.h"

#define UNALIGNED_LOAD32(_p) (*reinterpret_cast<const uint32 *>(_p))
#define UNALIGNED_LOAD64(_p) (*reinterpret_cast<const uint64 *>(_p))

//
// GCC can be told that a certain branch is not likely to be taken (for
// instance, a CHECK failure), and use that information in static analysis.
// Giving it this information can help it optimize for the common case in
// the absence of better information (ie. -fprofile-arcs).
//
#if defined(__GNUC__)
#define PREDICT_FALSE(x) (__builtin_expect(x, 0))
#else
#define PREDICT_FALSE(x) x
#endif

namespace pmem {
    namespace storage {

        inline int fastmemcmp_inlined(const void *a_void, const void *b_void, size_t n) {
            const uint8_t *a = reinterpret_cast<const uint8_t *>(a_void);
            const uint8_t *b = reinterpret_cast<const uint8_t *>(b_void);

            if (n >= 64) {
                return memcmp(a, b, n);
            }
            const void *a_limit = a + n;
            const size_t sizeof_uint64 = sizeof(uint64);
            while (a + sizeof_uint64 <= a_limit &&
                   UNALIGNED_LOAD64(a) == UNALIGNED_LOAD64(b)) {
                a += sizeof_uint64;
                b += sizeof_uint64;
            }
            const size_t sizeof_uint32 = sizeof(uint32);
            if (a + sizeof_uint32 <= a_limit &&
                UNALIGNED_LOAD32(a) == UNALIGNED_LOAD32(b)) {
                a += sizeof_uint32;
                b += sizeof_uint32;
            }
            while (a < a_limit) {
                int d = static_cast<uint32>(*a++) - static_cast<uint32>(*b++);
                if (d) return d;
            }
            return 0;
        }

        inline bool memeq(const void* a_v, const void* b_v, size_t n) {
            const uint8_t *a = reinterpret_cast<const uint8_t *>(a_v);
            const uint8_t *b = reinterpret_cast<const uint8_t *>(b_v);

            size_t n_rounded_down = n & ~static_cast<size_t>(7);
            if (PREDICT_FALSE(n_rounded_down == 0)) {  // n <= 7
                return memcmp(a, b, n) == 0;
            }
            // n >= 8
            uint64 u = UNALIGNED_LOAD64(a) ^ UNALIGNED_LOAD64(b);
            uint64 v = UNALIGNED_LOAD64(a + n - 8) ^ UNALIGNED_LOAD64(b + n - 8);
            if ((u | v) != 0) {  // The first or last 8 bytes differ.
                return false;
            }
            a += 8;
            b += 8;
            n = n_rounded_down - 8;
            if (n > 128) {
                // As of 2012, memcmp on x86-64 uses a big unrolled loop with SSE2
                // instructions, and while we could try to do something faster, it
                // doesn't seem worth pursuing.
                return memcmp(a, b, n) == 0;
            }
            for (; n >= 16; n -= 16) {
                uint64 x = UNALIGNED_LOAD64(a) ^ UNALIGNED_LOAD64(b);
                uint64 y = UNALIGNED_LOAD64(a + 8) ^ UNALIGNED_LOAD64(b + 8);
                if ((x | y) != 0) {
                return false;
                }
                a += 16;
                b += 16;
            }
            // n must be 0 or 8 now because it was a multiple of 8 at the top of the loop.
            return n == 0 || UNALIGNED_LOAD64(a) == UNALIGNED_LOAD64(b);
        }
    }
};

#endif