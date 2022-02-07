#ifndef _SkipListNodes_
#define _SkipListNodes_

#include "SkipListNode.h"
#include <string>
#include <vector>

class SkipListNodes {
    private:
    std::vector<SkipListNode *> nodes;

    public:
    void add(SkipListNode* node);
    void addAll(std::vector<SkipListNode*> const &vector);
    SkipListNode* pop();
    bool isEmpty();
};
#endif