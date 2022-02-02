#include "SkipListLeafNode.h"
#include "KeyValuePair.h"
#include "PersistentMemoryPool.h"

SkipListLeafNode::SkipListLeafNode(){
     this -> right = nullptr;
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

SkipListLeafNode* SkipListLeafNode::put(string key, string value) {
    PersistentLeaf* targetNode   = this -> leaf.get();
    SkipListLeafNode* targetLeaf = this; 

    while(targetNode -> right.get() && string(targetNode -> right.get() -> key()) <= key) {
        targetNode = targetNode -> right.get();
        targetLeaf = targetLeaf -> right;
    }
    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
   
    SkipListLeafNode* newNode   = new SkipListLeafNode();
    newNode -> right            = targetLeaf -> right;
    targetLeaf -> right         = newNode;
    
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
    PersistentLeaf* previousNode = nullptr;
    PersistentLeaf* targetNode   = this -> leaf.get();

    SkipListLeafNode* previousLeaf = nullptr;
    SkipListLeafNode* targetLeaf   = this;

    while(targetNode -> right.get() && string(targetNode -> right.get() -> key()) <= key) {
        previousLeaf = targetLeaf;
        targetLeaf   = targetLeaf -> right;

        previousNode = targetNode;
        targetNode   = targetNode -> right.get();
    }

    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
    if (string(targetNode -> key()) == key) {
        previousLeaf -> right = targetLeaf -> right;
        targetLeaf -> right   = nullptr;

        transaction::run(pmpool, [&] {
            targetNode -> clear();
            previousNode -> right = targetNode -> right;
            targetNode -> right   = nullptr;
            
            delete_persistent<PersistentLeaf>(targetLeaf -> leaf);
            targetLeaf -> leaf = nullptr;
        });
    }   
}