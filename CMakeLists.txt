cmake_minimum_required(VERSION 3.14)

project(pmemdb)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include engine and dependencies
add_subdirectory(dependencies)
add_subdirectory(src/engine)

# Enable testing
add_executable(pmemdb_tests "")
target_sources(pmemdb_tests
	PRIVATE
	test/engine/SkipListNodeTestUtils.h
	test/engine/SkipListNodeTestUtils.cpp
	test/engine/SkipListInternalNode_unit_test.cpp
	test/engine/SkipListLeafNode_unit_test.cpp
	test/engine/SkipListIterator_unit_test.cpp
	test/engine/SkipListNodes_unit_test.cpp
	test/engine/SkipList_unit_test.cpp
	test/engine/SkipList_exception_unit_test.cpp
	test/engine/SkipList_integration_test.cpp
	test/engine/PersistentMemoryPoolFixture.h
)

find_library(LIB_PMEM NAMES pmem libpmem)
find_library(LIB_PMEM_OBJ NAMES pmemobj libpmemobj)

target_link_libraries(pmemdb_tests
  	PRIVATE
    SkipList
    gtest_main
	${LIB_PMEM}
	${LIB_PMEM_OBJ}
)

enable_testing()
add_test(
  	NAME pmemdb_tests
  	COMMAND $<TARGET_FILE:pmemdb_tests>
)