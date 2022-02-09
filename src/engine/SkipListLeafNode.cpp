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
    PersistentLeaf* targetLeaf = this -> leaf.get();
    while(targetLeaf -> right.get() && std::string(targetLeaf -> right.get() -> key()) <= key) {
        targetLeaf = targetLeaf -> right.get();
    }

    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
    if (std::string(targetLeaf -> key()) == key) {
        transaction::run(pmpool, [&] {
            targetLeaf -> put(key, value);
        });
    }
}

void SkipListLeafNode::deleteBy(std::string key) {
    PersistentLeaf* previousLeaf = nullptr;
    PersistentLeaf* targetLeaf   = this -> leaf.get();

    SkipListLeafNode* previousNode = nullptr;
    SkipListLeafNode* targetNode   = this;

    while(targetLeaf -> right.get() && std::string(targetLeaf -> right.get() -> key()) <= key) {
        previousNode = targetNode;
        targetNode   = targetNode -> right;

        previousLeaf = targetLeaf;
        targetLeaf   = targetLeaf -> right.get();
    }

    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
    if (std::string(targetLeaf -> key()) == key) {
        previousNode -> right = targetNode -> right;
        targetNode -> right   = nullptr;

        transaction::run(pmpool, [&] {
            targetLeaf -> clear();
            previousLeaf -> right = targetLeaf -> right;
            targetLeaf -> right   = nullptr;
            
            delete_persistent<PersistentLeaf>(targetNode -> leaf);
            targetNode -> leaf = nullptr;
        });
        delete targetNode;
    }   
}

void SkipListLeafNode::deleteRange(std::string beginKey, std::string endKey) {
    PersistentLeaf* previousLeaf = nullptr;
    PersistentLeaf* targetLeaf   = this -> leaf.get();

    SkipListLeafNode* previousNode = nullptr;
    SkipListLeafNode* targetNode   = this;
    
    while(targetLeaf -> right.get() && std::string(targetLeaf -> right.get() -> key()) <= beginKey) {
        previousNode = targetNode;
        targetNode   = targetNode -> right;

        previousLeaf = targetLeaf;
        targetLeaf   = targetLeaf -> right.get();
    }
    if (std::string(targetLeaf -> key()) < beginKey) {
        previousNode = targetNode;
        targetNode   = targetNode -> right;

        previousLeaf = targetLeaf;
        targetLeaf   = targetLeaf -> right.get();
    }

    pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance() -> getPmpool();
    transaction::run(pmpool, [&] {
        PersistentLeaf* followerLeaf    = targetLeaf;
        SkipListLeafNode* followerNode  = targetNode;

        while(targetLeaf && std::string(targetLeaf -> key()) < endKey) {
            targetLeaf -> clear();
            targetLeaf = targetLeaf -> right.get();
            followerLeaf -> right = nullptr;
            followerLeaf = targetLeaf;

            delete_persistent<PersistentLeaf>(targetNode -> leaf);
            targetNode -> leaf    = nullptr;
            targetNode            = targetNode -> right;
            followerNode -> right = nullptr;
            followerNode          = targetNode;
        }
        previousLeaf -> right = targetLeaf;
        previousNode -> right = targetNode;
    });
}