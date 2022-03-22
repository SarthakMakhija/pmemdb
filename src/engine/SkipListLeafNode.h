#ifndef _SkipListLeafNode_
#define _SkipListLeafNode_

#include <string>
#include <functional>
#include "SkipListNode.h"
#include "PersistentLeaf.h"
#include "Status.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListLeafNode : public SkipListNode {
            private:
                SkipListLeafNode *right;
                persistent_ptr<PersistentLeaf> leaf;

            public:
                SkipListLeafNode();

                void persist();

                bool isLeaf();

                bool matchesKey(std::string key) const;

                bool isKeyLessEqualTo(std::string key);

                pmem::storage::KeyValuePair keyValuePair();

                pmem::storage::KeyValuePair rightKeyValuePair();

                std::pair<SkipListLeafNode *, Status>
                put(std::string key, std::string value, std::function<void(void)> postPutHook = [] {});

                std::pair<std::string, bool> getBy(std::string key);

                std::vector <pmem::storage::KeyValuePair> scan(std::string beginKey, std::string endKey, int64_t maxPairs);

                Status update(std::string key, std::string value, std::function<void(void)> postUpdateHook = [] {});

                Status deleteBy(std::string key, std::function<void(void)> postDeleteHook = [] {});

                Status deleteRange(std::string beginKey, std::string endKey,
                                   std::function<void(void)> postDeleteRangeHook = [] {});
            };
        }
    }
}

#endif