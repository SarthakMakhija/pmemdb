#ifndef _SkipListNodeIterator_
#define _SkipListNodeIterator_

#include <string>
#include "SkipListNode.h"
#include "SkipListNodes.h"

class SkipListNodeIterator {   
    private:
    SkipListNode* startingNode;

    public:
    SkipListNodeIterator(SkipListNode* startingNode);

    pair<SkipListNode*, bool> getBy(string key);
    void update(string key, string value);
    SkipListNodes insertPositions(string key);
};

#endif