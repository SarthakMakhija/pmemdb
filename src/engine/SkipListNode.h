#ifndef _SkipListNode_
#define _SkipListNode_

#include <string>
#include "KeyValuePair.h"

using namespace std;

class SkipListNode {
    private:
    string key;
    string value;
    SkipListNode* right;
    SkipListNode* down;

    public:
    SkipListNode(string key, string value);

    bool matchesKey(string key);
    bool isKeyLessEqualTo(string key);
    SkipListNode* addToRightWith(string key, string value);
    void updateDown(SkipListNode* down);
    void updateRight(SkipListNode* right);
    void updateValue(string value);
    KeyValuePair getKeyValuePair();
    KeyValuePair rightKeyValuePair();
    KeyValuePair downKeyValuePair();
};
#endif