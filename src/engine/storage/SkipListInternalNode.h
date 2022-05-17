#ifndef _SkipListInternalNode_
#define _SkipListInternalNode_

#include <vector>
#include "SkipListNode.h"
#include "SkipListLeafNode.h"
#include "WritePosition.h"
#include "comparator/KeyComparator.h"
#include "utils/LevelGenerator.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListInternalNode : public SkipListNode {
            private:
                const char* key;
                size_t keySize;
                SkipListNode *down;
                std::vector<SkipListInternalNode *> forwards;

                int generateLevel(int maxLevel);

            public:
                SkipListInternalNode(const Slice& key, int level);
                ~SkipListInternalNode();

                bool matchesKey(const Slice& key, KeyComparator *keyComparator) const override;

                bool isKeyLessEqualTo(const Slice& key, KeyComparator *keyComparator) override;

                KeyValuePair keyValuePair() override;

                SkipListNode *getDown();

                void attach(SkipListLeafNode *down);

                SkipListInternalNode* next();

                std::pair<SkipListNode *, bool> getBy(const Slice& key, KeyComparator *keyComparator);

                std::pair<SkipListNode *, bool> scan(const Slice& beginKey, KeyComparator *keyComparator);

                PutPosition putPositionOf(const Slice& key, KeyComparator *keyComparator, LevelGenerator *levelGenerator);

                SkipListNode *
                put(const Slice& key,
                    const std::vector<SkipListInternalNode *>& positions,
                    int nodeLevel);

                UpdatePosition updatePositionOf(const Slice& key, KeyComparator *keyComparator);

                DeletePosition deletePositionOf(const Slice& key, KeyComparator *keyComparator);

                void deleteBy(const Slice& key,
                              const std::vector<SkipListInternalNode *>& positions,
                              int deleteLevel,
                              KeyComparator *keyComparator);

                unsigned long totalKeys();

                void close();
            };
        }
    }
}
#endif