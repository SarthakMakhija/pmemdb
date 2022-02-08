#ifndef _SkipList_
#define _SkipList_

#include <string>
#include <vector>
#include "SkipListNode.h"
#include "SkipListNodes.h"



class SkipList {
    private:
    std::vector<SkipListNode *> tower;

    void update(std::string key, std::string value, SkipListNode* startingNode);

    public:
    SkipList(int towerSize);
    
    void put(std::string key, std::string value);
    void update(std::string key, std::string value);
    void deleteBy(std::string key);
    void deleteRange(std::string beginKey, std::string endKey);
    std::pair<std::string, bool> get(std::string key);
    std::vector<std::pair<std::string, bool>> multiGet(const std::vector<std::string> &keys);
    std::vector<KeyValuePair> scan(std::string beginKey, std::string endKey, int64_t maxPairs);
};
#endif