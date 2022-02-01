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

bool SkipListLeafNode::matchesKey(string key) const {
    return this -> key == key;
}

bool SkipListLeafNode::isKeyLessEqualTo(string key) {
    return this -> key <= key;
}

KeyValuePair SkipListLeafNode::keyValuePair() {
    return KeyValuePair(this -> key, this -> value);
}

KeyValuePair SkipListLeafNode::rightKeyValuePair() {
    if (this -> right != nullptr)  {
        return KeyValuePair(this -> right -> key, this -> right -> value);
    }
    return  KeyValuePair("", "");
}

void SkipListLeafNode::updateRight(SkipListLeafNode* right) {
    this -> right = right;
}

SkipListLeafNode* SkipListLeafNode::put(string key, string value) {
    SkipListLeafNode *targetNode = this;
    while(targetNode -> right != nullptr && targetNode -> right -> isKeyLessEqualTo(key)) {
        targetNode = targetNode -> right;
    }
    SkipListLeafNode* newNode = new SkipListLeafNode(key, value);
	newNode     -> updateRight(targetNode -> right);
	targetNode  -> updateRight(newNode);
	
    return newNode;
}

pair<string, bool>  SkipListLeafNode::getBy(string key) {
    SkipListLeafNode *targetNode = this;
    while(targetNode -> right != nullptr && targetNode -> right -> isKeyLessEqualTo(key)) {
        targetNode = targetNode -> right;
    }
    if (targetNode -> matchesKey(key)) {
        return make_pair(targetNode -> value, true);
    }
    return make_pair("", false);
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

void SkipListLeafNode::deleteBy(string key) {
    SkipListLeafNode *previusNode = nullptr;
    SkipListLeafNode *targetNode  = this;

    while(targetNode -> right != nullptr && targetNode -> right -> isKeyLessEqualTo(key)) {        
        previusNode = targetNode;
        targetNode  = targetNode -> right;
    }
    if (targetNode -> matchesKey(key)) {
        previusNode -> right = targetNode -> right;
        targetNode  -> right = nullptr;
        delete targetNode;
    }
}

void SkipListLeafNode::updateValue(string value) {
    this -> value = value;
}