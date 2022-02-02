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
    bool matchesKey(string key) const;
    bool isKeyLessEqualTo(string key);
    KeyValuePair keyValuePair();
    KeyValuePair rightKeyValuePair();
    
    SkipListLeafNode* put(string key, string value);
    pair<string, bool> getBy(string key);
    void update(string key, string value);
    void deleteBy(string key);
};

#endif