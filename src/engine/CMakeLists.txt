find_path(LIBPMEMOBJ_INCLUDE_DIR libpmemobj.h)
find_path(LIBPMEM_INCLUDE_DIR libpmem.h)

find_library(LIBPMEM_LIBRARY NAMES pmem libpmem)
find_library(LIBPMEMOBJ_LIBRARY NAMES pmemobj libpmemobj)

add_library(SkipList
        db/KeyValuePair.h
        db/KeyValuePair.cpp
        db/SkipListArena.h
        db/SkipListArena.cpp
        db/Status.h
        db/Db.h
        db/Db.cpp
        storage/SkipListNode.h
        storage/SkipList.h
        storage/SkipList.cpp
        storage/SkipListInternalNode.h
        storage/SkipListInternalNode.cpp
        storage/SkipListLeafNode.h
        storage/SkipListLeafNode.cpp
        storage/PersistentMemoryPool.h
        storage/PersistentMemoryPool.cpp
        storage/PersistentLeaf.h
        storage/WritePosition.h
        storage/utils/LevelGenerator.h
        storage/utils/LevelGenerator.cpp
        storage/utils/Fastmem.h
        storage/utils/IntegralTypes.h
        storage/Slice.h
        storage/Slice.cpp
        configuration/Configuration.h
        configuration/Configuration.cpp
        comparator/KeyComparator.h
        comparator/StringKeyComparator.h
        comparator/StringKeyComparator.cpp
        iterator/SkipListIterator.h
        iterator/SkipListIterator.cpp)

target_include_directories(SkipList PRIVATE ${LIBPMEMOBJ_INCLUDE_DIR} ${LIBPMEM_INCLUDE_DIR})
target_link_libraries(SkipList PRIVATE ${LIBPMEM_LIBRARY} ${LIBPMEMOBJ_LIBRARY})

install(TARGETS SkipList DESTINATION /usr/local/lib)