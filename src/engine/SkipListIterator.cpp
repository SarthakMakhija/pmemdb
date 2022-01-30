#include "SkipListIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"

SkipListIterator::SkipListIterator(SkipListNode* startingNode) : startingNode{startingNode} {
}

pair<SkipListNode*, bool> SkipListIterator::getBy(string key) {
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

void SkipListIterator::update(string key, string value) {
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

void SkipListIterator::deleteBy(string key) {
    SkipListNode* node = nullptr;

    if (!this -> startingNode -> isLeaf()) {
        node = static_cast<SkipListInternalNode*>(this -> startingNode) -> deleteBy(key);
        if (node != nullptr && node -> isLeaf()) {
            static_cast<SkipListLeafNode*>(node) -> deleteBy(key);
        }
    } else {
        static_cast<SkipListLeafNode*>(this -> startingNode) -> deleteBy(key);
    }
}

SkipListNodes SkipListIterator::insertPositions(string key) {
    SkipListNodes nodes;

    if (!this -> startingNode -> isLeaf()) {
        pair<vector<SkipListNode*>, SkipListNode*> leafNodeByPositionNodes = static_cast<SkipListInternalNode*>(this -> startingNode) -> insertPositionsFor(key);
        if (leafNodeByPositionNodes.second != nullptr && leafNodeByPositionNodes.second -> isLeaf()) {
            leafNodeByPositionNodes.first.push_back(static_cast<SkipListLeafNode*>(leafNodeByPositionNodes.second) -> insertPositionFor(key));
        }
        nodes.addAll(leafNodeByPositionNodes.first);        
    } else {
        nodes.add(static_cast<SkipListLeafNode*>(this -> startingNode) -> insertPositionFor(key));
    }
    return nodes;
}