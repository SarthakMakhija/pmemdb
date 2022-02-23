#ifndef _SkipListNodeTestUtils_
#define _SkipListNodeTestUtils_

#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"
#include "../../src/engine/SkipListNodeLevelGenerator.h"

KeyValuePair downKeyValuePairOf(SkipListInternalNode* node);
SkipListLeafNode* newSentinelLeafNode();
SkipListInternalNode* newSentinelInternalNode(int level);
void initializeSkipListNodeLevelGenerator(int maxLeve, double probability);
void cleanUpSkipListNodeLevelGenerator();

#endif