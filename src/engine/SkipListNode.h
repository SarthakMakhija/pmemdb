#ifndef _SkipListNode_
#define _SkipListNode_

#include <string>
#include <functional>
#include "KeyValuePair.h"

using namespace std;

class SkipListNode {
    public:
    virtual bool isLeaf() = 0;
    virtual SkipListNode* addToRightWith(string key, string value) = 0;
    virtual bool matchesKey(string key) const = 0;
    virtual bool isKeyLessEqualTo(string key) = 0;
    virtual void updateValue(string value) = 0;
    virtual KeyValuePair getKeyValuePair()  = 0;
    virtual KeyValuePair rightKeyValuePair() = 0;
};
#endif