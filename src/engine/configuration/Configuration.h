#ifndef _Configuration_
#define _Configuration_

#include "comparator/StringKeyComparator.h"
#include <stdint.h>

namespace pmem {
    namespace storage {
        class Configuration {

        private:
            const char *filePath;
            uint64_t fileSize;
            int skipListMaxLevel;
            KeyComparator *keyComparator;

        public:
            Configuration(const char *filePath,
                          uint64_t fileSize,
                          int skipListMaxLevel,
                          KeyComparator *keyComparator);

            const char *getFilePath() const;

            uint64_t getFileSize() const;

            int getSkipListMaxLevel() const;

            KeyComparator *getKeyComparator() const;
        };
    }
}

#endif