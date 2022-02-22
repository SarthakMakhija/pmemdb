#include "./SkipListNodeTestUtils.h"

KeyValuePair downKeyValuePairOf(SkipListInternalNode* node) {
    if (node != nullptr)  {
        if (!node -> getDown() -> isLeaf()) {
            return static_cast<SkipListInternalNode*>(node -> getDown()) -> keyValuePair();
        } else {
            return static_cast<SkipListLeafNode*>(node -> getDown()) -> keyValuePair();
        }
    }
    return KeyValuePair("", "");
}

SkipListLeafNode* newSentinelLeafNode() {
    SkipListLeafNode* sentinel = new SkipListLeafNode();
    sentinel -> persist();

    return sentinel;
}

SkipListInternalNode* newSentinelInternalNode(int level) {
    SkipListInternalNode* node     = new SkipListInternalNode("", "", level);
    SkipListLeafNode* sentinelNode = new SkipListLeafNode();
    sentinelNode -> persist();
    node -> attach(sentinelNode);

    return node;
}