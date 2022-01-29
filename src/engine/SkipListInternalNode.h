#ifndef _SkipListInternalNode_
#define _SkipListInternalNode_

#include <string>
#include "SkipListNode.h"

class SkipListInternalNode : public SkipListNode {
    private:
    string key;
    string value;
    SkipListInternalNode* right;
    SkipListNode* down;

    public:
    SkipListInternalNode(string key, string value);
    SkipListInternalNode();

    bool isLeaf();
    SkipListNode* addToRightWith(string key, string value);
    void updateDown(SkipListNode* down);
    void updateRight(SkipListInternalNode* right);
    KeyValuePair getKeyValuePair();
    KeyValuePair rightKeyValuePair();
    SkipListNode* iterate(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block);
    bool matchesKey(string key) const;
    bool isKeyLessEqualTo(string key);
    void updateValue(string value);
    SkipListNode* getDown();
    pair<SkipListNode*, bool> getBy(string key);
    SkipListNode* update(string key, string value);
};

#endif