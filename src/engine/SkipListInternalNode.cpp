#include "SkipListInternalNode.h"
#include "KeyValuePair.h"

SkipListInternalNode::SkipListInternalNode() : SkipListInternalNode("", "") {}

SkipListInternalNode::SkipListInternalNode(std::string key, std::string value) {
    this -> key     = key;
    this -> value   = value;
    this -> right   = nullptr;
    this -> down    = nullptr;
}

bool SkipListInternalNode::isLeaf() {
    return false;
}

SkipListNode* SkipListInternalNode::addToRightWith(std::string key, std::string value) {
    SkipListInternalNode* newNode = new SkipListInternalNode(key, value);
	newNode -> updateRight(this -> right);
	this -> updateRight(newNode);
	return newNode;
}

bool SkipListInternalNode::matchesKey(std::string key) const {
    return this -> key == key;
}

bool SkipListInternalNode::isKeyLessEqualTo(std::string key) {
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

std::pair<SkipListNode*, bool> SkipListInternalNode::getBy(std::string key) {
    SkipListNode *targetNode = this;
    for(; !targetNode -> isLeaf(); targetNode = static_cast<SkipListInternalNode*>(targetNode) -> down) {
        while(static_cast<SkipListInternalNode*>(targetNode) -> right != nullptr && static_cast<SkipListInternalNode*>(targetNode) -> right -> isKeyLessEqualTo(key)) {
			targetNode = static_cast<SkipListInternalNode*>(targetNode) -> right;
		}        
        if (targetNode -> matchesKey(key)) {
            return std::make_pair(targetNode, true);
        }
    }
   return std::make_pair(targetNode, false);
}

SkipListNode* SkipListInternalNode::update(std::string key, std::string value) {
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

SkipListNode* SkipListInternalNode::deleteBy(std::string key) {
    SkipListNode *previousNode  = nullptr;
    SkipListNode *targetNode    = this;

    for(; !targetNode -> isLeaf(); ) {
        while(static_cast<SkipListInternalNode*>(targetNode) -> right != nullptr && static_cast<SkipListInternalNode*>(targetNode) -> right -> isKeyLessEqualTo(key)) {
			previousNode = targetNode;
            targetNode = static_cast<SkipListInternalNode*>(targetNode) -> right;
		}        
        if (targetNode -> matchesKey(key)) {
            static_cast<SkipListInternalNode*>(previousNode) -> right = static_cast<SkipListInternalNode*>(targetNode) -> right;
            static_cast<SkipListInternalNode*>(targetNode) -> down    = nullptr;
            static_cast<SkipListInternalNode*>(targetNode) -> right   = nullptr;
            delete targetNode; //warning?
            targetNode = static_cast<SkipListInternalNode*>(previousNode) -> down;
        } else {
            targetNode = static_cast<SkipListInternalNode*>(targetNode) -> down;
        }
    }
   return targetNode;
}

std::pair<std::vector<SkipListNode*>, SkipListNode*> SkipListInternalNode::insertPositionsFor(std::string key) {
    std::vector<SkipListNode*> nodes;
    SkipListNode *targetNode = this;

    for(; !targetNode -> isLeaf(); targetNode = static_cast<SkipListInternalNode*>(targetNode) -> down) {
        while(static_cast<SkipListInternalNode*>(targetNode) -> right != nullptr && static_cast<SkipListInternalNode*>(targetNode) -> right -> isKeyLessEqualTo(key)) {
			targetNode = static_cast<SkipListInternalNode*>(targetNode) -> right;
		}        
        nodes.push_back(targetNode);
    }
   return std::make_pair(nodes, targetNode);
}

void SkipListInternalNode::updateValue(std::string value) {
    this -> value = value;
}