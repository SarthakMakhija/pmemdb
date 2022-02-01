#ifndef _SkipList_
#define _SkipList_

#include <string>
#include <vector>
#include "SkipListNode.h"
#include "SkipListNodes.h"

using namespace std;

class SkipList {
    private:
    vector<SkipListNode *> tower;

    void multiLevelPut(string key, string value);
    SkipListNodes collectNodes(string key);
    void update(string key, string value, SkipListNode* startingNode);

    public:
    SkipList(int towerSize);
    
    void put(string key, string value);
    void update(string key, string value);
    void deleteBy(string key);
    pair<string, bool> get(string key);
};
#endif