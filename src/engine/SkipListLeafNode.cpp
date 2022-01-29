#include "SkipListLeafNode.h"
#include "KeyValuePair.h"

SkipListLeafNode::SkipListLeafNode() : SkipListLeafNode("", "") {}

SkipListLeafNode::SkipListLeafNode(string key, string value) {
    this -> key     = key;
    this -> value   = value;
    this -> right   = nullptr;
}

bool SkipListLeafNode::isLeaf() {
    return true;
}

pair<SkipListNode*, bool> SkipListLeafNode::getBy(string key) {
    SkipListLeafNode *targetNode = this;
    while(targetNode -> right != nullptr && targetNode -> right -> isKeyLessEqualTo(key)) {
        targetNode = targetNode -> right;
    }
    if (targetNode -> matchesKey(key)) {
        return make_pair(targetNode, true);
    }
    return make_pair(nullptr, false);
}

void SkipListLeafNode::update(string key, string value) {
    SkipListLeafNode *targetNode = this;
    while(targetNode -> right != nullptr && targetNode -> right -> isKeyLessEqualTo(key)) {
        targetNode = targetNode -> right;
    }
    if (targetNode -> matchesKey(key)) {
        targetNode -> updateValue(value);
    }
}

SkipListNode* SkipListLeafNode::insertPosition(string key) {
    SkipListLeafNode *targetNode = this;
    while(targetNode -> right != nullptr && targetNode -> right -> isKeyLessEqualTo(key)) {
        targetNode = targetNode -> right;
    }    
    return targetNode;
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

KeyValuePair SkipListLeafNode::getKeyValuePair() {
    return KeyValuePair(this -> key, this -> value);
}

KeyValuePair SkipListLeafNode::rightKeyValuePair() {
    if (this -> right != nullptr)  {
        return KeyValuePair(this -> right -> key, this -> right -> value);
    }
    return  KeyValuePair("", "");
}

bool SkipListLeafNode::matchesKey(string key) const {
    return this -> key == key;
}

bool SkipListLeafNode::isKeyLessEqualTo(string key) {
    return this -> key <= key;
}

void SkipListLeafNode::updateValue(string value) {
    this -> value = value;
}