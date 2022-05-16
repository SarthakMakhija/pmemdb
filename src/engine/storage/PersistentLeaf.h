#ifndef _PersistentLeafNode_
#define _PersistentLeafNode_

#include <libpmemobj++/make_persistent.hpp>
#include <libpmemobj++/make_persistent_array.hpp>
#include <libpmemobj++/persistent_ptr.hpp>
#include <libpmemobj++/transaction.hpp>
#include <libpmemobj++/pool.hpp>
#include <libpmemobj/pool_base.h>
#include "db/KeyValueSize.h"
#include "Slice.h"

using pmem::obj::delete_persistent;
using pmem::obj::make_persistent;
using pmem::obj::persistent_ptr;
using pmem::obj::pool;
using pmem::obj::transaction;

namespace pmem {
    namespace storage {
        namespace internal {
            struct PersistentLeaf {
                persistent_ptr <PersistentLeaf> right;
                persistent_ptr<char[]> keyValue;

                static void setKeySizeDirect(char *p, uint32_t v) {
                    *((uint32_t * )(p)) = v;
                }

                static void setValueSizeDirect(char *p, uint32_t v) {
                    *((uint32_t * )((char *) (p) + sizeof(uint32_t))) = v;
                }

                void put(const pmem::storage::Slice& key, const pmem::storage::Slice& value) {
                    if (keyValue) {
                        char *p = keyValue.get();
                        delete_persistent<char[]>(keyValue, sizeof(uint32_t) + sizeof(uint32_t) + keySizeDirect(p) +
                                                            valueSizeDirect(p) );
                    }
                    size_t ksize = key.size();
                    size_t vsize = value.size();
                    size_t size = ksize + vsize + sizeof(uint32_t) + sizeof(uint32_t);

                    keyValue = make_persistent<char[]>(size);

                    char *p = keyValue.get();
                    setKeySizeDirect(p, (uint32_t) ksize);
                    setValueSizeDirect(p, (uint32_t) vsize);

                    char *kvptr = p + sizeof(uint32_t) + sizeof(uint32_t);
                    memcpy(kvptr, key.cdata(), ksize);
                    kvptr += ksize;
                    memcpy(kvptr, value.cdata(), vsize);
                }

                void clear() {
                    if (keyValue) {
                        char *p = keyValue.get();
                        //setKeySizeDirect(p, 0);
                        setValueSizeDirect(p, 0);
                        delete_persistent<char[]>(keyValue, sizeof(uint32_t) + sizeof(uint32_t) + keySizeDirect(p) +
                                                            valueSizeDirect(p) );
                        keyValue = nullptr;
                    }
                }

                Slice key() {
                    const char* key = ((char *) (keyValue.get()) + sizeof(uint32_t) + sizeof(uint32_t));
                    return Slice(key, keySize());
                }

                const char *value() const {
                    return ((char *) (keyValue.get()) + sizeof(uint32_t) + sizeof(uint32_t) + keySize() );
                }

                uint32_t keySize() const {
                    return *((uint32_t * )(keyValue.get()));
                }

                uint32_t valueSize() const {
                    return *((uint32_t * )((char *) (keyValue.get()) + sizeof(uint32_t)));
                }

                static uint32_t keySizeDirect(char *p) {
                    return *((uint32_t * )(p));
                }

                static uint32_t valueSizeDirect(char *p) {
                    return *((uint32_t * )((char *) (p) + sizeof(uint32_t)));
                }
            };
        }
    }
}

#endif