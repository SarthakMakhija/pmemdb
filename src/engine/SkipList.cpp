#include "SkipList.h"
#include "SkipListNode.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include "SkipListIterator.h"
#include "SkipListNodes.h"
#include <stdlib.h>
#include <stdexcept>

SkipList::SkipList(int towerSize) {
    if (towerSize < 1) {
        throw std::invalid_argument("towerSize has to be greater than or equal to one");
    }

    for (int index = 0; index < towerSize; index++) {
        SkipListNode *sentinelNode = nullptr;
        if (index == 0) {
            sentinelNode = new SkipListLeafNode();
            static_cast<SkipListLeafNode*>(sentinelNode) -> persist();
        } else {
            sentinelNode = new SkipListInternalNode();
        }
        this -> tower.push_back(sentinelNode);
        if (index >  0) {
            static_cast<SkipListInternalNode*>(sentinelNode) -> updateDown(this -> tower.at(index-1));
        }
    }
}

void SkipList::put(std::string key, std::string value) {
    if (key == "" || value == "") {
        throw std::invalid_argument("key and value can not be blank while putting");
    }

    std::pair<std::string, bool> valueByExistence = this -> get(key);
	if (!valueByExistence.second){
        SkipListIterator(this -> tower.back()).put(key, value);
        return;
    }
    throw std::invalid_argument("key already exists");
}

void SkipList::update(std::string key, std::string value) {
    if (key == "" || value == "") {
        throw std::invalid_argument("key and value can not be blank while updating");
    }

    this -> update(key, value, this -> tower.back());
}

void SkipList::deleteBy(std::string key) {
    if (key == "") {
        throw std::invalid_argument("key can not be blank while deleting the corresponding value");
    }

    SkipListIterator(this -> tower.back()).deleteBy(key);
}

void SkipList::deleteRange(std::string beginKey, std::string endKey) {
    if (beginKey == endKey || endKey < beginKey) {
        throw std::invalid_argument("beginKey and endKey must be different and endKey must be greater than beginKey");
    }
    SkipListIterator(this -> tower.back()).deleteRange(beginKey, endKey);
}

std::pair<std::string, bool> SkipList::get(std::string key) {
    SkipListNode *targetNode = this -> tower.back();
    return SkipListIterator(targetNode).getBy(key);
}

std::vector<std::pair<std::string, bool>> SkipList::multiGet(const std::vector<std::string> &keys) {
    SkipListNode *targetNode = this -> tower.back();
    return SkipListIterator(targetNode).multiGet(keys);
}

std::vector<KeyValuePair> SkipList::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
    if (maxPairs <= 0) {
        throw std::invalid_argument("maxPairs must be greater than 0");
    }
    if (beginKey == endKey || endKey < beginKey) {
        throw std::invalid_argument("beginKey and endKey must be different and endKey must be greater than beginKey");
    }
    SkipListNode *targetNode = this -> tower.back();
    return SkipListIterator(targetNode).scan(beginKey, endKey, maxPairs);
}

void SkipList::update(std::string key, std::string value, SkipListNode* startingNode) {
    SkipListIterator(startingNode).update(key, value);
}