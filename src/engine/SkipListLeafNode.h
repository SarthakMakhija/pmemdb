#ifndef _SkipListLeafNode_
#define _SkipListLeafNode_

#include <string>
#include "SkipListNode.h"
#include "PersistentLeaf.h"

class SkipListLeafNode : public SkipListNode {   
    private:
    SkipListLeafNode* right;
    persistent_ptr<PersistentLeaf> leaf;

    public:
    SkipListLeafNode();
    
    void persist();
    bool isLeaf();
    bool matchesKey(std::string key) const;
    bool isKeyLessEqualTo(std::string key);
    KeyValuePair keyValuePair();
    KeyValuePair rightKeyValuePair();
    
    SkipListLeafNode* put(std::string key, std::string value);
    std::pair<std::string, bool> getBy(std::string key);
    std::vector<KeyValuePair> scan(std::string beginKey, std::string endKey);
    void update(std::string key, std::string value);
    void deleteBy(std::string key);
};

#endif