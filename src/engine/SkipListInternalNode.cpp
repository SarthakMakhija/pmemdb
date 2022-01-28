#include "SkipListInternalNode.h"
#include "KeyValuePair.h"
#include <iostream>

SkipListInternalNode::SkipListInternalNode() : SkipListInternalNode("", "") {}

SkipListInternalNode::SkipListInternalNode(string key, string value) : SkipListNode(key, value)  {
    this -> right = nullptr;
    this -> down = nullptr;
}

bool SkipListInternalNode::isLeaf() {
    return false;
}

SkipListNode* SkipListInternalNode::traverse(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block) {
    SkipListNode *targetNode = this;
    for(; !targetNode -> isLeaf(); targetNode = static_cast<SkipListInternalNode*>(targetNode) -> down) {
        while(static_cast<SkipListInternalNode*>(targetNode) -> right != nullptr && static_cast<SkipListInternalNode*>(targetNode) -> right -> isKeyLessEqualTo(key)) {
			targetNode = static_cast<SkipListInternalNode*>(targetNode) -> right;
		}
        
        pair<SkipListNode*, bool> executionStatus = block(static_cast<SkipListNode*>(targetNode));
        if (executionStatus.second) {
            return executionStatus.first;
        }
    }
    if (targetNode -> isLeaf()) {
        return targetNode;
    }
    return nullptr;
}

SkipListNode* SkipListInternalNode::addToRightWith(string key, string value) {
    SkipListInternalNode* newNode = new SkipListInternalNode(key, value);
	newNode -> updateRight(static_cast<SkipListInternalNode*>(this -> right));
	this -> updateRight(newNode);
	return newNode;
}

void SkipListInternalNode::updateRight(SkipListInternalNode* right) {
    this -> right = right;
}

void SkipListInternalNode::updateDown(SkipListInternalNode* down) {
    this -> down = down;
}

KeyValuePair SkipListInternalNode::downKeyValuePair() {
    if (this -> down != nullptr)  {
        return KeyValuePair(this -> down -> key, this -> down -> value);
    }
    return KeyValuePair("", "");
}

KeyValuePair SkipListInternalNode::rightKeyValuePair() {
    if (this -> right != nullptr)  {
        return KeyValuePair(this -> right -> key, this -> right -> value);
    }
    return  KeyValuePair("", "");
}