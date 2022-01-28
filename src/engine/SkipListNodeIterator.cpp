#include "SkipListNodeIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include <iostream>

SkipListNodeIterator::SkipListNodeIterator(SkipListNode* startingNode) : startingNode{startingNode} {
}

SkipListNode* SkipListNodeIterator::iterate(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block) {
    SkipListNode* node = nullptr;

    if (!this -> startingNode -> isLeaf()) {
        node = static_cast<SkipListInternalNode*>(this -> startingNode) -> iterate(key, block);
        if (node != nullptr && node -> isLeaf()) {
            return static_cast<SkipListLeafNode*>(node) -> iterate(key, block);
        }
        return node;
    } else {
        return static_cast<SkipListLeafNode*>(this -> startingNode) -> iterate(key, block);
    }
    return nullptr;
}
