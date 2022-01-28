#include "SkipListNodeIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include <iostream>

SkipListNodeIterator::SkipListNodeIterator(SkipListNode* startingNode) : startingNode{startingNode} {
}

SkipListNode* SkipListNodeIterator::iterate(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block) {
    SkipListNode* node = nullptr;

    if (!this -> startingNode -> isLeaf()) {
        node = static_cast<SkipListInternalNode*>(this -> startingNode) -> traverse(key, block);
        //cout << "node " << node << endl;
        //cout << node->getKeyValuePair().getKey() << endl;
        //cout << node->getKeyValuePair().getValue() << endl;
        if (node !=  nullptr && node -> isLeaf()) {
            node =  static_cast<SkipListLeafNode*>(node) -> traverse(key, block);
            //cout << "node " << node << endl;
            return node;
        }
        return node;
    } else {
        return static_cast<SkipListLeafNode*>(this -> startingNode) -> traverse(key, block);
    }
    return nullptr;
}
