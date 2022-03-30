find_path(LIBPMEMOBJ_INCLUDE_DIR libpmemobj.h)
find_path(LIBPMEM_INCLUDE_DIR libpmem.h)

find_library(LIBPMEM_LIBRARY NAMES pmem libpmem)
find_library(LIBPMEMOBJ_LIBRARY NAMES pmemobj libpmemobj)

add_library(SkipList
        KeyValuePair.h
        KeyValuePair.cpp
        SkipListNode.h
        SkipList.h
        SkipList.cpp
        SkipListInternalNode.h
        SkipListInternalNode.cpp
        SkipListLeafNode.h
        SkipListLeafNode.cpp
        SkipListIterator.h
        SkipListIterator.cpp
        PersistentMemoryPool.h
        PersistentMemoryPool.cpp
        PersistentLeaf.h
        WritePosition.h
        Status.h
        Db.h
        Db.cpp
        Configuration.h
        Configuration.cpp
        KeyComparator.h
        StringKeyComparator.h
        StringKeyComparator.cpp
        KeyValueSize.h
        KeyValueSize.cpp)

target_include_directories(SkipList PRIVATE ${LIBPMEMOBJ_INCLUDE_DIR} ${LIBPMEM_INCLUDE_DIR})
target_link_libraries(SkipList PRIVATE ${LIBPMEM_LIBRARY} ${LIBPMEMOBJ_LIBRARY})
