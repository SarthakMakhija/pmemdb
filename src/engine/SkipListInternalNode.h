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
                std::string key;
                std::string value;
                SkipListNode *down;
                std::vector<SkipListInternalNode *> forwards;

                void updateValue(std::string value);

                int generateLevel(int maxLevel, double probability);

            public:
                SkipListInternalNode(std::string key, std::string value, int level);

                bool isLeaf();

                bool matchesKey(std::string key) const;

                bool isKeyLessEqualTo(std::string key);

                bool isKeyLessThan(std::string key);

                bool isKeyGreaterEqualTo(std::string key);

                KeyValuePair keyValuePair();

                SkipListNode *getDown();

                void attach(SkipListLeafNode *down);

                std::pair<SkipListNode *, bool> getBy(std::string key);

                std::vector <KeyValuePair> scan(std::string beginKey, std::string endKey, int64_t maxPairs);

                PutPosition putPositionOf(std::string key, double withProbability);

                SkipListNode *
                put(std::string key, std::string value, std::vector<SkipListInternalNode *> positions, int nodeLevel);

                UpdatePosition updatePositionOf(std::string key);

                void update(std::string key, std::string value);

                DeletePosition deletePositionOf(std::string key);

                void deleteBy(std::string key, std::vector<SkipListInternalNode *> positions, int deleteLevel);
            };
        }
    }
}
#endif