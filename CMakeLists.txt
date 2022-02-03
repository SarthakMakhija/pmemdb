cmake_minimum_required(VERSION 3.14)

set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CMAKE_SYSTEM_NAME Linux)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(pmemdb)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include engine and dependencies
# add_subdirectory(dependencies)
add_subdirectory(src/engine)

# Enable testing
add_executable(pmemdb_tests "")
target_sources(pmemdb_tests
	PRIVATE	
	test/engine/SkipList_integration_test.cpp
	test/engine/SkipList_exception_unit_test.cpp
	test/engine/SkipList_unit_test.cpp
	test/engine/SkipListInternalNode_unit_test.cpp
	test/engine/SkipListIterator_unit_test.cpp
	test/engine/SkipListLeafNode_unit_test.cpp
	test/engine/SkipListNodes_unit_test.cpp
	test/engine/SkipListNodeTestUtils.h
	test/engine/SkipListNodeTestUtils.cpp
	test/engine/PersistentMemoryPoolFixture.h
)

find_package(GTest CONFIG REQUIRED)
# find_package(libpmemobj++ CONFIG REQUIRED)

# find_library(LIB_PMEM NAMES pmem libpmem HINTS ${PROJECT_BINARY_DIR})
# find_library(LIB_PMEM_OBJ NAMES pmemobj libpmemobj HINTS ${PROJECT_BINARY_DIR})
# find_library(LIB_PMEM_OBJ_CPP NAMES pmemobj-cpp libpmemobj-cpp HINTS ${PROJECT_BINARY_DIR})
# find_package(libpmemobj-cpp REQUIRED)

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
	${LIBPMEM_LIBRARY} ${LIBPMEMOBJ_LIBRARY}
)

add_test(
  	NAME pmemdb_tests
  	COMMAND $<TARGET_FILE:pmemdb_tests>
)