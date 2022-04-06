#include <random>
#include "LevelGenerator.h"

namespace pmem {
    namespace storage {
        namespace internal {
            LevelGenerator::LevelGenerator(int maxLevel) {
                this->maxLevel = maxLevel;
                this->skipFactor = 2;
                srand((unsigned) time(0));
            }

            int LevelGenerator::generate() {
                double random = (double) rand() / RAND_MAX;
                int level = 1;
                while (level <= maxLevel && random < (1.0) / this->skipFactor) {
                    level = level + 1;
                    random = (double) rand() / RAND_MAX;
                }
                return level;
            }

            int LevelGenerator::getMaxLevel() {
                return this->maxLevel;
            }
        }
    }
}