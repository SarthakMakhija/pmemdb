#include "SkipListIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"

SkipListIterator::SkipListIterator(SkipListNode* startingNode) : startingNode{startingNode} {
}

void SkipListIterator::put(std::string key, std::string value) {
    if (this -> startingNode -> isLeaf()) {
         static_cast<SkipListLeafNode*>(this -> startingNode) -> put(key, value);
         return;
    }
    
    std::pair<SkipListNodes, SkipListNode*> leafNodeByInternalNodes  = this -> insertPositionsFor(key);
    SkipListNode* node = static_cast<SkipListLeafNode*>(leafNodeByInternalNodes.second) -> put(key, value);
    SkipListNodes parents = leafNodeByInternalNodes.first;
    SkipListNode* left = nullptr;

    while (rand() % 2 == 1 && !parents.isEmpty()) {
        left = parents.pop();
		SkipListNode* newNode = static_cast<SkipListInternalNode*>(left) -> addToRightWith(key, value);
        if (!newNode -> isLeaf()) {
		    static_cast<SkipListInternalNode*>(newNode) -> updateDown(node);
        }
		node = newNode;
    }
}

std::vector<std::pair<std::string, bool>> SkipListIterator::multiGet(const std::vector<std::string> &keys) {
    std::vector<std::pair<std::string, bool>> result;   
   
    for (auto key : keys) {
        result.push_back(getBy(key));
    }
    return result;
}

std::pair<std::string, bool> SkipListIterator::getBy(std::string key) {
    if (!this -> startingNode -> isLeaf()) {
        std::pair<SkipListNode*, bool> existenceByNode = static_cast<SkipListInternalNode*>(this -> startingNode) -> getBy(key);
        if (existenceByNode.second) {
            return std::make_pair(existenceByNode.first -> keyValuePair().getValue(), true);
        }
        return static_cast<SkipListLeafNode*>(existenceByNode.first) -> getBy(key);
    } else {
        return static_cast<SkipListLeafNode*>(this -> startingNode) -> getBy(key);
    }
}

std::vector<KeyValuePair> SkipListIterator::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
    if (!this -> startingNode -> isLeaf()) {
        SkipListNode* leafNode = static_cast<SkipListInternalNode*>(this -> startingNode) -> scan(beginKey);
        return static_cast<SkipListLeafNode*>(leafNode) -> scan(beginKey, endKey, maxPairs);
    } else {
        return static_cast<SkipListLeafNode*>(this -> startingNode) -> scan(beginKey, endKey, maxPairs);
    }
}

void SkipListIterator::update(std::string key, std::string value) {
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

void SkipListIterator::deleteBy(std::string key) {
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

void SkipListIterator::deleteRange(std::string beginKey, std::string endKey) {
    SkipListNode* node = nullptr;

    if (!this -> startingNode -> isLeaf()) {
        node = static_cast<SkipListInternalNode*>(this -> startingNode) -> deleteRange(beginKey, endKey);
        if (node != nullptr && node -> isLeaf()) {
            static_cast<SkipListLeafNode*>(node) -> deleteRange(beginKey, endKey);
        }
    } else {
        static_cast<SkipListLeafNode*>(this -> startingNode) -> deleteRange(beginKey, endKey);
    }
}

std::pair<SkipListNodes, SkipListNode*> SkipListIterator::insertPositionsFor(std::string key) {
    SkipListNodes nodes;
    
    std::pair<std::vector<SkipListNode*>, SkipListNode*> leafNodeByInternalNodes 
            = static_cast<SkipListInternalNode*>(this -> startingNode) -> insertPositionsFor(key);
    
    nodes.addAll(leafNodeByInternalNodes.first);        
    return std::make_pair(nodes,  leafNodeByInternalNodes.second);
}