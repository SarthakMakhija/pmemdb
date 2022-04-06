#ifndef _SkipListInternalNode_
#define _SkipListInternalNode_

#include <vector>
#include "SkipListNode.h"
#include "SkipListLeafNode.h"
#include "WritePosition.h"
#include "../comparator/KeyComparator.h"
#include "./utils/LevelGenerator.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListInternalNode : public SkipListNode {
            private:
                const char *key;
                SkipListNode *down;
                std::vector<SkipListInternalNode *> forwards;

                int generateLevel(int maxLevel);

            public:
                SkipListInternalNode(const char *key, int level);

                bool isLeaf();

                bool matchesKey(const char *key, KeyComparator *keyComparator) const;

                bool isKeyLessEqualTo(const char *key, KeyComparator *keyComparator);

                KeyValuePair keyValuePair();

                SkipListNode *getDown();

                void attach(SkipListLeafNode *down);

                std::pair<SkipListNode *, bool> getBy(const char *key, KeyComparator *keyComparator);

                std::pair<SkipListNode *, bool> scan(const char *beginKey, KeyComparator *keyComparator);

                PutPosition putPositionOf(const char *key, KeyComparator *keyComparator, LevelGenerator *levelGenerator);

                SkipListNode *
                put(const char *key,
                    const std::vector<SkipListInternalNode *>& positions,
                    int nodeLevel);

                UpdatePosition updatePositionOf(const char *key, KeyComparator *keyComparator);

                DeletePosition deletePositionOf(const char *key, KeyComparator *keyComparator);

                void deleteBy(const char *key,
                              const std::vector<SkipListInternalNode *>& positions,
                              int deleteLevel,
                              KeyComparator *keyComparator);

                unsigned long totalKeys();
            };
        }
    }
}
#endif