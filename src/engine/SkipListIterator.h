#ifndef _SkipListIterator_
#define _SkipListIterator_

#include <string>
#include "SkipListNode.h"
#include "SkipListNodes.h"

class SkipListIterator {   
    private:
    SkipListNode* startingNode;
    
    std::pair<SkipListNodes, SkipListNode*> insertPositionsFor(std::string key);

    public:
    SkipListIterator(SkipListNode* startingNode);

    void put(std::string key, std::string value);
    std::pair<std::string, bool> getBy(std::string key);
    std::vector<std::pair<std::string, bool>> multiGet(const std::vector<std::string> &keys);
    std::vector<KeyValuePair> scan(std::string beginKey, std::string endKey);
    void update(std::string key, std::string value);
    void deleteBy(std::string key);        
};

#endif