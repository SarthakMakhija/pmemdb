add_library(SkipList SkipListNode.h SkipListNode.cpp KeyValuePair.h KeyValuePair.cpp SkipListNodes.h SkipListNodes.cpp SkipList.h SkipList.cpp)
target_link_libraries(SkipList glog::glog)