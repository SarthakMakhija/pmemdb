#include "KeyValuePair.h"

KeyValuePair::KeyValuePair(std::string key, std::string value) : key(key),  value(value) {
}

std::string KeyValuePair::getKey() const {
    return key;
}

std::string KeyValuePair::getValue() const {
    return value;
}

bool KeyValuePair::operator==(const KeyValuePair &other) const {
    return key == other.key && value == other.value;
}