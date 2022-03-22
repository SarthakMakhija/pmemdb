#ifndef _SkipListNodeTestUtils_
#define _SkipListNodeTestUtils_

#include "../../src/engine/SkipListInternalNode.h"
#include "../../src/engine/SkipListLeafNode.h"
#include "../../src/engine/KeyValuePair.h"


pmem::storage::KeyValuePair downKeyValuePairOf(pmem::storage::internal::SkipListInternalNode* node);
pmem::storage::internal::SkipListLeafNode* newSentinelLeafNode();
pmem::storage::internal::SkipListInternalNode* newSentinelInternalNode(int level);

#endif