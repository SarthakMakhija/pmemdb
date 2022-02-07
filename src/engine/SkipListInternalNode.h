#ifndef _SkipListInternalNode_
#define _SkipListInternalNode_

#include <string>
#include <vector>
#include "SkipListNode.h"

class SkipListInternalNode : public SkipListNode {
    private:
    std::string key;
    std::string value;
    SkipListInternalNode* right;
    SkipListNode* down;

    void updateValue(std::string value);

    public:
    SkipListInternalNode(std::string key, std::string value);
    SkipListInternalNode();

    bool isLeaf();
    SkipListNode* addToRightWith(std::string key, std::string value);
    bool matchesKey(std::string key) const;
    bool isKeyLessEqualTo(std::string key);    
    KeyValuePair keyValuePair();
    KeyValuePair rightKeyValuePair();

    SkipListNode* getDown();
    void updateDown(SkipListNode* down);
    void updateRight(SkipListInternalNode* right);    
    
    std::pair<SkipListNode*, bool> getBy(std::string key);
    SkipListNode* scan(std::string beginKey);
    SkipListNode* update(std::string key, std::string value);
    SkipListNode* deleteBy(std::string key);
    std::pair<std::vector<SkipListNode*>, SkipListNode*> insertPositionsFor(std::string key);
};

#endif