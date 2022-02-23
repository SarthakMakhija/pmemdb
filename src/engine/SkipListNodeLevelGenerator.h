#ifndef _SkipListNodeLevelGenerator_
#define _SkipListNodeLevelGenerator_

#include <stdexcept>

class SkipListNodeLevelGenerator {

    public:
    SkipListNodeLevelGenerator(SkipListNodeLevelGenerator &other) = delete;

    void operator=(const SkipListNodeLevelGenerator &) = delete;

    static SkipListNodeLevelGenerator* initialize(int maxLevel, double probability);

    static SkipListNodeLevelGenerator* getInstance();

    int generateLevel();

	~SkipListNodeLevelGenerator() {
        instance = nullptr;
	}

    private:
	static SkipListNodeLevelGenerator* instance;
    int maxLevel;
    double probability;

	SkipListNodeLevelGenerator(int maxLevel_, double probability_) {
        maxLevel = maxLevel_;
        probability = probability_;
	}
};

#endif