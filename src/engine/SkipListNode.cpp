#include "SkipListNode.h"
#include <iostream>

SkipListNode::SkipListNode() : SkipListNode("", "") {}

SkipListNode::SkipListNode(string  key, string value) {
    this -> key = key;
    this -> value = value;
}

bool SkipListNode::matchesKey(string key) const {
    return this -> key == key;
}

bool SkipListNode::isKeyLessEqualTo(string key) {
    return this -> key <= key;
}

void SkipListNode::updateValue(string value) {
    this -> value = value;
}

KeyValuePair SkipListNode::getKeyValuePair() {
    return KeyValuePair(this -> key, this -> value);
}

SkipListNode* SkipListNode::addToRightWith(string key, string value) {
    return nullptr;
}