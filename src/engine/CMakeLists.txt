find_library(LIB_PMEM NAMES pmem libpmem)
find_library(LIB_PMEM_OBJ NAMES pmemobj libpmemobj)

add_library(SkipList 
            KeyValuePair.h 
            KeyValuePair.cpp 
            SkipListNode.h 
            SkipListNodes.h 
            SkipListNodes.cpp 
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
            PersistentLeaf.h)

target_link_libraries(SkipList 
                      glog::glog
                      ${LIB_PMEM}
                      ${LIB_PMEM_OBJ})