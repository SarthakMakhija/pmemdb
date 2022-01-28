#ifndef _SkipListInternalNode_
#define _SkipListInternalNode_

#include <string>
#include "SkipListNode.h"

class SkipListInternalNode : public SkipListNode {
    private:
    SkipListInternalNode* right;
    SkipListNode* down;

    public:
    SkipListInternalNode(string key, string value);
    SkipListInternalNode();

    bool isLeaf();
    SkipListNode* addToRightWith(string key, string value);
    void updateDown(SkipListNode* down);
    void updateRight(SkipListInternalNode* right);
    KeyValuePair downKeyValuePair();
    KeyValuePair rightKeyValuePair();
    SkipListNode* iterate(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block);
};

#endif