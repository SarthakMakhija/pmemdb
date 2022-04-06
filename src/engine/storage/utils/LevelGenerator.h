#ifndef _LevelGenerator_
#define _LevelGenerator_

namespace pmem {
    namespace storage {
        namespace internal {
            class LevelGenerator {
            private:
                int maxLevel;
                int skipFactor;
            public:
                LevelGenerator(int maxLevel);
                int generate();
                int getMaxLevel();
            };
        }
    }
}
#endif
