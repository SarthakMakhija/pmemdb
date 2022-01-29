#include "SkipListNodeIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"

SkipListNodeIterator::SkipListNodeIterator(SkipListNode* startingNode) : startingNode{startingNode} {
}

SkipListNode* SkipListNodeIterator::iterate(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block) {
    SkipListNode* node = nullptr;

    if (!this -> startingNode -> isLeaf()) {
        node = this -> startingNode -> iterate(key, block);
        if (node != nullptr && node -> isLeaf()) {
            return static_cast<SkipListLeafNode*>(node) -> iterate(key, block);
        }
        return node;
    } else {
        return this -> startingNode -> iterate(key, block);
    }
}

pair<SkipListNode*, bool> SkipListNodeIterator::getBy(string key) {
    SkipListNode* node = nullptr;

    if (!this -> startingNode -> isLeaf()) {
        pair<SkipListNode*, bool> existenceByNode = static_cast<SkipListInternalNode*>(this -> startingNode) -> getBy(key);
        if (existenceByNode.second) {
            return existenceByNode;
        }
        return static_cast<SkipListLeafNode*>(existenceByNode.first) -> getBy(key);
    } else {
        return static_cast<SkipListLeafNode*>(this -> startingNode) -> getBy(key);
    }
}
