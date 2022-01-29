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

    pair<SkipListNode*, bool> existenceByNode = this -> getByKey(key);
	if (!existenceByNode.second){
        this -> multiLevelPut(key, value);
        return;
    }
    this -> update(key, value, existenceByNode.first);
}

void SkipList::update(string key, string value) {
    if (key == "" || value == "") {
        throw std::invalid_argument("key and value can not be blank while updating");
    }

    this -> update(key, value, this -> tower.back());
}


pair<string, bool> SkipList::get(string key) {
    pair<SkipListNode*, bool> existenceByNode = this -> getByKey(key);
    if (existenceByNode.second) {
        return make_pair(existenceByNode.first -> keyValuePair().getValue(), true);
    }
    return make_pair("", false);
}

void SkipList::multiLevelPut(string key, string value) {
    SkipListNodes parents = this -> collectNodes(key);
    
    SkipListNode* left = parents.pop();
    SkipListNode* node = left -> addToRightWith(key, value);

    while (rand() % 2 == 1 && !parents.isEmpty()) {
        left = parents.pop();
		SkipListNode* newNode = left -> addToRightWith(key, value);
        if (!newNode -> isLeaf()) {
		    static_cast<SkipListInternalNode*>(newNode) -> updateDown(node);
        }
		node = newNode;
    }
}

SkipListNodes SkipList::collectNodes(string key) {
    SkipListNode *targetNode = this -> tower.back();
    return SkipListIterator(targetNode).insertPositions(key);
}

void SkipList::update(string key, string value, SkipListNode* startingNode) {
    SkipListIterator(startingNode).update(key, value);
}

pair<SkipListNode*, bool> SkipList::getByKey(string key) {
    SkipListNode *targetNode = this -> tower.back();
    return SkipListIterator(targetNode).getBy(key);
}