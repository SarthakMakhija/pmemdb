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

void SkipList::put(string key, string value) {
    if (key == "" || value == "") {
        throw std::invalid_argument("key and value can not be blank while putting");
    }

    pair<string, bool> valueByExistence = this -> get(key);
	if (!valueByExistence.second){
        SkipListIterator(this -> tower.back()).put(key, value);
        return;
    }
    throw std::invalid_argument("key already exists");
}

void SkipList::update(string key, string value) {
    if (key == "" || value == "") {
        throw std::invalid_argument("key and value can not be blank while updating");
    }

    this -> update(key, value, this -> tower.back());
}

void SkipList::deleteBy(string key) {
    if (key == "") {
        throw std::invalid_argument("key can not be blank while deleting the corresponding value");
    }

    SkipListIterator(this -> tower.back()).deleteBy(key);
}

pair<string, bool> SkipList::get(string key) {
    SkipListNode *targetNode = this -> tower.back();
    return SkipListIterator(targetNode).getBy(key);
}

void SkipList::update(string key, string value, SkipListNode* startingNode) {
    SkipListIterator(startingNode).update(key, value);
}