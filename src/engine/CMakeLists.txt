find_path(LIBPMEMOBJ_INCLUDE_DIR libpmemobj.h)
find_path(LIBPMEM_INCLUDE_DIR libpmem.h)

find_library(LIBPMEM_LIBRARY NAMES pmem libpmem)
find_library(LIBPMEMOBJ_LIBRARY NAMES pmemobj libpmemobj)

add_library(SkipList
        SHARED
        db/KeyValuePair.h
        db/KeyValuePair.cpp
        db/SkipListIterator.h
        db/SkipListIterator.cpp
        db/Status.h
        db/Db.h
        db/Db.cpp
        db/KeyValueSize.h
        db/KeyValueSize.cpp
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
        configuration/Configuration.h
        configuration/Configuration.cpp
        comparator/KeyComparator.h
        comparator/StringKeyComparator.h
        comparator/StringKeyComparator.cpp)

target_include_directories(SkipList PRIVATE ${LIBPMEMOBJ_INCLUDE_DIR} ${LIBPMEM_INCLUDE_DIR})
target_link_libraries(SkipList PRIVATE ${LIBPMEM_LIBRARY} ${LIBPMEMOBJ_LIBRARY})

install(TARGETS SkipList DESTINATION /usr/local/lib)
install(FILES
        db/KeyValuePair.h
        db/SkipListIterator.h
        db/Status.h
        db/Db.h
        db/KeyValueSize.h
        storage/SkipListNode.h
        storage/SkipList.h
        storage/SkipListInternalNode.h
        storage/SkipListLeafNode.h
        storage/PersistentMemoryPool.h
        storage/PersistentLeaf.h
        storage/WritePosition.h
        storage/utils/LevelGenerator.h
        configuration/Configuration.h
        comparator/KeyComparator.h
        comparator/StringKeyComparator.h
        DESTINATION /usr/local/include)