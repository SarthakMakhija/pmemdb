#include "SkipListIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include <algorithm>

SkipListIterator::SkipListIterator(SkipListNode* startingNode) : startingNode{startingNode} {
}

void SkipListIterator::put(std::string key, std::string value, double probability) {
    PutPosition putPosition = static_cast<SkipListInternalNode*>(this -> startingNode) -> putPosition(key, probability);
    if (putPosition.leaf != nullptr) {
        SkipListLeafNode* newLeaf = static_cast<SkipListLeafNode*>(putPosition.leaf) -> put(key, value);
        SkipListNode* newInternal = static_cast<SkipListInternalNode*>(putPosition.internal) -> put(key, value, putPosition.positions, putPosition.newLevel);
        static_cast<SkipListInternalNode*>(newInternal) -> attach(newLeaf);
    }
}

std::vector<std::pair<std::string, bool>> SkipListIterator::multiGet(std::vector<std::string> keys) {
    std::vector<std::pair<std::string, bool>> result;   

    std::sort(keys.begin(), keys.end());
    SkipListNode* starting = this -> startingNode;

    for (auto key : keys) {
        std::pair<SkipListNode*, bool> existenceByNode = static_cast<SkipListInternalNode*>(startingNode) -> getBy(key);
        if (existenceByNode.second) {
            result.push_back(std::make_pair(existenceByNode.first -> keyValuePair().getValue(), true));
            startingNode = existenceByNode.first;
        } else {
            result.push_back(std::make_pair("", false));
        }
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
    UpdatePosition updatePosition = static_cast<SkipListInternalNode*>(this -> startingNode) -> updatePosition(key);
    if (updatePosition.leaf != nullptr) {
        static_cast<SkipListLeafNode*>(updatePosition.leaf) -> update(key, value);
        static_cast<SkipListInternalNode*>(updatePosition.internal) -> update(key, value);
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