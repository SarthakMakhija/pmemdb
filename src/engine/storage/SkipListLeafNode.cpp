#include "SkipListLeafNode.h"

namespace pmem {
    namespace storage {
        namespace internal {
            SkipListLeafNode::SkipListLeafNode() {
                this->right = nullptr;
            }

            SkipListLeafNode::~SkipListLeafNode() {
                this->right = nullptr;
            }

            void SkipListLeafNode::persist(PersistentMemoryPool *persistentMemoryPool) {
                pmem::obj::pool_base pmpool = persistentMemoryPool->getPmpool();

                transaction::run(pmpool, [&] {
                    persistent_ptr<pmem::storage::internal::PersistentLeaf> newLeaf = make_persistent<pmem::storage::internal::PersistentLeaf>();
                    this->leaf = newLeaf;
                    size_t blankSize = strlen("") + 1;
                    this->leaf.get()->put(Slice("", blankSize), Slice("", blankSize));
                });
            }

            bool SkipListLeafNode::matchesKey(const char *key, pmem::storage::KeyComparator *keyComparator) const {
                pmem::storage::internal::PersistentLeaf *leaf = this->leaf.get();
                return keyComparator->compare(leaf->key(), Slice(key)) == 0;
            }

            bool SkipListLeafNode::isKeyLessEqualTo(const char *key, pmem::storage::KeyComparator *keyComparator) {
                pmem::storage::internal::PersistentLeaf *leaf = this->leaf.get();
                return keyComparator->compare(leaf->key(), Slice(key)) <= 0;
            }

            pmem::storage::KeyValuePair SkipListLeafNode::keyValuePair() {
                pmem::storage::internal::PersistentLeaf *leaf = this->leaf.get();
                return KeyValuePair(leaf->key().cdata(), leaf->value().cdata());
            }

            pmem::storage::KeyValuePair SkipListLeafNode::rightKeyValuePair() {
                pmem::storage::internal::PersistentLeaf *right = this->leaf->right.get();
                if (right) {
                    return KeyValuePair(right->key().cdata(), right->value().cdata());
                }
                return KeyValuePair("", "");
            }

            std::pair<SkipListLeafNode *, Status>
            SkipListLeafNode::put(const pmem::storage::Slice& key,
                                  const pmem::storage::Slice& value,
                                  pmem::storage::KeyComparator *keyComparator,
                                  pmem::storage::internal::PersistentMemoryPool* persistentMemoryPool,
                                  std::function<void(void)> postPutHook) {

                pmem::storage::internal::PersistentLeaf *targetLeaf = this->leaf.get();
                SkipListLeafNode *targetNode = this;

                while (targetLeaf->right.get() && keyComparator->compare(targetLeaf->right.get()->key(), key) <= 0) {
                    targetLeaf = targetLeaf->right.get();
                    targetNode = targetNode->right;
                }

                pmem::obj::pool_base pmpool = persistentMemoryPool->getPmpool();
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

            std::pair<Slice, bool>
            SkipListLeafNode::getBy(const pmem::storage::Slice& key, pmem::storage::KeyComparator *keyComparator) {
                pmem::storage::internal::PersistentLeaf *targetLeaf = this->leaf.get();
                while (targetLeaf->right.get() && keyComparator->compare(targetLeaf->right.get()->key(), key) <= 0) {
                    targetLeaf = targetLeaf->right.get();
                }
                if (keyComparator->compare(targetLeaf->key(), key) == 0) {
                    return std::make_pair(targetLeaf->value(), true);
                }
                return std::make_pair(Slice(""), false);
            }

            std::vector <pmem::storage::KeyValuePair>
            SkipListLeafNode::scan(const pmem::storage::Slice& beginKey,
                                   const pmem::storage::Slice& endKey,
                                   int64_t maxPairs,
                                   pmem::storage::KeyComparator *keyComparator) {

                pmem::storage::internal::PersistentLeaf *targetLeaf = this->leaf.get();
                while (targetLeaf->right.get() &&
                       keyComparator->compare(targetLeaf->right.get()->key(), beginKey) <= 0) {
                    targetLeaf = targetLeaf->right.get();
                }
                if (keyComparator->compare(targetLeaf->key(), beginKey) < 0) {
                    targetLeaf = targetLeaf->right.get();
                }

                std::vector <pmem::storage::KeyValuePair> keyValuePairs;
                int64_t pairCount = 0;

                while (targetLeaf && keyComparator->compare(targetLeaf->key(), endKey) < 0) {
                    keyValuePairs.push_back(KeyValuePair(targetLeaf->key().cdata(), targetLeaf->value().cdata()));
                    targetLeaf = targetLeaf->right.get();
                    pairCount = pairCount + 1;

                    if (pairCount == maxPairs) {
                        break;
                    }
                }
                return keyValuePairs;
            }

            Status
            SkipListLeafNode::update(const pmem::storage::Slice& key,
                                     const pmem::storage::Slice& value,
                                     pmem::storage::KeyComparator *keyComparator,
                                     pmem::storage::internal::PersistentMemoryPool* persistentMemoryPool,
                                     std::function<void(void)> postUpdateHook) {

                pmem::storage::internal::PersistentLeaf *targetLeaf = this->leaf.get();
                while (targetLeaf->right.get() && keyComparator->compare(targetLeaf->right.get()->key(), key) <= 0) {
                    targetLeaf = targetLeaf->right.get();
                }

                pmem::obj::pool_base pmpool = persistentMemoryPool->getPmpool();
                try {
                    if (keyComparator->compare(targetLeaf->key(), key) == 0) {
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

            Status SkipListLeafNode::deleteBy(const pmem::storage::Slice& key,
                                              pmem::storage::KeyComparator *keyComparator,
                                              PersistentMemoryPool* pool,
                                              std::function<void(void)> postDeleteHook) {

                pmem::storage::internal::PersistentLeaf *previousLeaf = nullptr;
                pmem::storage::internal::PersistentLeaf *targetLeaf = this->leaf.get();

                SkipListLeafNode *previousNode = nullptr;
                SkipListLeafNode *targetNode = this;

                while (targetLeaf->right.get() && keyComparator->compare(targetLeaf->right.get()->key(), key) <= 0) {
                    previousNode = targetNode;
                    targetNode = targetNode->right;

                    previousLeaf = targetLeaf;
                    targetLeaf = targetLeaf->right.get();
                }

                pmem::obj::pool_base pmpool = pool->getPmpool();
                SkipListLeafNode *previousRight = previousNode->right;
                SkipListLeafNode *targetRight = targetNode->right;
                persistent_ptr<pmem::storage::internal::PersistentLeaf> persistentLeaf = targetNode->leaf;

                try {
                    if (keyComparator->compare(targetLeaf->key(), key) == 0) {
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