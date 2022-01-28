#ifndef _SkipListLeafNode_
#define _SkipListLeafNode_

#include <string>
#include "SkipListNode.h"

class SkipListLeafNode : public SkipListNode {   
    private:
    SkipListLeafNode* right;

    public:
    SkipListLeafNode(string key, string value);
    SkipListLeafNode();
    
    bool isLeaf();
    SkipListNode* addToRightWith(string key, string value);
    void updateDown(SkipListLeafNode* down);
    void updateRight(SkipListLeafNode* right);
    KeyValuePair rightKeyValuePair();
    SkipListNode* traverse(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block);
};

#endif