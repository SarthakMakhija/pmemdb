#include "./SkipListNodeLevelGenerator.h"

SkipListNodeLevelGenerator* SkipListNodeLevelGenerator::instance = nullptr;

SkipListNodeLevelGenerator* SkipListNodeLevelGenerator::initialize(int maxLevel, double probability) {
    if(instance == nullptr) {
        instance = new SkipListNodeLevelGenerator(maxLevel, probability);
    }
    return instance;
}

SkipListNodeLevelGenerator* SkipListNodeLevelGenerator::getInstance() { 
    if(instance == nullptr) {
        throw std::logic_error("Can not invoke getInstance without initializing SkipListNodeLevelGenerator");
    }
    return instance;
}

int SkipListNodeLevelGenerator::generateLevel() { 
    double random = (double)rand()/RAND_MAX;
    int level = 1;

    while (random < this -> probability && level < maxLevel) {
        level = level + 1;
        random = (double)rand()/RAND_MAX;
    }
    return level;
}