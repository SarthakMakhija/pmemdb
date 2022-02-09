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
    PersistentLeaf* targetLeaf   = this -> leaf.get();
    SkipListLeafNode* targetNode = this; 

    while(targetLeaf -> right.get() && std::string(targetLeaf -> right.get() -> key()) <= key) {
        targetLeaf = targetLeaf -> right.get();
        targetNode = targetNode -> right;
    }
    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
   
    SkipListLeafNode* newNode   = new SkipListLeafNode();
    newNode -> right            = targetNode -> right;
    targetNode -> right         = newNode;
    
    transaction::run(pmpool, [&] {
        persistent_ptr<PersistentLeaf> newLeaf = make_persistent<PersistentLeaf>();
        newNode -> leaf = newLeaf;                
        newNode -> leaf.get() -> put(key, value); 
        
        newNode -> leaf.get() -> right = targetLeaf -> right;
        targetLeaf -> right = newLeaf;
    });

    return newNode;
}

std::pair<std::string, bool>  SkipListLeafNode::getBy(std::string key) {
    PersistentLeaf* targetLeaf = this -> leaf.get();
    while(targetLeaf -> right.get() && std::string(targetLeaf -> right.get() -> key()) <= key) {
        targetLeaf = targetLeaf -> right.get();
    }
    if (std::string(targetLeaf -> key()) == key) {
        return std::make_pair(std::string(targetLeaf -> value()), true);
    }
    return std::make_pair("", false);
}

std::vector<KeyValuePair> SkipListLeafNode::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
    PersistentLeaf* targetLeaf = this -> leaf.get();
    while(targetLeaf -> right.get() && std::string(targetLeaf -> right.get() -> key()) <= beginKey) {
        targetLeaf = targetLeaf -> right.get();
    }
    if (std::string(targetLeaf -> key()) < beginKey) {
        targetLeaf = targetLeaf -> right.get();
    }

    std::vector<KeyValuePair> keyValuePairs;
    int64_t pairCount = 0;
    
    while(targetLeaf && std::string(targetLeaf -> key()) < endKey) {
        keyValuePairs.push_back(KeyValuePair(targetLeaf -> key(), targetLeaf -> value()));
        targetLeaf = targetLeaf -> right.get();
        pairCount = pairCount + 1;

        if (pairCount == maxPairs) {
            break;
        }
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

void SkipListLeafNode::deleteRange(std::string beginKey, std::string endKey) {
    PersistentLeaf* previousNode = nullptr;
    PersistentLeaf* targetNode   = this -> leaf.get();

    SkipListLeafNode* previousLeaf = nullptr;
    SkipListLeafNode* targetLeaf   = this;
    
    while(targetNode -> right.get() && std::string(targetNode -> right.get() -> key()) <= beginKey) {
        previousLeaf = targetLeaf;
        targetLeaf   = targetLeaf -> right;

        previousNode = targetNode;
        targetNode   = targetNode -> right.get();
    }
    if (std::string(targetNode -> key()) < beginKey) {
        previousLeaf = targetLeaf;
        targetLeaf   = targetLeaf -> right;

        previousNode = targetNode;
        targetNode   = targetNode -> right.get();
    }

    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
    transaction::run(pmpool, [&] {
        PersistentLeaf* previous = targetNode;
        SkipListLeafNode* p      = targetLeaf;

        while(targetNode && std::string(targetNode -> key()) < endKey) {
            targetNode -> clear();
            targetNode = targetNode -> right.get();
            previous -> right = nullptr;
            previous = targetNode;

            delete_persistent<PersistentLeaf>(targetLeaf -> leaf);
            targetLeaf -> leaf = nullptr;
            targetLeaf = targetLeaf -> right;
            p -> right = nullptr;
            p = targetLeaf;
        }
        previousNode -> right = targetNode;
        previousLeaf -> right = targetLeaf;
    });
}