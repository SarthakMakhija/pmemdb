#ifndef _SkipListIterator_
#define _SkipListIterator_

#include <string>
#include <functional>
#include "SkipListNode.h"
#include "Status.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListIterator {
            private:
                SkipListNode *startingNode;

            public:
                SkipListIterator(SkipListNode *startingNode);

                Status put(std::string key, std::string value, double probability,
                         std::function<void(void)> postPutHook = [] {});

                std::pair<std::string, bool> getBy(std::string key);

                std::vector <std::pair<std::string, bool>> multiGet(std::vector <std::string> keys);

                std::vector <KeyValuePair> scan(std::string beginKey, std::string endKey, int64_t maxPairs);

                Status update(std::string key, std::string value, std::function<void(void)> postUpdateHook = [] {});

                Status deleteBy(std::string key, std::function<void(void)> postDeleteHook = [] {});
            };
        }
    }
}
#endif