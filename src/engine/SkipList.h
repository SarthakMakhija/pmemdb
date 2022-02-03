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
    std::pair<std::string, bool> get(std::string key);
};
#endif