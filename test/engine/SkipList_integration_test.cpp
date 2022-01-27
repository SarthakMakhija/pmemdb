#include <gtest/gtest.h>
#include <string>
#include "../../src/engine/SkipList.h"

TEST(SkipListIntegration, CreateASkipListAndAdd1000KeyValuePairs) {
    SkipList* skipList = new SkipList(10);
    for (int count = 1; count <= 1000; count++) {
        string key   = "Key-"    + std::to_string(count);
        string value = "Value-"  + std::to_string(count);

        skipList -> put(key, value);
    }
    for (int count = 1; count <= 1000; count++) {
        string key           = "Key-"   + std::to_string(count);
        string expectedValue = "Value-" + std::to_string(count);
        
        pair<string, bool> existenceByValue = skipList -> get(key);
        ASSERT_EQ(expectedValue, existenceByValue.first);
    }
}
