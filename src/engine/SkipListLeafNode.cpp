#include "SkipListLeafNode.h"
#include "KeyValuePair.h"
#include "PersistentMemoryPool.h"

namespace pmem {
    namespace storage {
        namespace internal {
            SkipListLeafNode::SkipListLeafNode() {
                this->right = nullptr;
            }

            void SkipListLeafNode::persist() {
                pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance()->getPmpool();

                transaction::run(pmpool, [&] {
                    persistent_ptr<pmem::storage::internal::PersistentLeaf> newLeaf = make_persistent<pmem::storage::internal::PersistentLeaf>();
                    this->leaf = newLeaf;
                    this->leaf.get()->put("", "");
                });
            }

            bool SkipListLeafNode::isLeaf() {
                return true;
            }

            bool SkipListLeafNode::matchesKey(std::string key) const {
                pmem::storage::internal::PersistentLeaf *leaf = this->leaf.get();
                return std::string(leaf->key()) == key;
            }

            bool SkipListLeafNode::isKeyLessEqualTo(std::string key) {
                pmem::storage::internal::PersistentLeaf *leaf = this->leaf.get();
                return std::string(leaf->key()) <= key;
            }

            pmem::storage::KeyValuePair SkipListLeafNode::keyValuePair() {
                pmem::storage::internal::PersistentLeaf *leaf = this->leaf.get();
                return KeyValuePair(std::string(leaf->key()), std::string(leaf->value()));
            }

            pmem::storage::KeyValuePair SkipListLeafNode::rightKeyValuePair() {
                pmem::storage::internal::PersistentLeaf *right = this->leaf->right.get();
                if (right) {
                    return KeyValuePair(std::string(right->key()), std::string(right->value()));
                }
                return KeyValuePair("", "");
            }

            std::pair<SkipListLeafNode *, Status>
            SkipListLeafNode::put(std::string key, std::string value, std::function<void(void)> postPutHook) {
                pmem::storage::internal::PersistentLeaf *targetLeaf = this->leaf.get();
                SkipListLeafNode *targetNode = this;

                while (targetLeaf->right.get() && std::string(targetLeaf->right.get()->key()) <= key) {
                    targetLeaf = targetLeaf->right.get();
                    targetNode = targetNode->right;
                }
                pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance()->getPmpool();

                SkipListLeafNode *newNode = new SkipListLeafNode();
                newNode->right = targetNode->right;
                targetNode->right = newNode;

                try {
                    transaction::run(pmpool, [&] {
                        persistent_ptr<pmem::storage::internal::PersistentLeaf> newLeaf = make_persistent<pmem::storage::internal::PersistentLeaf>();
                        newNode->leaf = newLeaf;
                        newNode->leaf.get()->put(key, value);

                        newNode->leaf.get()->right = targetLeaf->right;
                        targetLeaf->right = newLeaf;
                        postPutHook();
                    });
                } catch (...) {
                    return std::make_pair(nullptr, Status::Failed);
                }
                return std::make_pair(newNode, Status::Ok);
            }

            std::pair<std::string, bool> SkipListLeafNode::getBy(std::string key) {
                pmem::storage::internal::PersistentLeaf *targetLeaf = this->leaf.get();
                while (targetLeaf->right.get() && std::string(targetLeaf->right.get()->key()) <= key) {
                    targetLeaf = targetLeaf->right.get();
                }
                if (std::string(targetLeaf->key()) == key) {
                    return std::make_pair(std::string(targetLeaf->value()), true);
                }
                return std::make_pair("", false);
            }

            std::vector <pmem::storage::KeyValuePair>
            SkipListLeafNode::scan(std::string beginKey, std::string endKey, int64_t maxPairs) {
                pmem::storage::internal::PersistentLeaf *targetLeaf = this->leaf.get();
                while (targetLeaf->right.get() && std::string(targetLeaf->right.get()->key()) <= beginKey) {
                    targetLeaf = targetLeaf->right.get();
                }
                if (std::string(targetLeaf->key()) < beginKey) {
                    targetLeaf = targetLeaf->right.get();
                }

                std::vector <pmem::storage::KeyValuePair> keyValuePairs;
                int64_t pairCount = 0;

                while (targetLeaf && std::string(targetLeaf->key()) < endKey) {
                    keyValuePairs.push_back(KeyValuePair(targetLeaf->key(), targetLeaf->value()));
                    targetLeaf = targetLeaf->right.get();
                    pairCount = pairCount + 1;

                    if (pairCount == maxPairs) {
                        break;
                    }
                }
                return keyValuePairs;
            }

            Status
            SkipListLeafNode::update(std::string key, std::string value, std::function<void(void)> postUpdateHook) {
                pmem::storage::internal::PersistentLeaf *targetLeaf = this->leaf.get();
                while (targetLeaf->right.get() && std::string(targetLeaf->right.get()->key()) <= key) {
                    targetLeaf = targetLeaf->right.get();
                }

                pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance()->getPmpool();
                try {
                    if (std::string(targetLeaf->key()) == key) {
                        transaction::run(pmpool, [&] {
                            targetLeaf->put(key, value);
                            postUpdateHook();
                        });
                    }
                } catch (...) {
                    return Status::Failed;
                }
                return Status::Ok;
            }

            Status SkipListLeafNode::deleteBy(std::string key, std::function<void(void)> postDeleteHook) {
                pmem::storage::internal::PersistentLeaf *previousLeaf = nullptr;
                pmem::storage::internal::PersistentLeaf *targetLeaf = this->leaf.get();

                SkipListLeafNode *previousNode = nullptr;
                SkipListLeafNode *targetNode = this;

                while (targetLeaf->right.get() && std::string(targetLeaf->right.get()->key()) <= key) {
                    previousNode = targetNode;
                    targetNode = targetNode->right;

                    previousLeaf = targetLeaf;
                    targetLeaf = targetLeaf->right.get();
                }

                pmem::obj::pool_base pmpool = PersistentMemoryPool::getInstance()->getPmpool();
                SkipListLeafNode *previousRight = previousNode->right;
                SkipListLeafNode *targetRight = targetNode->right;
                persistent_ptr<pmem::storage::internal::PersistentLeaf> persistentLeaf = targetNode->leaf;

                try {
                    if (std::string(targetLeaf->key()) == key) {
                        previousNode->right = targetNode->right;
                        targetNode->right = nullptr;

                        transaction::run(pmpool, [&] {
                            targetLeaf->clear();
                            previousLeaf->right = targetLeaf->right;
                            targetLeaf->right = nullptr;

                            delete_persistent<pmem::storage::internal::PersistentLeaf>(targetNode->leaf);
                            targetNode->leaf = nullptr;
                            postDeleteHook();
                        });
                        delete targetNode;
                    }
                } catch (...) {
                    previousNode->right = previousRight;
                    targetNode->right = targetRight;
                    targetNode->leaf = persistentLeaf;

                    return Status::Failed;
                }
                return Status::Ok;
            }
        }
    }
}