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

bool SkipListLeafNode::matchesKey(std::string key) const {
    PersistentLeaf* leaf = this -> leaf.get();
    return std::string(leaf -> key()) == key;
}

bool SkipListLeafNode::isKeyLessEqualTo(std::string key) {
    PersistentLeaf* leaf = this -> leaf.get();
    return std::string(leaf -> key()) <= key;
}

KeyValuePair SkipListLeafNode::keyValuePair() {
    PersistentLeaf* leaf = this -> leaf.get();
    return KeyValuePair(std::string(leaf -> key()), std::string(leaf -> value()));
}

KeyValuePair SkipListLeafNode::rightKeyValuePair() {
    PersistentLeaf* right = this -> leaf -> right.get();
    if (right) {
        return KeyValuePair(std::string(right -> key()), std::string(right -> value()));
    }
    return KeyValuePair("", "");
}

SkipListLeafNode* SkipListLeafNode::put(std::string key, std::string value) {
    PersistentLeaf* targetNode   = this -> leaf.get();
    SkipListLeafNode* targetLeaf = this; 

    while(targetNode -> right.get() && std::string(targetNode -> right.get() -> key()) <= key) {
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

std::pair<std::string, bool>  SkipListLeafNode::getBy(std::string key) {
    PersistentLeaf* targetNode = this -> leaf.get();
    while(targetNode -> right.get() && std::string(targetNode -> right.get() -> key()) <= key) {
        targetNode = targetNode -> right.get();
    }
    if (std::string(targetNode -> key()) == key) {
        return std::make_pair(std::string(targetNode -> value()), true);
    }
    return std::make_pair("", false);
}

std::vector<KeyValuePair> SkipListLeafNode::scan(std::string beginKey, std::string endKey) {
    PersistentLeaf* targetNode = this -> leaf.get();
    while(targetNode -> right.get() && std::string(targetNode -> right.get() -> key()) <= beginKey) {
        targetNode = targetNode -> right.get();
    }
    if (std::string(targetNode -> key()) < beginKey) {
        targetNode = targetNode -> right.get();
    }
    std::vector<KeyValuePair> keyValuePairs;
    while(targetNode && std::string(targetNode -> key()) < endKey) {
        keyValuePairs.push_back(KeyValuePair(targetNode -> key(), targetNode -> value()));
        targetNode = targetNode -> right.get();
    }
    return keyValuePairs;
}

void SkipListLeafNode::update(std::string key, std::string value) {
    PersistentLeaf* targetNode = this -> leaf.get();
    while(targetNode -> right.get() && std::string(targetNode -> right.get() -> key()) <= key) {
        targetNode = targetNode -> right.get();
    }

    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
    if (std::string(targetNode -> key()) == key) {
        transaction::run(pmpool, [&] {
            targetNode -> put(key, value);
        });
    }
}

void SkipListLeafNode::deleteBy(std::string key) {
    PersistentLeaf* previousNode = nullptr;
    PersistentLeaf* targetNode   = this -> leaf.get();

    SkipListLeafNode* previousLeaf = nullptr;
    SkipListLeafNode* targetLeaf   = this;

    while(targetNode -> right.get() && std::string(targetNode -> right.get() -> key()) <= key) {
        previousLeaf = targetLeaf;
        targetLeaf   = targetLeaf -> right;

        previousNode = targetNode;
        targetNode   = targetNode -> right.get();
    }

    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
    if (std::string(targetNode -> key()) == key) {
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