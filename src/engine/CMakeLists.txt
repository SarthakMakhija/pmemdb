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
            SkipListIterator.cpp)

target_link_libraries(SkipList glog::glog)