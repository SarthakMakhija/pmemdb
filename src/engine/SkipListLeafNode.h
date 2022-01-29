#ifndef _SkipListLeafNode_
#define _SkipListLeafNode_

#include <string>
#include "SkipListNode.h"

class SkipListLeafNode : public SkipListNode {   
    private:
    string key;
    string value;
    SkipListLeafNode* right;

    void updateValue(string value);
    
    public:
    SkipListLeafNode(string key, string value);
    SkipListLeafNode();
    
    bool isLeaf();
    SkipListNode* addToRightWith(string key, string value);
    void updateRight(SkipListLeafNode* right);
    KeyValuePair getKeyValuePair();
    KeyValuePair rightKeyValuePair();
    bool matchesKey(string key) const;
    bool isKeyLessEqualTo(string key);
    
    pair<SkipListNode*, bool> getBy(string key);
    void update(string key, string value);
    SkipListNode* insertPosition(string key);
};

#endif