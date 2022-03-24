#ifndef _Configuration_
#define _Configuration_

#include <stdint.h>
namespace pmem {
    namespace storage {
        class Configuration {

        private:
            const char *filePath;
            uint64_t fileSize;
            int skipListTowerSize;
            double probability;

        public:
            Configuration(const char *filePath,
                          uint64_t fileSize,
                          int skipListTowerSize,
                          double probability);

            const char *getFilePath();

            uint64_t getFileSize();

            int getSkipListTowerSize();

            double getProbability();
        };
    }
}

#endif