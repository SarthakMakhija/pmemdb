#include "SkipList.h"
#include "SkipListNode.h"
#include "SkipListInternalNode.h"
#include "SkipListLeafNode.h"
#include "SkipListIterator.h"
#include <stdlib.h>
#include <stdexcept>

SkipList::SkipList(int towerSize, double probability) {
    if (towerSize < 1) {
        throw std::invalid_argument("towerSize has to be greater than or equal to one");
    }
    if (probability < 0 || probability > 1) {
        throw std::invalid_argument("probability must be less than 1 and greater than zero");
    }
    SkipListLeafNode* sentinelLeafNode = new SkipListLeafNode();
    sentinelLeafNode -> persist();

    this -> header = new SkipListInternalNode("", "", towerSize);
    this -> header -> attach(sentinelLeafNode);
    this -> probability = probability;
}

void SkipList::put(std::string key, std::string value) {
    if (key == "" || value == "") {
        throw std::invalid_argument("key and value can not be blank while putting");
    }

    SkipListIterator(this -> header).put(key, value, this -> probability);
}

void SkipList::update(std::string key, std::string value) {
    if (key == "" || value == "") {
        throw std::invalid_argument("key and value can not be blank while updating");
    }

    std::lock_guard<std::shared_mutex> lock(this -> mutex_);
    SkipListIterator(this -> header).update(key, value);
}

void SkipList::deleteBy(std::string key) {
    if (key == "") {
        throw std::invalid_argument("key can not be blank while deleting the corresponding value");
    }

    std::lock_guard<std::shared_mutex> lock(this -> mutex_);
    SkipListIterator(this -> header).deleteBy(key);
}

void SkipList::deleteRange(std::string beginKey, std::string endKey) {
    if (beginKey == endKey || endKey < beginKey) {
        throw std::invalid_argument("beginKey and endKey must be different and endKey must be greater than beginKey");
    }

    std::lock_guard<std::shared_mutex> lock(this -> mutex_);
    SkipListIterator(this -> header).deleteRange(beginKey, endKey);
}

std::pair<std::string, bool> SkipList::get(std::string key) {
    std::shared_lock<std::shared_mutex> lock(this -> mutex_);
    return SkipListIterator(this -> header).getBy(key);
}

std::vector<std::pair<std::string, bool>> SkipList::multiGet(const std::vector<std::string> &keys) {
    std::shared_lock<std::shared_mutex> lock(this -> mutex_);
    return SkipListIterator(this -> header).multiGet(keys);
}

std::vector<KeyValuePair> SkipList::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
    if (maxPairs <= 0) {
        throw std::invalid_argument("maxPairs must be greater than 0");
    }
    if (beginKey == endKey || endKey < beginKey) {
        throw std::invalid_argument("beginKey and endKey must be different and endKey must be greater than beginKey");
    }

    std::shared_lock<std::shared_mutex> lock(this -> mutex_);
    return SkipListIterator(this -> header).scan(beginKey, endKey, maxPairs);
}