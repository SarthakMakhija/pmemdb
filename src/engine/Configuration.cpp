#include "Configuration.h"

namespace pmem {
    namespace storage {
        Configuration::Configuration(const char *filePath,
                                     uint64_t fileSize,
                                     int skipListTowerSize,
                                     KeyComparator* keyComparator) : filePath{filePath},
                                                                    fileSize{fileSize},
                                                                    skipListTowerSize{skipListTowerSize},
                                                                    keyComparator{keyComparator} {
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

        KeyComparator* Configuration::getKeyComparator() {
            return keyComparator;
        }
    }
}