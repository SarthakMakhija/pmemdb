cmake_minimum_required(VERSION 3.14)

project(pmemdb)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include engine and dependencies
add_subdirectory(src/engine)

# Enable testing
add_executable(pmemdb_tests "")
target_sources(pmemdb_tests
        PRIVATE
        test/engine/unit/SkipList_exception_unit_test.cpp
        test/engine/unit/Db_exception_unit_test.cpp
        test/engine/integration/SkipListInternalNode_integration_test.cpp
        test/engine/integration/SkipListIterator_integration_test.cpp
        test/engine/integration/SkipListLeafNode_integration_test.cpp
        test/engine/integration/SkipListLeafNode_transactional_write_integration_test.cpp
        test/engine/integration/SkipListIterator_atomic_write_integration_test.cpp
        test/engine/integration/Db_concurrent_integration_test.cpp
        test/engine/integration/Db_integration_test.cpp
        test/engine/integration/Db_serialized_key_integration_test.cpp
        test/engine/functional/Db_functional_test.cpp
        test/engine/SkipListNodeTestUtils.h
        test/engine/SkipListNodeTestUtils.cpp
        test/engine/integration/PersistentMemoryPoolFixture.h
        test/engine/DbFixture.h
        test/engine/DbTestsHelper.h
        test/engine/DbTestsHelper.cpp
)

find_package(GTest CONFIG REQUIRED)

find_path(LIBPMEMOBJ_INCLUDE_DIR libpmemobj.h)
find_path(LIBPMEM_INCLUDE_DIR libpmem.h)

find_library(LIBPMEM_LIBRARY NAMES pmem libpmem)
find_library(LIBPMEMOBJ_LIBRARY NAMES pmemobj libpmemobj)

enable_testing()

target_include_directories(pmemdb_tests PRIVATE ${LIBPMEMOBJ_INCLUDE_DIR} ${LIBPMEM_INCLUDE_DIR})
target_link_libraries(pmemdb_tests
        PRIVATE
        SkipList
        GTest::gtest
        GTest::gtest_main
        ${LIBPMEM_LIBRARY} ${LIBPMEMOBJ_LIBRARY})

add_test(
        NAME pmemdb_tests
        COMMAND $<TARGET_FILE:pmemdb_tests>
)