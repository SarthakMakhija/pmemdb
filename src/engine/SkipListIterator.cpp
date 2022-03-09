#include "SkipListIterator.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include <algorithm>

SkipListIterator::SkipListIterator(SkipListNode* startingNode) : startingNode{startingNode} {
}

void SkipListIterator::put(std::string key, std::string value, double probability, std::function<void(void)> postPutHook) {
    PutPosition putPosition = static_cast<SkipListInternalNode*>(this -> startingNode) -> putPositionOf(key, probability);
    if (putPosition.leaf != nullptr) {
        std::pair<SkipListLeafNode*, Status> statusNodePair = static_cast<SkipListLeafNode*>(putPosition.leaf) -> put(key, value, postPutHook);
        if (statusNodePair.second != Status::Failed) {
            SkipListLeafNode* newLeaf = statusNodePair.first;
            SkipListNode* newInternal = static_cast<SkipListInternalNode*>(putPosition.internal) -> put(key, value, putPosition.positions, putPosition.newLevel);
            static_cast<SkipListInternalNode*>(newInternal) -> attach(newLeaf);
        }
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

void SkipListIterator::update(std::string key, std::string value, std::function<void(void)> postUpdateHook) {
    UpdatePosition updatePosition = static_cast<SkipListInternalNode*>(this -> startingNode) -> updatePositionOf(key);
    if (updatePosition.leaf != nullptr) {
        Status status = static_cast<SkipListLeafNode*>(updatePosition.leaf) -> update(key, value, postUpdateHook);
        if (status != Status::Failed) {
            static_cast<SkipListInternalNode*>(updatePosition.internal) -> update(key, value);
        }
    }
}

void SkipListIterator::deleteBy(std::string key, std::function<void(void)> postDeleteHook) {
    DeletePosition deletePosition = static_cast<SkipListInternalNode*>(this -> startingNode) -> deletePositionOf(key);
    if (deletePosition.internal != nullptr && deletePosition.leaf != nullptr) {
        Status status = static_cast<SkipListLeafNode*>(deletePosition.leaf) -> deleteBy(key, postDeleteHook);
        if (status != Status::Failed) {
            static_cast<SkipListInternalNode*>(deletePosition.internal) -> deleteBy(key, deletePosition.positions, deletePosition.deleteLevel);
        }
    }
}

void SkipListIterator::deleteRange(std::string beginKey, std::string endKey, std::function<void(void)> postDeleteRangeHook) {
    DeleteRangePosition deletePosition = static_cast<SkipListInternalNode*>(this -> startingNode) -> deleteRangePositionOf(beginKey, endKey);
    if (deletePosition.internal != nullptr && deletePosition.leaf != nullptr) {
        Status status = static_cast<SkipListLeafNode*>(deletePosition.leaf) -> deleteRange(beginKey, endKey, postDeleteRangeHook);
        if (status != Status::Failed) {
            static_cast<SkipListInternalNode*>(deletePosition.internal) -> deleteRange(beginKey, endKey, deletePosition.positions, deletePosition.deleteLevel);
        }
    }
}