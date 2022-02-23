#ifndef _SkipListNodeTestUtils_
#define _SkipListNodeTestUtils_

#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"

KeyValuePair downKeyValuePairOf(SkipListInternalNode* node);
SkipListLeafNode* newSentinelLeafNode();
SkipListInternalNode* newSentinelInternalNode(int level);

#endif