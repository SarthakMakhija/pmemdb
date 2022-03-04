#include "SkipListNode.h"

struct UpdatePosition {
    SkipListNode* internal;
    SkipListNode* leaf;
};


class SkipListInternalNode;

struct PutPosition {
    std::vector<SkipListInternalNode*> positions;
    int newLevel;
    SkipListNode* internal;
    SkipListNode* leaf;
};