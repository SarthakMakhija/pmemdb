#include "SkipListNodes.h"

void SkipListNodes::add(SkipListNode* node) {
    nodes.push_back(node);
}

void SkipListNodes::addAll(std::vector<SkipListNode*> const &vector) {
    nodes.insert(nodes.end(), vector.begin(), vector.end());
}

bool SkipListNodes::isEmpty() {
    return nodes.size() == 0;
}

SkipListNode* SkipListNodes::pop() {
    SkipListNode* node = nodes.back();
    nodes.pop_back();
    return node;
}