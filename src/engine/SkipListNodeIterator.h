#ifndef _SkipListNodeIterator_
#define _SkipListNodeIterator_

#include <string>
#include "SkipListNode.h"

class SkipListNodeIterator {   
    private:
    SkipListNode* startingNode;

    public:
    SkipListNodeIterator(SkipListNode* startingNode);
    SkipListNode* iterate(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block);
    pair<SkipListNode*, bool> getBy(string key);
};

#endif