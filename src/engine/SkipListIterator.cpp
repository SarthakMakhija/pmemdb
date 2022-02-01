#include "SkipListIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"

SkipListIterator::SkipListIterator(SkipListNode* startingNode) : startingNode{startingNode} {
}

void SkipListIterator::put(string key, string value) {
    if (this -> startingNode -> isLeaf()) {
         static_cast<SkipListLeafNode*>(this -> startingNode) -> put(key, value);
         return;
    }
    
    pair<SkipListNodes, SkipListNode*> leafNodeByInternalNodes  = this -> insertPositionsFor(key);
    SkipListNode* node = static_cast<SkipListLeafNode*>(leafNodeByInternalNodes.second) -> put(key, value);
    SkipListNodes parents = leafNodeByInternalNodes.first;
    SkipListNode* left = nullptr;

    while (rand() % 2 == 1 && !parents.isEmpty()) {
        left = parents.pop();
		SkipListNode* newNode = left -> addToRightWith(key, value);
        if (!newNode -> isLeaf()) {
		    static_cast<SkipListInternalNode*>(newNode) -> updateDown(node);
        }
		node = newNode;
    }
}

pair<string, bool> SkipListIterator::getBy(string key) {
    if (!this -> startingNode -> isLeaf()) {
        pair<SkipListNode*, bool> existenceByNode = static_cast<SkipListInternalNode*>(this -> startingNode) -> getBy(key);
        if (existenceByNode.second) {
            return make_pair(existenceByNode.first -> keyValuePair().getValue(), true);
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

pair<SkipListNodes, SkipListNode*> SkipListIterator::insertPositionsFor(string key) {
    SkipListNodes nodes;
    
    pair<vector<SkipListNode*>, SkipListNode*> leafNodeByInternalNodes 
            = static_cast<SkipListInternalNode*>(this -> startingNode) -> insertPositionsFor(key);
    
    nodes.addAll(leafNodeByInternalNodes.first);        
    return make_pair(nodes,  leafNodeByInternalNodes.second);
}