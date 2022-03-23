#include "SkipListInternalNode.h"
#include "KeyValuePair.h"

namespace pmem {
    namespace storage {
        namespace internal {
            SkipListInternalNode::SkipListInternalNode(std::string key, std::string value, int level) {
                this->key = key;
                this->value = value;
                this->down = nullptr;

                this->forwards.resize(level);
                for (int index = 0; index < level; index++) {
                    this->forwards[index] = nullptr;
                }
            }

            bool SkipListInternalNode::isLeaf() {
                return false;
            }

            bool SkipListInternalNode::matchesKey(std::string key) const {
                return this->key == key;
            }

            bool SkipListInternalNode::isKeyLessEqualTo(std::string key) {
                return this->key <= key;
            }

            bool SkipListInternalNode::isKeyLessThan(std::string key) {
                return this->key < key;
            }

            bool SkipListInternalNode::isKeyGreaterEqualTo(std::string key) {
                return this->key >= key;
            }

            KeyValuePair SkipListInternalNode::keyValuePair() {
                return KeyValuePair(this->key, this->value);
            }

            SkipListNode *SkipListInternalNode::getDown() {
                return this->down;
            }

            void SkipListInternalNode::attach(SkipListLeafNode *down) {
                this->down = down;
            }

            std::pair<SkipListNode *, bool> SkipListInternalNode::getBy(std::string key) {
                SkipListInternalNode *current = this;
                for (int level = this->forwards.size() - 1; level >= 0; level--) {
                    while (current->forwards[level] && current->forwards[level]->key < key) {
                        current = current->forwards[level];
                    }
                }
                current = current->forwards[0];

                if (current && current->key == key) {
                    return std::make_pair(current -> down, true);
                }
                return std::make_pair(nullptr, false);
            }

            std::pair<SkipListNode *, bool> SkipListInternalNode::scan(std::string beginKey) {
                SkipListInternalNode *current = this;
                for (int level = this->forwards.size() - 1; level >= 0; level--) {
                    while (current->forwards[level] && current->forwards[level]->key <= beginKey) {
                        current = current->forwards[level];
                    }
                }
                if (current && current -> key < beginKey) {
                    current = current -> forwards[0];
                }

                if (current != nullptr) {
                    return std::make_pair(current -> down, true);
                }
                return std::make_pair(nullptr, false);
            }

            PutPosition SkipListInternalNode::putPositionOf(std::string key, double withProbability) {
                SkipListInternalNode *current = this;
                std::vector < SkipListInternalNode * > positions(this->forwards.size(), nullptr);

                for (int level = this->forwards.size() - 1; level >= 0; level--) {
                    while (current->forwards[level] && current->forwards[level]->key < key) {
                        current = current->forwards[level];
                    }
                    positions[level] = current;
                }
                current = current->forwards[0];
                if (current == nullptr || current->key != key) {
                    int newLevel = generateLevel(this->forwards.size(), withProbability);
                    return PutPosition{positions, newLevel, this, positions[0]->down};
                }
                std::vector < SkipListInternalNode * > empty;
                return PutPosition{empty, -1, nullptr, nullptr};
            }

            SkipListNode *SkipListInternalNode::put(std::string key,
                                                    std::string value,
                                                    std::vector<SkipListInternalNode *> positions,
                                                    int nodeLevel) {

                SkipListInternalNode *newNode = new SkipListInternalNode(key, value, nodeLevel);
                for (int level = 0; level < nodeLevel; level++) {
                    newNode->forwards[level] = positions[level]->forwards[level];
                    positions[level]->forwards[level] = newNode;
                }
                return newNode;
            }

            UpdatePosition SkipListInternalNode::updatePositionOf(std::string key) {
                SkipListInternalNode *current = this;
                for (int level = this->forwards.size() - 1; level >= 0; level--) {
                    while (current->forwards[level] && current->forwards[level]->key < key) {
                        current = current->forwards[level];
                    }
                }
                current = current->forwards[0];
                if (current && current->key == key) {
                    return UpdatePosition{current, current->down};
                }
                return UpdatePosition{nullptr, nullptr};
            }

            DeletePosition SkipListInternalNode::deletePositionOf(std::string key) {
                SkipListInternalNode *current = this;
                std::vector < SkipListInternalNode * > positions(this->forwards.size(), nullptr);

                for (int level = this->forwards.size() - 1; level >= 0; level--) {
                    while (current->forwards[level] && current->forwards[level]->key < key) {
                        current = current->forwards[level];
                    }
                    positions[level] = current;
                }
                current = current->forwards[0];
                if (current != nullptr && current->key == key) {
                    return DeletePosition{positions, (int) this->forwards.size(), current, positions[0]->down};
                }
                return DeletePosition{positions, -1, nullptr, nullptr};
            }

            void SkipListInternalNode::deleteBy(std::string key,
                                                std::vector<SkipListInternalNode *> positions,
                                                int deleteLevel) {

                SkipListInternalNode *current = this;
                if (current != nullptr && current->key == key) {
                    for (int level = 0; level < deleteLevel; level++) {
                        if (positions[level]->forwards[level] != current) {
                            break;
                        }
                        positions[level]->forwards[level] = current->forwards[level];
                    }
                    current->down = nullptr;
                    for (int level = 0; level < current->forwards.size(); level++) {
                        current->forwards[level] = nullptr;
                    }
                    delete current;
                }
            }

            int SkipListInternalNode::generateLevel(int maxLevel, double probability) {
                double random = (double) rand() / RAND_MAX;
                int level = 1;

                while (random < probability && level < maxLevel) {
                    level = level + 1;
                    random = (double) rand() / RAND_MAX;
                }
                return level;
            }
        }
    }
}