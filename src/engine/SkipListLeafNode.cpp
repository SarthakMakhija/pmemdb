#include "SkipListLeafNode.h"
#include "KeyValuePair.h"
#include "PersistentMemoryPool.h"

SkipListLeafNode::SkipListLeafNode() : SkipListLeafNode("", "") {}

SkipListLeafNode::SkipListLeafNode(string key, string value) {
    this -> key     = key;
    this -> value   = value;
    this -> right   = nullptr;
}

void SkipListLeafNode::persist() {
    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
    
    transaction::run(pmpool, [&] {
        persistent_ptr<PersistentLeaf> newLeaf = make_persistent<PersistentLeaf>();
        this -> leaf = newLeaf;
        this -> leaf.get() -> put("", "");
    });
}

bool SkipListLeafNode::isLeaf() {
    return true;
}

bool SkipListLeafNode::matchesKey(string key) const {
    PersistentLeaf* leaf = this -> leaf.get();
    return string(leaf -> key()) == key;
}

bool SkipListLeafNode::isKeyLessEqualTo(string key) {
    PersistentLeaf* leaf = this -> leaf.get();
    return string(leaf -> key()) <= key;
}

KeyValuePair SkipListLeafNode::keyValuePair() {
    PersistentLeaf* leaf = this -> leaf.get();
    return KeyValuePair(string(leaf -> key()), string(leaf -> value()));
}

KeyValuePair SkipListLeafNode::rightKeyValuePair() {
    PersistentLeaf* right = this -> leaf -> right.get();
    if (right) {
        return KeyValuePair(string(right -> key()), string(right -> value()));
    }
    return KeyValuePair("", "");
}

void SkipListLeafNode::updateRight(SkipListLeafNode* right) {
    this -> right = right;
}

SkipListLeafNode* SkipListLeafNode::put(string key, string value) {
    PersistentLeaf* targetNode = this -> leaf.get();
    while(targetNode -> right.get() && string(targetNode -> right.get() -> key()) <= key) {
        targetNode = targetNode -> right.get();
    }
    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
    SkipListLeafNode* newNode   = new SkipListLeafNode("", "");
    
    transaction::run(pmpool, [&] {
        persistent_ptr<PersistentLeaf> newLeaf = make_persistent<PersistentLeaf>();
        newNode -> leaf = newLeaf;                
        newNode -> leaf.get() -> put(key, value); 
        
        newNode -> leaf.get() -> right = targetNode -> right;
        targetNode -> right = newLeaf;
    });

    return newNode;
}

pair<string, bool>  SkipListLeafNode::getBy(string key) {
    PersistentLeaf* targetNode = this -> leaf.get();
    while(targetNode -> right.get() && string(targetNode -> right.get() -> key()) <= key) {
        targetNode = targetNode -> right.get();
    }
    if (string(targetNode -> key()) == key) {
        return make_pair(string(targetNode -> value()), true);
    }
    return make_pair("", false);
}

void SkipListLeafNode::update(string key, string value) {
    PersistentLeaf* targetNode = this -> leaf.get();
    while(targetNode -> right.get() && string(targetNode -> right.get() -> key()) <= key) {
        targetNode = targetNode -> right.get();
    }

    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
    if (string(targetNode -> key()) == key) {
        transaction::run(pmpool, [&] {
            targetNode -> put(key, value);
        });
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