#include "SkipList.h"
#include "SkipListNode.h"
#include "SkipListNodes.h"
#include <stdlib.h>
#include <stdexcept>

SkipList::SkipList(int towerSize) {
    if (towerSize < 1) {
        throw std::invalid_argument("towerSize has to be greater than or equal to one");
    }

    for (int index = 0; index < towerSize; index++){
        SkipListNode *sentinelNode = new SkipListNode();
        this -> tower.push_back(sentinelNode);
        if (index >  0) {
            sentinelNode -> updateDown(this -> tower.at(index-1));
        }
    }
}

void SkipList::put(string key, string value) {
    if (key == "") {
        throw std::invalid_argument("key can not be blank");
    }
    if (value == "") {
        throw std::invalid_argument("value can not be blank");
    }

    pair<SkipListNode*, bool> existenceByNode = this -> getByKey(key);
	if (!existenceByNode.second){
        this -> multiLevelPut(key, value);
        return;
    }
    this -> update(key, value, existenceByNode.first);
}

pair<string, bool> SkipList::get(string key) {
    pair<SkipListNode*, bool> existenceByNode = this -> getByKey(key);
    if (existenceByNode.second) {
        return make_pair(existenceByNode.first -> getKeyValuePair().getValue(), true);
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
		newNode -> updateDown(node);
		node = newNode;
    }
}

SkipListNodes SkipList::collectNodes(string key) {
    SkipListNode *targetNode = this -> tower.back();
    SkipListNodes parents;

    targetNode -> traverse(
        key, 
        [&, key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
            parents.add(node);
            return make_pair(node, false);
        }
    );
    return parents;
}

void SkipList::update(string key, string value, SkipListNode* startingNode) {
    startingNode -> traverse(
        key, 
        [key, value] (SkipListNode* node) -> pair<SkipListNode*, bool> {
            if (node -> matchesKey(key)) {
			    node -> updateValue(value);
		    }
            return make_pair(node, false);
        }
    );
}

pair<SkipListNode*, bool> SkipList::getByKey(string key) {
    SkipListNode *targetNode = this -> tower.back();

    SkipListNode* node = targetNode -> traverse(
        key, 
        [key] (SkipListNode* node) -> pair<SkipListNode*, bool> {
        if (node -> matchesKey(key)) {
			return make_pair(node, true);
		} else {
			return make_pair(nullptr, false);
		}
    });

    if (node != nullptr) {
        return make_pair(node, true);
    }
    return make_pair(nullptr, false);
}




