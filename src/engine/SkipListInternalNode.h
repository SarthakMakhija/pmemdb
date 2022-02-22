#ifndef _SkipListInternalNode_
#define _SkipListInternalNode_

#include <string>
#include <vector>
#include "SkipListNode.h"
#include "SkipListLeafNode.h"


class SkipListInternalNode : public SkipListNode {
    private:
    std::string key;
    std::string value;
    SkipListNode* down;
    std::vector<SkipListInternalNode*> forwards;

    void updateValue(std::string value);

    public:
    SkipListInternalNode(std::string key, std::string value, int level);

    bool isLeaf();
    bool matchesKey(std::string key) const;
    bool isKeyLessEqualTo(std::string key);
    bool isKeyLessThan(std::string key);
    bool isKeyGreaterEqualTo(std::string key);

    KeyValuePair keyValuePair();

    SkipListNode* getDown();
    void attach(SkipListLeafNode* down);

    std::pair<SkipListNode*, bool> getBy(std::string key);
    std::vector<KeyValuePair> scan(std::string beginKey, std::string endKey, int64_t maxPairs);
    std::pair<SkipListNode*, SkipListNode*> put(std::string key, std::string value);
    SkipListNode* update(std::string key, std::string value);
    SkipListNode* deleteBy(std::string key);
    SkipListNode* deleteRange(std::string beginKey, std::string endKey);
};

#endif