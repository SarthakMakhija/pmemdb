#include "./SkipListNodeTestUtils.h"

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

pmem::storage::KeyComparator* stringKeyComparator() {
    return new pmem::storage::StringKeyComparator();
}