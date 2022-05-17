#include "SkipListInternalNode.h"

namespace pmem {
    namespace storage {
        namespace internal {
            SkipListInternalNode::SkipListInternalNode(const Slice& key, int level) {
                this->key       = key.cdata();
                this->keySize   = key.size();
                this->down      = nullptr;

                this->forwards.resize(level);
                for (int index = 0; index < level; index++) {
                    this->forwards[index] = nullptr;
                }
            }

            SkipListInternalNode::~SkipListInternalNode() {
                for (int index = 0; index < this->forwards.size(); index++) {
                    this->forwards[index] = nullptr;
                }
                this->forwards.clear();
                delete static_cast<SkipListLeafNode*>(this->down);
                //TODO: Need to figure out the issue where deleting this char* causes a delete issue in Valgrind
                //delete this->key;
            }

            bool SkipListInternalNode::matchesKey(const char *key, KeyComparator *keyComparator) const {
                return keyComparator->compare(this->key, Slice(key)) == 0;
            }

            bool SkipListInternalNode::isKeyLessEqualTo(const char *key, KeyComparator *keyComparator) {
                return keyComparator->compare(this->key, Slice(key)) <= 0;
            }

            KeyValuePair SkipListInternalNode::keyValuePair() {
                return KeyValuePair(Slice(this->key, this->keySize), Slice(""));
            }

            SkipListNode *SkipListInternalNode::getDown() {
                return this->down;
            }

            void SkipListInternalNode::attach(SkipListLeafNode *down) {
                this->down = down;
            }

            SkipListInternalNode* SkipListInternalNode::next() {
                SkipListInternalNode *current = this;
                if (current != nullptr) {
                    current = current->forwards[0];
                }
                return current;
            }

            std::pair<SkipListNode *, bool> SkipListInternalNode::getBy(const Slice& key, KeyComparator *keyComparator) {
                SkipListInternalNode *current = this;
                for (int level = this->forwards.size() - 1; level >= 0; level--) {
                    while (current->forwards[level] && keyComparator->compare(current->forwards[level]->key, key) < 0) {
                        current = current->forwards[level];
                    }
                }
                current = current->forwards[0];

                if (current && keyComparator->compare(current->key, key) == 0) {
                    return std::make_pair(current, true);
                }
                return std::make_pair(nullptr, false);
            }

            std::pair<SkipListNode *, bool>
            SkipListInternalNode::scan(const Slice& beginKey, KeyComparator *keyComparator) {
                SkipListInternalNode *current = this;
                for (int level = this->forwards.size() - 1; level >= 0; level--) {
                    while (current->forwards[level] &&
                           keyComparator->compare(current->forwards[level]->key, beginKey) <= 0) {
                        current = current->forwards[level];
                    }
                }
                if (current && keyComparator->compare(current->key, beginKey) < 0) {
                    current = current->forwards[0];
                }
                if (current != nullptr) {
                    return std::make_pair(current->down, true);
                }
                return std::make_pair(nullptr, false);
            }

            PutPosition
            SkipListInternalNode::putPositionOf(const Slice& key, KeyComparator *keyComparator, LevelGenerator* levelGenerator) {
                SkipListInternalNode *current = this;
                std::vector < SkipListInternalNode * > positions(this->forwards.size(), nullptr);
                for (int level = this->forwards.size() - 1; level >= 0; level--) {
                    while (current->forwards[level] && keyComparator->compare(current->forwards[level]->key, key) < 0) {
                        current = current->forwards[level];
                    }
                    positions[level] = current;
                }
                current = current->forwards[0];
                if (current == nullptr || keyComparator->compare(current->key, key) != 0) {
                    int newLevel = levelGenerator->generate();
                    return PutPosition{positions, newLevel, this, positions[0]->down};
                }
                std::vector < SkipListInternalNode * > empty;
                return PutPosition{empty, -1, nullptr, nullptr};
            }

            SkipListNode *SkipListInternalNode::put(const Slice& key,
                                                    const std::vector<SkipListInternalNode *>& positions,
                                                    int nodeLevel) {

                SkipListInternalNode *newNode = new SkipListInternalNode(key, nodeLevel);
                for (int level = 0; level < nodeLevel; level++) {
                    newNode->forwards[level] = positions[level]->forwards[level];
                    positions[level]->forwards[level] = newNode;
                }
                return newNode;
            }

            UpdatePosition SkipListInternalNode::updatePositionOf(const Slice& key, KeyComparator *keyComparator) {
                SkipListInternalNode *current = this;
                for (int level = this->forwards.size() - 1; level >= 0; level--) {
                    while (current->forwards[level] && keyComparator->compare(current->forwards[level]->key, key) < 0) {
                        current = current->forwards[level];
                    }
                }
                current = current->forwards[0];
                if (current && keyComparator->compare(current->key, key) == 0) {
                    return UpdatePosition{current, current->down};
                }
                return UpdatePosition{nullptr, nullptr};
            }

            DeletePosition SkipListInternalNode::deletePositionOf(const Slice& key, KeyComparator *keyComparator) {
                SkipListInternalNode *current = this;
                std::vector < SkipListInternalNode * > positions(this->forwards.size(), nullptr);

                for (int level = this->forwards.size() - 1; level >= 0; level--) {
                    while (current->forwards[level] && keyComparator->compare(current->forwards[level]->key, key) < 0) {
                        current = current->forwards[level];
                    }
                    positions[level] = current;
                }
                current = current->forwards[0];
                if (current != nullptr && keyComparator->compare(current->key, key) == 0) {
                    return DeletePosition{positions, (int) this->forwards.size(), current, positions[0]->down};
                }
                return DeletePosition{positions, -1, nullptr, nullptr};
            }

            void SkipListInternalNode::deleteBy(const Slice& key,
                                                const std::vector<SkipListInternalNode *>& positions,
                                                int deleteLevel,
                                                KeyComparator *keyComparator) {

                SkipListInternalNode *current = this;
                if (current != nullptr && keyComparator->compare(current->key, key) == 0) {
                    for (int level = 0; level < deleteLevel; level++) {
                        if (positions[level]->forwards[level] != current) {
                            break;
                        }
                        positions[level]->forwards[level] = current->forwards[level];
                    }
                    //we nullify the down-pointer and invoke SkipListInternalNode's destructor.
                    //actual SkipListLeafNode gets deleted in SkipListLeafNode -> delete(..) method
                    current->down = nullptr;
                    delete current;
                }
            }

            unsigned long SkipListInternalNode::totalKeys() {
                SkipListInternalNode *current = this;
                unsigned long count = 0;

                while (current->forwards[0] != nullptr) {
                    current = current->forwards[0];
                    count = count + 1;
                }
                return count;
            }

            void SkipListInternalNode::close() {
                SkipListInternalNode* current = this;
                SkipListInternalNode* previous = nullptr;

                while(current) {
                    previous = current;
                    current = current->forwards[0];

                    delete previous;
                }
            }
        }
    }
}