#include "./SkipListNodeTestUtils.h"

pmem::storage::KeyValuePair downKeyValuePairOf(pmem::storage::internal::SkipListInternalNode* node) {
    if (node != nullptr)  {
        if (!node -> getDown() -> isLeaf()) {
            return static_cast<pmem::storage::internal::SkipListInternalNode*>(node -> getDown()) -> keyValuePair();
        } else {
            return static_cast<pmem::storage::internal::SkipListLeafNode*>(node -> getDown()) -> keyValuePair();
        }
    }
    return pmem::storage::KeyValuePair("", "");
}

pmem::storage::internal::SkipListLeafNode* newSentinelLeafNode() {
    auto sentinel = new pmem::storage::internal::SkipListLeafNode();
    sentinel -> persist();

    return sentinel;
}

pmem::storage::internal::SkipListInternalNode* newSentinelInternalNode(int level) {
    pmem::storage::internal::SkipListInternalNode* node     = new pmem::storage::internal::SkipListInternalNode("", level);
    pmem::storage::internal::SkipListLeafNode* sentinelNode = new pmem::storage::internal::SkipListLeafNode();
    sentinelNode -> persist();
    node -> attach(sentinelNode);

    return node;
}