#include "Configuration.h"

namespace pmem {
    namespace storage {
        Configuration::Configuration(const char *filePath,
                                     uint64_t fileSize,
                                     int skipListTowerSize,
                                     double probability) : filePath{filePath},
                                                           fileSize{fileSize},
                                                           skipListTowerSize{skipListTowerSize},
                                                           probability{probability} {
        }

        const char *Configuration::getFilePath() {
            return filePath;
        }

        uint64_t Configuration::getFileSize() {
            return fileSize;
        }

        int Configuration::getSkipListTowerSize() {
            return skipListTowerSize;
        }

        double Configuration::getProbability() {
            return probability;
        }
    }
}