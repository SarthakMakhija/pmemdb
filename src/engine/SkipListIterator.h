#ifndef _SkipListIterator_
#define _SkipListIterator_

#include <string>
#include "SkipListNode.h"
#include "SkipListNodes.h"

class SkipListIterator {   
    private:
    SkipListNode* startingNode;

    public:
    SkipListIterator(SkipListNode* startingNode);

    pair<SkipListNode*, bool> getBy(string key);
    void update(string key, string value);
    SkipListNodes insertPositions(string key);
};

#endif