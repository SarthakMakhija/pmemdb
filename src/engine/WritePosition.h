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

struct DeletePosition {
    std::vector<SkipListInternalNode*> positions;
    int deleteLevel;
    SkipListNode* internal;
    SkipListNode* leaf;
};

struct DeleteRangePosition {
    std::vector<SkipListInternalNode*> positions;
    int deleteLevel;
    SkipListNode* internal;
    SkipListNode* leaf;
};