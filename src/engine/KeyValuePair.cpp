#include "KeyValuePair.h"

KeyValuePair::KeyValuePair(string key, string value) : key(key),  value(value) {
}

string KeyValuePair::getKey() const {
    return key;
}

string KeyValuePair::getValue() const {
    return value;
}

bool KeyValuePair::operator==(const KeyValuePair &other) const {
    return key == other.key && value == other.value;
}