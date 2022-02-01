#ifndef _SkipListNodeTestUtils_
#define _SkipListNodeTestUtils_

#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"

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

#endif