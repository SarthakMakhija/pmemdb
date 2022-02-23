#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipListNodeLevelGenerator.h"

TEST(SkipListNodeLevelGenerator, GenerateALevelBetween1AndMaxLevel) {
    int level = SkipListNodeLevelGenerator::initialize(10, 0.75) -> getInstance() -> generateLevel();
    ASSERT_TRUE(level >= 1);
    ASSERT_TRUE(level < 10);
}