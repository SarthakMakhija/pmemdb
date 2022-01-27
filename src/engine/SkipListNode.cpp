#include "SkipListNode.h"

SkipListNode::SkipListNode() : SkipListNode("", "") {}

SkipListNode::SkipListNode(string  key, string value) {
    this -> key = key;
    this -> value = value;
    this  -> right =  nullptr;
    this -> down = nullptr;
}

bool SkipListNode::matchesKey(string key) const {
    return this -> key == key;
}

bool SkipListNode::isKeyLessEqualTo(string key) {
    return this -> key <= key;
}

SkipListNode* SkipListNode::addToRightWith(string key, string value) {
    SkipListNode* newNode = new SkipListNode(key, value);
	newNode -> updateRight(this -> right);
	this -> updateRight(newNode);
	return newNode;
}

void SkipListNode::updateDown(SkipListNode* down) {
    this -> down = down;
}

void SkipListNode::updateRight(SkipListNode* right) {
    this -> right = right;
}

void SkipListNode::updateValue(string value) {
    this -> value = value;
}

KeyValuePair SkipListNode::getKeyValuePair() {
    return KeyValuePair(this -> key, this -> value);
}

KeyValuePair SkipListNode::rightKeyValuePair() {
    if (this -> right != nullptr)  {
        return KeyValuePair(this -> right -> key, this -> right -> value);
    }
    return  KeyValuePair("", "");
}

KeyValuePair SkipListNode::downKeyValuePair() {
    if (this -> down != nullptr)  {
        return KeyValuePair(this -> down -> key, this -> down -> value);
    }
    return  KeyValuePair("", "");
}

SkipListNode* SkipListNode::traverse(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block) {
    for(SkipListNode *targetNode = this; targetNode != nullptr; targetNode = targetNode -> down) {
        while(targetNode -> right != nullptr && targetNode -> right -> isKeyLessEqualTo(key)) {
			targetNode = targetNode -> right;
		}
        pair<SkipListNode*, bool> executionStatus = block(targetNode);
        if (executionStatus.second) {
            return executionStatus.first;
        }
    }
    return nullptr;
}