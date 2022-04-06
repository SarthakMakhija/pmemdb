#include "Configuration.h"

namespace pmem {
    namespace storage {
        Configuration::Configuration(const char *filePath,
                                     uint64_t fileSize,
                                     int skipListMaxLevel,
                                     KeyComparator *keyComparator) : filePath{filePath},
                                                                     fileSize{fileSize},
                                                                     skipListMaxLevel{skipListMaxLevel},
                                                                     keyComparator{keyComparator} {
        }

        const char *Configuration::getFilePath() const {
            return filePath;
        }

        uint64_t Configuration::getFileSize() const {
            return fileSize;
        }

        int Configuration::getSkipListMaxLevel() const {
            return skipListMaxLevel;
        }

        KeyComparator *Configuration::getKeyComparator() const {
            return keyComparator;
        }
    }
}