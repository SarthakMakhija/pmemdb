#ifndef _SkipListLeafNode_
#define _SkipListLeafNode_

#include <string>
#include "SkipListNode.h"

class SkipListLeafNode : public SkipListNode {   
    private:
    string key;
    string value;
    SkipListLeafNode* right;

    public:
    SkipListLeafNode(string key, string value);
    SkipListLeafNode();
    
    bool isLeaf();
    SkipListNode* addToRightWith(string key, string value);
    void updateRight(SkipListLeafNode* right);
    KeyValuePair getKeyValuePair();
    KeyValuePair rightKeyValuePair();
    SkipListNode* iterate(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block);
    bool matchesKey(string key) const;
    bool isKeyLessEqualTo(string key);
    void updateValue(string value);
    pair<SkipListNode*, bool> getBy(string key);
    void update(string key, string value);
    SkipListNode* insertPosition(string key);
};

#endif