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

void SkipListNodeIterator::update(string key, string value) {
    SkipListNode* node = nullptr;

    if (!this -> startingNode -> isLeaf()) {
        node = static_cast<SkipListInternalNode*>(this -> startingNode) -> update(key, value);
        if (node != nullptr && node -> isLeaf()) {
            static_cast<SkipListLeafNode*>(node) -> update(key, value);
        }
    } else {
        static_cast<SkipListLeafNode*>(this -> startingNode) -> update(key, value);
    }
}

SkipListNodes SkipListNodeIterator::insertPositions(string key) {
    SkipListNodes nodes;

    if (!this -> startingNode -> isLeaf()) {
        pair<vector<SkipListNode*>, SkipListNode*> leafNodeByPositionNodes = static_cast<SkipListInternalNode*>(this -> startingNode) -> insertPositions(key);
        if (leafNodeByPositionNodes.second != nullptr && leafNodeByPositionNodes.second -> isLeaf()) {
            leafNodeByPositionNodes.first.push_back(static_cast<SkipListLeafNode*>(leafNodeByPositionNodes.second) -> insertPosition(key));
        }
        nodes.addAll(leafNodeByPositionNodes.first);        
    } else {
        nodes.add(static_cast<SkipListLeafNode*>(this -> startingNode) -> insertPosition(key));
    }
    return nodes;
}