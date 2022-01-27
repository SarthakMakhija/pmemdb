#ifndef _SkipListNodes_
#define _SkipListNodes_

#include "SkipListNode.h"
#include <string>
#include <vector>

using namespace std;

class SkipListNodes {
    private:
    vector<SkipListNode *> nodes;

    public:
    void add(SkipListNode* node);
    SkipListNode* pop();
    bool isEmpty();
};
#endif