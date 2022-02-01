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
    bool matchesKey(string key) const;
    bool isKeyLessEqualTo(string key);
    KeyValuePair keyValuePair();
    KeyValuePair rightKeyValuePair();
    
    void updateRight(SkipListLeafNode* right);
    
    SkipListLeafNode* put(string key, string value);
    pair<string, bool> getBy(string key);
    void update(string key, string value);
    void deleteBy(string key);
    SkipListNode* insertPositionFor(string key);
};

#endif