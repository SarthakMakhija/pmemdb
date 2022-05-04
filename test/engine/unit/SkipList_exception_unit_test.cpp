#include <gtest/gtest.h>
#include "../../../src/engine/storage/SkipList.h"
#include "../../../src/engine/storage/utils/LevelGenerator.h"
#include "../../../src/engine/comparator/StringKeyComparator.h"
#include "../integration/PersistentMemoryPoolFixture.h"

using namespace pmem::storage;

TEST(SkipListException, SkipListException_ThrowInvalidArgumentExceptionGivenTowerSizeIsZero) {
    ASSERT_THROW(new SkipList(new LevelGenerator(0), new StringKeyComparator()), std::invalid_argument);
}

TEST(SkipListException, SkipListException_ThrowInvalidArgumentExceptionGivenTowerSizeIsLessThanZero) {
    ASSERT_THROW(new SkipList(new LevelGenerator(-1), new StringKeyComparator()), std::invalid_argument);
}