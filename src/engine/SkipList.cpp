#include "SkipList.h"
#include "SkipListNode.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include "SkipListIterator.h"
#include <stdlib.h>
#include <stdexcept>

SkipList::SkipList(int towerSize) {
    if (towerSize < 1) {
        throw std::invalid_argument("towerSize has to be greater than or equal to one");
    }

    SkipListLeafNode* sentinelLeafNode = new SkipListLeafNode();
    sentinelLeafNode -> persist();

    this -> header = new SkipListInternalNode("", "", towerSize);
    this -> header -> attach(sentinelLeafNode);
}

void SkipList::put(std::string key, std::string value) {
    if (key == "" || value == "") {
        throw std::invalid_argument("key and value can not be blank while putting");
    }

    std::pair<std::string, bool> valueByExistence = this -> get(key);
	if (!valueByExistence.second){
        SkipListIterator(this -> header).put(key, value);
        return;
    }
    throw std::invalid_argument("key already exists");
}

void SkipList::update(std::string key, std::string value) {
    if (key == "" || value == "") {
        throw std::invalid_argument("key and value can not be blank while updating");
    }

    SkipListIterator(this -> header).update(key, value);
}

/*
void SkipList::deleteBy(std::string key) {
    if (key == "") {
        throw std::invalid_argument("key can not be blank while deleting the corresponding value");
    }

    SkipListIterator(this -> header).deleteBy(key);
}

void SkipList::deleteRange(std::string beginKey, std::string endKey) {
    if (beginKey == endKey || endKey < beginKey) {
        throw std::invalid_argument("beginKey and endKey must be different and endKey must be greater than beginKey");
    }
    SkipListIterator(this -> header).deleteRange(beginKey, endKey);
}
*/

std::pair<std::string, bool> SkipList::get(std::string key) {
    return SkipListIterator(this -> header).getBy(key);
}

std::vector<std::pair<std::string, bool>> SkipList::multiGet(const std::vector<std::string> &keys) {
    return SkipListIterator(this -> header).multiGet(keys);
}

std::vector<KeyValuePair> SkipList::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
    if (maxPairs <= 0) {
        throw std::invalid_argument("maxPairs must be greater than 0");
    }
    if (beginKey == endKey || endKey < beginKey) {
        throw std::invalid_argument("beginKey and endKey must be different and endKey must be greater than beginKey");
    }
    return SkipListIterator(this -> header).scan(beginKey, endKey, maxPairs);
}