cmake_minimum_required(VERSION 3.14)

project(collections)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fetch Google test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# include engine library
add_subdirectory(src/engine)

# Enable testing
add_executable(pmemdb_tests "")
target_sources(pmemdb_tests
	PRIVATE
	test/engine/SkipListNode_unit_test.cpp
)

target_link_libraries(pmemdb_tests
  	PRIVATE
    SkipList
    gtest_main
)

enable_testing()
add_test(
  	NAME pmemdb_tests
  	COMMAND $<TARGET_FILE:pmemdb_tests>
)