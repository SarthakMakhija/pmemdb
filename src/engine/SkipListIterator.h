#ifndef _SkipListIterator_
#define _SkipListIterator_

#include <string>
#include "SkipListNode.h"

class SkipListIterator {   
    private:
    SkipListNode* startingNode;
    
    public:
    SkipListIterator(SkipListNode* startingNode);

    void put(std::string key, std::string value, double probability);
    std::pair<std::string, bool> getBy(std::string key);
    std::vector<std::pair<std::string, bool>> multiGet(const std::vector<std::string> &keys);
    std::vector<KeyValuePair> scan(std::string beginKey, std::string endKey, int64_t maxPairs);
    void update(std::string key, std::string value);
    void deleteBy(std::string key);        
    void deleteRange(std::string beginKey, std::string endKey);
};

#endif