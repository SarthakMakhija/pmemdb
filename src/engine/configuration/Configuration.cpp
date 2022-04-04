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

        const char *Configuration::getFilePath() const {
            return filePath;
        }

        uint64_t Configuration::getFileSize() const {
            return fileSize;
        }

        int Configuration::getSkipListTowerSize() const {
            return skipListTowerSize;
        }

        KeyComparator* Configuration::getKeyComparator() const {
            return keyComparator;
        }
    }
}