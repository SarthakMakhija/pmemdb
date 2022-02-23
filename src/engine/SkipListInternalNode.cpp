#include "SkipListInternalNode.h"
#include "KeyValuePair.h"

SkipListInternalNode::SkipListInternalNode(std::string key, std::string value, int level) {
    this -> key     = key;
    this -> value   = value;
    this -> down    = nullptr;

    this -> forwards.resize(level);
    for (int index = 0; index < level; index++) {
        this -> forwards[index] = nullptr;
    }
}

bool SkipListInternalNode::isLeaf() {
    return false;
}

bool SkipListInternalNode::matchesKey(std::string key) const {
    return this -> key == key;
}

bool SkipListInternalNode::isKeyLessEqualTo(std::string key) {
    return this -> key <= key;
}

bool SkipListInternalNode::isKeyLessThan(std::string key) {
    return this -> key < key;
}

bool SkipListInternalNode::isKeyGreaterEqualTo(std::string key) {
    return this -> key >= key;
}

KeyValuePair SkipListInternalNode::keyValuePair() {
    return KeyValuePair(this -> key, this -> value);
}

SkipListNode* SkipListInternalNode::getDown()  {
    return this -> down;
}

void SkipListInternalNode::attach(SkipListLeafNode* down) {
    this -> down = down;
}

std::pair<SkipListNode*, bool> SkipListInternalNode::getBy(std::string key) {
    SkipListInternalNode* current = this;
    for(int level = this -> forwards.size()-1; level >= 0; level--) {
       while(current -> forwards[level] && current -> forwards[level] -> key < key) {
           current = current -> forwards[level];
       }
    }
    current = current -> forwards[0];
    if (current && current -> key == key) {
       return std::make_pair(current, true);
    }
    return std::make_pair(nullptr, false);
}

std::vector<KeyValuePair> SkipListInternalNode::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
    SkipListInternalNode* current = this;
    for(int level = this -> forwards.size()-1; level >= 0; level--) {
       while(current -> forwards[level] && current -> forwards[level] -> key <= beginKey) {
           current = current -> forwards[level];
       }
    }
    if (current -> key < beginKey) {
        current = current -> forwards[0];
    }
    std::vector<KeyValuePair> keyValuePairs;
    int64_t pairCount = 0;
    
    while(current && current -> key < endKey) {
        keyValuePairs.push_back(KeyValuePair(current -> key, current -> value));
        current = current -> forwards[0];
        pairCount = pairCount + 1;

        if (pairCount == maxPairs) {
            break;
        }
    }
    return keyValuePairs;
}

std::pair<SkipListNode*, SkipListNode*> SkipListInternalNode::put(std::string key, std::string value, double probability) {
    SkipListInternalNode* current = this;
    std::vector<SkipListInternalNode*> positions(this -> forwards.size(), nullptr);

    for(int level = this -> forwards.size()-1; level >= 0; level--) {
       while(current -> forwards[level] && current -> forwards[level] -> key < key) {
           current = current -> forwards[level];
       }
       positions[level] = current; 
    }
    current = current -> forwards[0];
    if (current == nullptr || current -> key != key) {
        int newLevel = generateLevel(this -> forwards.size(), probability);
        SkipListInternalNode* newNode = new SkipListInternalNode(key, value, newLevel);

        for (int level = 0; level < newLevel; level++) {
            newNode -> forwards[level] = positions[level] -> forwards[level];
            positions[level] -> forwards[level] = newNode;
        }
        return std::make_pair(newNode, positions[0] -> down);
    }
    return std::make_pair(nullptr, nullptr);
}

SkipListNode* SkipListInternalNode::update(std::string key, std::string value) {
    SkipListInternalNode* current = this;
    for(int level = this -> forwards.size()-1; level >= 0; level--) {
       while(current -> forwards[level] && current -> forwards[level] -> key < key) {
           current = current -> forwards[level];
       }
    }
    current = current -> forwards[0];
    if (current && current -> key == key) {
        current -> updateValue(value);
        return current -> down;
    }
    return nullptr;
}

SkipListNode* SkipListInternalNode::deleteBy(std::string key) {
    SkipListInternalNode* current = this;
    std::vector<SkipListInternalNode*> positions(this -> forwards.size(), nullptr);

    for(int level = this -> forwards.size()-1; level >= 0; level--) {
       while(current -> forwards[level] && current -> forwards[level] -> key < key) {
           current = current -> forwards[level];
       }
       positions[level] = current; 
    }
    current = current -> forwards[0];
    if (current != nullptr && current -> key == key) {
         for (int level = 0; level < this -> forwards.size(); level++) {
            if (positions[level] -> forwards[level] != current) {
                break;
            }
            positions[level] -> forwards[level] = current -> forwards[level];
        }
        current -> down = nullptr;
        for (int level = 0; level < current -> forwards.size(); level++) {
            current -> forwards[level] = nullptr;
        }
        delete current;
        return positions[0] -> down;
    }
    return nullptr;
}

SkipListNode* SkipListInternalNode::deleteRange(std::string beginKey, std::string endKey) {
    SkipListInternalNode* current = this;
    std::vector<SkipListInternalNode*> positions(this -> forwards.size(), nullptr);

    for(int level = this -> forwards.size()-1; level >= 0; level--) {
       while(current -> forwards[level] && current -> forwards[level] -> key < beginKey) {
           current = current -> forwards[level];
       }
       positions[level] = current; 
    }
    current = current -> forwards[0];
    while (current != nullptr && current -> key >= beginKey && current -> key < endKey) {
         for (int level = 0; level < this -> forwards.size(); level++) {
            if (positions[level] -> forwards[level] != current) {
                break;
            }
            positions[level] -> forwards[level] = current -> forwards[level];
        }
        current -> down = nullptr;
        SkipListInternalNode* backup = current -> forwards[0];
        for (int level = 0; level < current -> forwards.size(); level++) {
            current -> forwards[level] = nullptr;
        }
        delete current;
        current = backup;
    }
    return positions[0] -> down;
}

void SkipListInternalNode::updateValue(std::string value) {
    this -> value = value;
}

int SkipListInternalNode::generateLevel(int maxLevel, double probability) { 
    double random = (double)rand()/RAND_MAX;
    int level = 1;

    while (random < probability && level < maxLevel) {
        level = level + 1;
        random = (double)rand()/RAND_MAX;
    }
    return level;
}