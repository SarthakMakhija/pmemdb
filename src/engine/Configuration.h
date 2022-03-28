#ifndef _Configuration_
#define _Configuration_

#include "StringKeyComparator.h"
#include <stdint.h>

namespace pmem {
    namespace storage {
        class Configuration {

        private:
            const char *filePath;
            uint64_t fileSize;
            int skipListTowerSize;
            double probability;
            KeyComparator *keyComparator;

        public:
            Configuration(const char *filePath,
                          uint64_t fileSize,
                          int skipListTowerSize,
                          double probability,
                          KeyComparator *keyComparator);

            const char *getFilePath();

            uint64_t getFileSize();

            int getSkipListTowerSize();

            double getProbability();

            KeyComparator *getKeyComparator();
        };
    }
}

#endif