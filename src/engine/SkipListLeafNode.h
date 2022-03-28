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

                bool matchesKey(const char* key) const;

                bool isKeyLessEqualTo(const char* key);

                pmem::storage::KeyValuePair keyValuePair();

                pmem::storage::KeyValuePair rightKeyValuePair();

                std::pair<SkipListLeafNode *, Status>
                put(const char* key, const char* value, std::function<void(void)> postPutHook = [] {});

                std::pair<std::string, bool> getBy(const char* key);

                std::vector <pmem::storage::KeyValuePair> scan(const char* beginKey, const char* endKey, int64_t maxPairs);

                Status update(const char* key, const char* value, std::function<void(void)> postUpdateHook = [] {});

                Status deleteBy(const char* key, std::function<void(void)> postDeleteHook = [] {});
            };
        }
    }
}

#endif