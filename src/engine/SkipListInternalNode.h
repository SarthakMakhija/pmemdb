#ifndef _SkipListInternalNode_
#define _SkipListInternalNode_

#include <string>
#include <vector>
#include "SkipListNode.h"

class SkipListInternalNode : public SkipListNode {
    private:
    string key;
    string value;
    SkipListInternalNode* right;
    SkipListNode* down;

    void updateValue(string value);

    public:
    SkipListInternalNode(string key, string value);
    SkipListInternalNode();

    bool isLeaf();
    SkipListNode* addToRightWith(string key, string value);
    bool matchesKey(string key) const;
    bool isKeyLessEqualTo(string key);    
    KeyValuePair getKeyValuePair();
    KeyValuePair rightKeyValuePair();

    SkipListNode* getDown();
    void updateDown(SkipListNode* down);
    void updateRight(SkipListInternalNode* right);    
    
    pair<SkipListNode*, bool> getBy(string key);
    SkipListNode* update(string key, string value);
    pair<vector<SkipListNode*>, SkipListNode*> insertPositions(string key);
};

#endif