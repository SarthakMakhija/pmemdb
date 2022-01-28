#include "SkipListLeafNode.h"
#include "KeyValuePair.h"
#include <iostream>

SkipListLeafNode::SkipListLeafNode() : SkipListLeafNode("", "") {}

SkipListLeafNode::SkipListLeafNode(string key, string value) : SkipListNode(key, value)  {
    this -> right = nullptr;
}

bool SkipListLeafNode::isLeaf() {
    return true;
}

SkipListNode* SkipListLeafNode::traverse(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block) {
    SkipListLeafNode *targetNode = this;
    while(targetNode -> right != nullptr && targetNode -> right -> isKeyLessEqualTo(key)) {
        targetNode = static_cast<SkipListLeafNode*>(targetNode -> right);
    }
    pair<SkipListNode*, bool> executionStatus = block(static_cast<SkipListNode*>(targetNode));
    if (executionStatus.second) {
        return executionStatus.first;
    }
    return nullptr;
}

SkipListNode* SkipListLeafNode::addToRightWith(string key, string value) {
    SkipListLeafNode* newNode = new SkipListLeafNode(key, value);
	newNode -> updateRight(static_cast<SkipListLeafNode*>(this -> right));
	this -> updateRight(newNode);
	return newNode;
}

void SkipListLeafNode::updateRight(SkipListLeafNode* right) {
    this -> right = right;
}

KeyValuePair SkipListLeafNode::rightKeyValuePair() {
    if (this -> right != nullptr)  {
        return KeyValuePair(this -> right -> key, this -> right -> value);
    }
    return  KeyValuePair("", "");
}