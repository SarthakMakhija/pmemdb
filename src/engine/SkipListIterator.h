#ifndef _SkipListIterator_
#define _SkipListIterator_

#include <string>
#include "SkipListNode.h"
#include "SkipListNodes.h"

class SkipListIterator {   
    private:
    SkipListNode* startingNode;
    
    pair<SkipListNodes, SkipListNode*> insertPositionsFor(string key);

    public:
    SkipListIterator(SkipListNode* startingNode);

    void put(string key, string value);
    pair<string, bool> getBy(string key);
    void update(string key, string value);
    void deleteBy(string key);        
};

#endif