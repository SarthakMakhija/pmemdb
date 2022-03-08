#include "SkipListIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include <algorithm>

SkipListIterator::SkipListIterator(SkipListNode* startingNode) : startingNode{startingNode} {
}

void SkipListIterator::put(std::string key, std::string value, double probability) {
    try {
        PutPosition putPosition = static_cast<SkipListInternalNode*>(this -> startingNode) -> putPositionOf(key, probability);
        if (putPosition.leaf != nullptr) {
            SkipListLeafNode* newLeaf = static_cast<SkipListLeafNode*>(putPosition.leaf) -> put(key, value);
            SkipListNode* newInternal = static_cast<SkipListInternalNode*>(putPosition.internal) -> put(key, value, putPosition.positions, putPosition.newLevel);
            static_cast<SkipListInternalNode*>(newInternal) -> attach(newLeaf);
        }
    } catch(...){}
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
    try {
        UpdatePosition updatePosition = static_cast<SkipListInternalNode*>(this -> startingNode) -> updatePositionOf(key);
        if (updatePosition.leaf != nullptr) {
            static_cast<SkipListLeafNode*>(updatePosition.leaf) -> update(key, value);
            static_cast<SkipListInternalNode*>(updatePosition.internal) -> update(key, value);
        }
    } catch(...) {}
}

void SkipListIterator::deleteBy(std::string key) {
    try {
        DeletePosition deletePosition = static_cast<SkipListInternalNode*>(this -> startingNode) -> deletePositionOf(key);
        if (deletePosition.internal != nullptr && deletePosition.leaf != nullptr) {
            static_cast<SkipListLeafNode*>(deletePosition.leaf) -> deleteBy(key);
            static_cast<SkipListInternalNode*>(deletePosition.internal) -> deleteBy(key, deletePosition.positions, deletePosition.deleteLevel);
        }
    } catch(...){}
}

void SkipListIterator::deleteRange(std::string beginKey, std::string endKey) {
    try {
        DeleteRangePosition deletePosition = static_cast<SkipListInternalNode*>(this -> startingNode) -> deleteRangePositionOf(beginKey, endKey);
        if (deletePosition.internal != nullptr && deletePosition.leaf != nullptr) {
            static_cast<SkipListLeafNode*>(deletePosition.leaf) -> deleteRange(beginKey, endKey);
            static_cast<SkipListInternalNode*>(deletePosition.internal) -> deleteRange(beginKey, endKey, deletePosition.positions, deletePosition.deleteLevel);
        }
    } catch(...) {}
}