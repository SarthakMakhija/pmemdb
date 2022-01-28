#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"

KeyValuePair downKeyValuePairOf(SkipListInternalNode* node) {
    if (node != nullptr)  {
        if (!node -> getDown() -> isLeaf()) {
            return static_cast<SkipListInternalNode*>(node -> getDown()) -> getKeyValuePair();
        } else {
            return static_cast<SkipListLeafNode*>(node -> getDown()) -> getKeyValuePair();
        }
    }
    return KeyValuePair("", "");
}