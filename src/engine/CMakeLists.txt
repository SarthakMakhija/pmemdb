add_library(SkipList 
            SkipListNode.h 
            SkipListNode.cpp 
            KeyValuePair.h 
            KeyValuePair.cpp 
            SkipListNodes.h 
            SkipListNodes.cpp 
            SkipList.h 
            SkipList.cpp
            SkipListInternalNode.h 
            SkipListInternalNode.cpp
            SkipListLeafNode.h 
            SkipListLeafNode.cpp
            SkipListNodeIterator.h 
            SkipListNodeIterator.cpp)

target_link_libraries(SkipList glog::glog)