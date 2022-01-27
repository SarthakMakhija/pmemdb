#ifndef _SkipListNode_
#define _SkipListNode_

#include <string>
#include <functional>
#include "KeyValuePair.h"

using namespace std;

class SkipListNode {
    private:
    string key;
    string value;
    SkipListNode* right;
    SkipListNode* down;

    public:
    SkipListNode();
    SkipListNode(string key, string value);

    bool matchesKey(string key) const;
    bool isKeyLessEqualTo(string key);
    SkipListNode* addToRightWith(string key, string value);
    void updateDown(SkipListNode* down);
    void updateRight(SkipListNode* right);
    void updateValue(string value);
    KeyValuePair getKeyValuePair();
    KeyValuePair rightKeyValuePair();
    KeyValuePair downKeyValuePair();
    SkipListNode* traverse(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block);
};
#endif