#include <gtest/gtest.h>
#include "../../../src/engine/storage/SkipList.h"
#include "../../../src/engine/storage/utils/LevelGenerator.h"
#include "../integration/PersistentMemoryPoolFixture.h"

using namespace pmem::storage;

TEST(SkipListException, SkipListException_ThrowInvalidArgumentExceptionGivenTowerSizeIsZero) {
    ASSERT_THROW(new SkipList(new LevelGenerator(0)), std::invalid_argument);
}

TEST(SkipListException, SkipListException_ThrowInvalidArgumentExceptionGivenTowerSizeIsLessThanZero) {
    ASSERT_THROW(new SkipList(new LevelGenerator(-1)), std::invalid_argument);
}