#ifndef _SkipListInternalNode_
#define _SkipListInternalNode_

#include <string>
#include <vector>
#include "SkipListNode.h"
#include "SkipListLeafNode.h"
#include "WritePosition.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListInternalNode : public SkipListNode {
            private:
                const char* key;
                SkipListNode *down;
                std::vector<SkipListInternalNode *> forwards;

                int generateLevel(int maxLevel, double probability);

            public:
                SkipListInternalNode(const char* key, int level);

                bool isLeaf();

                bool matchesKey(const char* key) const;

                bool isKeyLessEqualTo(const char* key);

                bool isKeyLessThan(std::string key);

                bool isKeyGreaterEqualTo(std::string key);

                KeyValuePair keyValuePair();

                SkipListNode *getDown();

                void attach(SkipListLeafNode *down);

                std::pair<SkipListNode *, bool> getBy(const char* key);

                std::pair<SkipListNode *, bool> scan(const char* beginKey);

                PutPosition putPositionOf(const char* key, double withProbability);

                SkipListNode *
                put(const char* key, std::vector<SkipListInternalNode *> positions, int nodeLevel);

                UpdatePosition updatePositionOf(const char* key);

                DeletePosition deletePositionOf(const char* key);

                void deleteBy(const char* key, std::vector<SkipListInternalNode *> positions, int deleteLevel);
            };
        }
    }
}
#endif