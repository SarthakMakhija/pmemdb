#ifndef _SkipListNode_
#define _SkipListNode_

#include <string>
#include <functional>
#include "KeyValuePair.h"

using namespace std;

class SkipListNode {
    protected:
    string key;
    string value;

    public:
    SkipListNode();
    SkipListNode(string key, string value);

    virtual bool isLeaf() = 0;
    virtual SkipListNode* addToRightWith(string key, string value);

    bool matchesKey(string key) const;
    bool isKeyLessEqualTo(string key);
    void updateValue(string value);
    KeyValuePair getKeyValuePair();
    KeyValuePair rightKeyValuePair();
};
#endif