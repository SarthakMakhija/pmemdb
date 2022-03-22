#ifndef _SkipListIterator_
#define _SkipListIterator_

#include <string>
#include <functional>
#include "SkipListNode.h"

namespace pmem {
    namespace storage {
        namespace internal {
            class SkipListIterator {
            private:
                SkipListNode *startingNode;

            public:
                SkipListIterator(SkipListNode *startingNode);

                void put(std::string key, std::string value, double probability,
                         std::function<void(void)> postPutHook = [] {});

                std::pair<std::string, bool> getBy(std::string key);

                std::vector <std::pair<std::string, bool>> multiGet(std::vector <std::string> keys);

                std::vector <KeyValuePair> scan(std::string beginKey, std::string endKey, int64_t maxPairs);

                void update(std::string key, std::string value, std::function<void(void)> postUpdateHook = [] {});

                void deleteBy(std::string key, std::function<void(void)> postDeleteHook = [] {});

                void deleteRange(std::string beginKey, std::string endKey,
                                 std::function<void(void)> postDeleteRangeHook = [] {});
            };
        }
    }
}
#endif