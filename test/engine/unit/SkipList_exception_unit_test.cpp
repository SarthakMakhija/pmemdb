#include <gtest/gtest.h>
#include "../../../src/engine/SkipList.h"
#include "../integration/PersistentMemoryPoolFixture.h"

using namespace pmem::storage;

TEST(SkipListException, SkipListException_ThrowInvalidArgumentExceptionGivenTowerSizeIsZero) {
    ASSERT_THROW(new SkipList(0), std::invalid_argument);
}

TEST(SkipListException, SkipListException_ThrowInvalidArgumentExceptionGivenTowerSizeIsLessThanZero) {
    ASSERT_THROW(new SkipList(-1), std::invalid_argument);
}