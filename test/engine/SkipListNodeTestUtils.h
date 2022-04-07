#ifndef _SkipListNodeTestUtils_
#define _SkipListNodeTestUtils_

#include "../../src/engine/storage/SkipListInternalNode.h"
#include "../../src/engine/storage/SkipListLeafNode.h"
#include "../../src/engine/db/KeyValuePair.h"
#include "../../src/engine/comparator/StringKeyComparator.h"

pmem::storage::KeyComparator* stringKeyComparator();
pmem::storage::internal::SkipListLeafNode* newSentinelLeafNode();
pmem::storage::internal::SkipListInternalNode* newSentinelInternalNode(int level);

#endif