#include "SkipListIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"

SkipListIterator::SkipListIterator(SkipListNode* startingNode) : startingNode{startingNode} {
}

void SkipListIterator::put(std::string key, std::string value, double probability) {
    std::pair<SkipListNode*, SkipListNode*> leafByInternalNode = static_cast<SkipListInternalNode*>(this -> startingNode) -> put(key, value, probability);
    SkipListLeafNode* leafNode = static_cast<SkipListLeafNode*>(leafByInternalNode.second) -> put(key, value);
    static_cast<SkipListInternalNode*>(leafByInternalNode.first) -> attach(leafNode);
}

std::vector<std::pair<std::string, bool>> SkipListIterator::multiGet(const std::vector<std::string> &keys) {
    std::vector<std::pair<std::string, bool>> result;   
   
    for (auto key : keys) {
        result.push_back(getBy(key));
    }
    return result;
}

std::pair<std::string, bool> SkipListIterator::getBy(std::string key) {
    std::pair<SkipListNode*, bool> existenceByNode = static_cast<SkipListInternalNode*>(this -> startingNode) -> getBy(key);
    if (existenceByNode.second) {
        return std::make_pair(existenceByNode.first -> keyValuePair().getValue(), true);
    }
    return std::make_pair("", false);
}

std::vector<KeyValuePair> SkipListIterator::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
    return static_cast<SkipListInternalNode*>(this -> startingNode) -> scan(beginKey, endKey, maxPairs);
}

void SkipListIterator::update(std::string key, std::string value) {
    SkipListNode* node = static_cast<SkipListInternalNode*>(this -> startingNode) -> update(key, value);
    if (node != nullptr && node -> isLeaf()) {
        static_cast<SkipListLeafNode*>(node) -> update(key, value);
    }
}

void SkipListIterator::deleteBy(std::string key) {
    SkipListNode* node = static_cast<SkipListInternalNode*>(this -> startingNode) -> deleteBy(key);
    if (node != nullptr && node -> isLeaf()) {
        static_cast<SkipListLeafNode*>(node) -> deleteBy(key);
    }
}

void SkipListIterator::deleteRange(std::string beginKey, std::string endKey) {
    SkipListNode* node = static_cast<SkipListInternalNode*>(this -> startingNode) -> deleteRange(beginKey, endKey);
    if (node != nullptr && node -> isLeaf()) {
        static_cast<SkipListLeafNode*>(node) -> deleteRange(beginKey, endKey);
    }
}