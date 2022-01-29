#include "SkipListInternalNode.h"
#include "KeyValuePair.h"

SkipListInternalNode::SkipListInternalNode() : SkipListInternalNode("", "") {}

SkipListInternalNode::SkipListInternalNode(string key, string value) {
    this -> key     = key;
    this -> value   = value;
    this -> right   = nullptr;
    this -> down    = nullptr;
}

bool SkipListInternalNode::isLeaf() {
    return false;
}

SkipListNode* SkipListInternalNode::iterate(string key, function<pair<SkipListNode*, bool> (SkipListNode*)> block) {
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

pair<SkipListNode*, bool> SkipListInternalNode::getBy(string key) {
    SkipListNode *targetNode = this;
    for(; !targetNode -> isLeaf(); targetNode = static_cast<SkipListInternalNode*>(targetNode) -> down) {
        while(static_cast<SkipListInternalNode*>(targetNode) -> right != nullptr && static_cast<SkipListInternalNode*>(targetNode) -> right -> isKeyLessEqualTo(key)) {
			targetNode = static_cast<SkipListInternalNode*>(targetNode) -> right;
		}        
        if (targetNode -> matchesKey(key)) {
            return make_pair(targetNode, true);
        }
    }
   return make_pair(targetNode, false);
}

SkipListNode* SkipListInternalNode::addToRightWith(string key, string value) {
    SkipListInternalNode* newNode = new SkipListInternalNode(key, value);
	newNode -> updateRight(this -> right);
	this -> updateRight(newNode);
	return newNode;
}

void SkipListInternalNode::updateRight(SkipListInternalNode* right) {
    this -> right = right;
}

void SkipListInternalNode::updateDown(SkipListNode* down) {
    this -> down = down;
}

KeyValuePair SkipListInternalNode::getKeyValuePair() {
    return KeyValuePair(this -> key, this -> value);
}

KeyValuePair SkipListInternalNode::rightKeyValuePair() {
    if (this -> right != nullptr)  {
        return KeyValuePair(this -> right -> key, this -> right -> value);
    }
    return  KeyValuePair("", "");
}

bool SkipListInternalNode::matchesKey(string key) const {
    return this -> key == key;
}

bool SkipListInternalNode::isKeyLessEqualTo(string key) {
    return this -> key <= key;
}

void SkipListInternalNode::updateValue(string value) {
    this -> value = value;
}

SkipListNode* SkipListInternalNode::getDown()  {
    return this -> down;
}