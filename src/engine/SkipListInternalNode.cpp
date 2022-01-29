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

SkipListNode* SkipListInternalNode::addToRightWith(string key, string value) {
    SkipListInternalNode* newNode = new SkipListInternalNode(key, value);
	newNode -> updateRight(this -> right);
	this -> updateRight(newNode);
	return newNode;
}

bool SkipListInternalNode::matchesKey(string key) const {
    return this -> key == key;
}

bool SkipListInternalNode::isKeyLessEqualTo(string key) {
    return this -> key <= key;
}

KeyValuePair SkipListInternalNode::keyValuePair() {
    return KeyValuePair(this -> key, this -> value);
}

KeyValuePair SkipListInternalNode::rightKeyValuePair() {
    if (this -> right != nullptr)  {
        return KeyValuePair(this -> right -> key, this -> right -> value);
    }
    return  KeyValuePair("", "");
}

SkipListNode* SkipListInternalNode::getDown()  {
    return this -> down;
}

void SkipListInternalNode::updateDown(SkipListNode* down) {
    this -> down = down;
}

void SkipListInternalNode::updateRight(SkipListInternalNode* right) {
    this -> right = right;
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

SkipListNode* SkipListInternalNode::update(string key, string value) {
    SkipListNode *targetNode = this;
    for(; !targetNode -> isLeaf(); targetNode = static_cast<SkipListInternalNode*>(targetNode) -> down) {
        while(static_cast<SkipListInternalNode*>(targetNode) -> right != nullptr && static_cast<SkipListInternalNode*>(targetNode) -> right -> isKeyLessEqualTo(key)) {
			targetNode = static_cast<SkipListInternalNode*>(targetNode) -> right;
		}        
        if (targetNode -> matchesKey(key)) {
            static_cast<SkipListInternalNode*>(targetNode) -> updateValue(value);
        }
    }
   return targetNode;
}

pair<vector<SkipListNode*>, SkipListNode*> SkipListInternalNode::insertPositions(string key) {
    vector<SkipListNode*> nodes;
    SkipListNode *targetNode = this;

    for(; !targetNode -> isLeaf(); targetNode = static_cast<SkipListInternalNode*>(targetNode) -> down) {
        while(static_cast<SkipListInternalNode*>(targetNode) -> right != nullptr && static_cast<SkipListInternalNode*>(targetNode) -> right -> isKeyLessEqualTo(key)) {
			targetNode = static_cast<SkipListInternalNode*>(targetNode) -> right;
		}        
        nodes.push_back(targetNode);
    }
   return make_pair(nodes, targetNode);
}

void SkipListInternalNode::updateValue(string value) {
    this -> value = value;
}